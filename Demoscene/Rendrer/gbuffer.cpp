


//Todo find out how to split this using a pass system for now I'll just get it all in plce for AO

#include <stdio.h>

#include "../util.h"
#include "gbuffer.h"

GBuffer::GBuffer() 
{
	
	m_fbo = 0;
	//geo_fbo = 0;
	//light_fbo = 0;
	//m_fbo = 0;
	//m_AoTexture = 0;
	m_depthTexture = 0;
	m_finalTexture = 0;
	ZERO_MEM(m_textures);
	//ZERO_MEM(ao_textures);
}

GBuffer::~GBuffer()
{
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);

	}

	if (m_textures[0] != 0) {
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	}

	if (m_depthTexture != 0) {
		glDeleteTextures(1, &m_depthTexture);
		//glDeleteRenderbuffers(1, &m_depthTexture);
	}

	if (m_finalTexture != 0) {
		glDeleteTextures(1, &m_finalTexture);
	}
}


bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{


	GLenum error;
	//GLRC initContext = wglGetCurrentContext();
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);
	
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);


	glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);

	glGenTextures(1, &m_finalTexture);

	glGenTextures(1, &m_depthTexture);





	
	
	

	//Should ditch the entire procedure oh well time to hack samples
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, GL_TEXTURE_2D, m_textures[0], 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, m_textures[0], 0);
	//End of PosDepth


	for (unsigned int i = 1; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);

	}

	
	//But depth and final are still utilized in the geometry fbo, should be be set an seperate FBO
	// depth
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	//gl::GenerateMipmap(gl::TEXTURE_2D);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);


	// final
	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGB,GL_FLOAT, NULL);
	//gl::GenerateMipmap(gl::TEXTURE_2D);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);

	
	GLenum StatusGbuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	printf("FB Status, status: 0x%x\n", StatusGbuffer);
	if (StatusGbuffer != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", StatusGbuffer);
		return false;
	}


	SetNumberOfSamples(ARRAY_SIZE_IN_ELEMENTS(m_textures)+1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}


void GBuffer::StartFrame()
{
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}


void GBuffer::BindForGeomPass()
{
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER,geo_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	GLenum DrawBuffers[] = { 
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };

	
	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
}



/*
void GBuffer::BindForAoPass()
{



	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, ao_fbo);
	glClear(GL_COLOR_BUFFER_BIT);



	glActiveTexture(GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION]);
	glActiveTexture(GL_TEXTURE3 );
	glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_NORMAL]);


}
*/

void GBuffer::BindForStencilPass()
{	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glDrawBuffer(GL_NONE);
}





void GBuffer::BindForLightPass()
{
	//prob not right


	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	//glDrawBuffer(GL_COLOR_ATTACHMENT1);


	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
	}	
	//glActiveTexture(GL_TEXTURE3);
	
	
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, ao_textures[AO_TEXTURE_TYPE_AO_MAP]);

	//glBindTexture(GL_TEXTURE_2D, m_AoTexture);

}


void GBuffer::BindForFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GBuffer::EnablePass(int PassId) {

	switch (PassId) {
	case 0:
		this->StartFrame();
		break;
	case 1:
		this->BindForGeomPass();
		break;
	case 2:
		this->BindForStencilPass();
		break;
	case 3:
		this->BindForLightPass();
		break;
	case 4:
		this->BindForFinalPass();
		break;

	
	}


}