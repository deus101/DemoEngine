#include "renderPacket.h"


//#include "../util.h"
#include "../Engine/asset.h"
//using namespace NS_ENG;
using namespace NS_VEC;
using namespace NS_EFF;

//I should go back to using context class/objects
//renderPacket::renderPacket(const NS_REND::context &aContext)
renderPacket::renderPacket()
{

	//typedef boost::shared_ptr< struct EffectStage > EffectStagePtr;
	//typedef boost::shared_ptr<const struct EffectStage> EffectStageConstPtr;


	m_shaderProg = 0;

	m_StageParameters.StageValue = 0;
	
	m_StageParamPtr = boost::make_shared<EffectStage>(m_StageParameters);

	m_StageParamConstPtr = boost::const_pointer_cast<const  EffectStage>(m_StageParamPtr);

}
renderPacket::~renderPacket()
{
	//m_StageParamPtr.
	//delete &m_StageParamPtr;
	//NS_ENG::asset::CurrentStage.
	//delete &this->m_StageParameters;
	//NS_ENG::asset::SetCurrentStage(NULL);
	//NS_ENG::asset::SetCurrentStage(m_StageParamConstPtr);
	//this->m_StageParamConstPtr.unique();
	//this->m_StageParamConstPtr.owner_before()
	//delete m_StageParamPtr;
	//delete this->m_StageParameters

}
bool renderPacket::Init()
{
	//I want to let my packets keep track of the samplers it needs
	//opprette bufferne her?
	m_shaderProg = glCreateProgram();

	//m_shaderProg = context.getProgram();
	if (m_shaderProg == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;

}


bool renderPacket::LoadShader(GLenum ShaderType, const char *fileName)
{
	
	GLint success;
	GLint shaderLength = 0;

	std::cout << fileName << std::endl;
	const GLchar *glslStringPtr[1];
	

	GLchar *shaderText = NULL;
	
	FILE *fp;


	fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		while (fgetc(fp) != EOF)
		{
			shaderLength++;
		}
		rewind(fp);
		shaderText = (GLchar *)malloc(shaderLength + 1);
		if (shaderText != NULL)
		{
			fread(shaderText, 1, shaderLength, fp);
		}
		shaderText[shaderLength] = '\0';
		fclose(fp);
	}


	if (shaderText == NULL)
		return false;

	glslStringPtr[0] = shaderText;

	GLuint ShaderObj = glCreateShader(ShaderType);

	m_shaderObjList.push_back(ShaderObj);

	
	glShaderSource(ShaderObj, 1, glslStringPtr, NULL);


	glCompileShader(ShaderObj);

	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[2048];
		glGetShaderInfoLog(ShaderObj, 2048, NULL, infoLog);
		fprintf(stderr, "epic Vertex #%d ffffail!\n", ShaderObj);
		fprintf(stderr, "%s\n", infoLog);
		Sleep(10000);
		return false;
	}

	glAttachShader(m_shaderProg, ShaderObj);

	return true;
}

bool renderPacket::Finalize()
{
	glLinkProgram(m_shaderProg);

	GLint success;

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[2048];
		glGetProgramInfoLog(m_shaderProg, 2048, NULL, infoLog);

		fprintf(stderr, "%s\n", infoLog);
		Sleep(10000);
		return false;
	}


	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[2048];
		glGetProgramInfoLog(m_shaderProg, 2048, NULL, infoLog);

		fprintf(stderr, "%s\n", infoLog);
		Sleep(10000);
		return false;
	}
	std::cout << "ShaderObject! : " << m_shaderProg << std::endl;
	//glUseProgram(m_shaderProg);
	return true;

}


void renderPacket::Enable()
{
	NS_ENG::asset::SetCurrentStage(m_StageParamConstPtr);
	glUseProgram(m_shaderProg);
}


GLint renderPacket::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint renderPacket::GetProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(m_shaderProg, param, &ret);
	return ret;
}
