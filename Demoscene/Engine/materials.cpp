#include "materials.h"
#include "../Libs/FreeImage.h"


#include <algorithm>
#include <stdio.h>
#include <iostream>


using namespace NS_ENG;
using namespace std;


//namespace NS_MAT
//namespace NS_ENG
//{
std::list <s_mat> Material::classMaterialList = std::list <s_mat>();


Material::Material() : asset()
{
	//Mats.Clear();

}


//this should be a static method, and eaach materials object should be stored in the static container as one materials
//hmm but what about shader jutsu?
//void LoadMats(const char *param, MATERIALS& Mats)
void Material::LoadMats( const char *param)
{
	FILE * mtlFile;
	mtlFile = fopen( param , "rb");

	//Mats.Clear();
	
		
	char line[1024] = "";
	char id[512] = "";
	cout << "---------MTL:Begin loading materials from:" << param << endl;
	
	while( fscanf(mtlFile, "%s", id) > 0)
	{

		if(strcmp (id, "newmtl") == 0)
		{
			s_mat mtl;
			char nam[80] ="";
			fscanf(mtlFile, "%79s", nam );
			mtl.name = nam;
			cout << "-material name: " << mtl.name << endl;
			//Mats.m_Materials.push_back( mtl );
			NS_ENG::Material::classMaterialList.push_back(mtl);

			NS_ENG::Material::classMaterialList.back().matID =  NS_ENG::Material::classMaterialList.size();
		}
		if(strcmp (id, "Kd") == 0)
		{
				
			fscanf(mtlFile, "%f %f %f", &NS_ENG::Material::classMaterialList.back().diff[0], &NS_ENG::Material::classMaterialList.back().diff[1], &NS_ENG::Material::classMaterialList.back().diff[2]);
			//fscanf(mtlFile, "%f %f %f", &Mats.m_Materials.back().diff[0], &Mats.m_Materials.back().diff[1], &Mats.m_Materials.back().diff[2]);

			//Mats.m_Materials.back().diff[3] = 1.0;
			NS_ENG::Material::classMaterialList.back().diff[3] = 1.0;
				//input[3] = 1.0;
			//Mats.m_Materials.back().diff = input;
		}
		if(strcmp (id, "Ka") == 0)
		{
				
			//fscanf(mtlFile, "%f %f %f", &Mats.m_Materials.back().amb[0], &Mats.m_Materials.back().amb[1], &Mats.m_Materials.back().amb[2]);
			//Mats.m_Materials.back().amb[3] = 1.0;
			fscanf(mtlFile, "%f %f %f", &NS_ENG::Material::classMaterialList.back().amb[0], &NS_ENG::Material::classMaterialList.back().amb[1], &NS_ENG::Material::classMaterialList.back().amb[2]);
			NS_ENG::Material::classMaterialList.back().amb[3] = 1.0;
		}
		if(strcmp (id, "Ks") == 0)
		{
			
			//fscanf(mtlFile, "%f %f %f", &Mats.m_Materials.back().spec[0], &Mats.m_Materials.back().spec[1], &Mats.m_Materials.back().spec[2]);
			//Mats.m_Materials.back().spec[3] = 1.0;
			fscanf(mtlFile, "%f %f %f", &NS_ENG::Material::classMaterialList.back().spec[0], &NS_ENG::Material::classMaterialList.back().spec[1], &NS_ENG::Material::classMaterialList.back().spec[2]);
			NS_ENG::Material::classMaterialList.back().spec[3] = 1.0;
		}
		if(strcmp (id, "Ke") == 0)
		{	
			NS_VEC::VEC3 c;
			//fscanf(mtlFile, "%f %f %f", &Mats.m_Materials.back().emmi[0], &Mats.m_Materials.back().emmi[1], &Mats.m_Materials.back().emmi[2]);
			//Mats.m_Materials.back().emmi[3] = 1.0;
			fscanf(mtlFile, "%f %f %f", &NS_ENG::Material::classMaterialList.back().emmi[0], &NS_ENG::Material::classMaterialList.back().emmi[1], &NS_ENG::Material::classMaterialList.back().emmi[2]);
			NS_ENG::Material::classMaterialList.back().emmi[3] = 1.0;

		}
		if(strcmp (id, "Ns") == 0)
		{
			GLfloat shin = 0.0f;
			fscanf(mtlFile, "%f", &shin);
			//Mats.m_Materials.back().shiny = shin;
			NS_ENG::Material::classMaterialList.back().shiny = shin;
		}
		else if (strcmp(id, "map_Kd") == 0)
		{

			//here we implement a map object...
			bool duplicate = false;
			//load
			GLuint tmp_TName;
				
			char path[80];

			fscanf(mtlFile, "%79s", path);
			string tmp_Path(path);

			for (auto MatIter : NS_ENG::Material::classMaterialList) 	
			{

					//cout << "mats :" << MatIter.matID << endl;
					//if (obj.compare(MatIter->meshy.file_name) == 0)
					if (tmp_Path.compare(MatIter.tPath) == 0)
					{
						cout << " The material: " << MatIter.matID << ":" << MatIter.name << " have allready loaded the texture " << MatIter.tPath << endl;
						cout << " Using the GlName :" << MatIter.tUnit << endl;
						//meshy.m_Groups[u].matid = j;
						//meshy.m_Groups[u].matid = MatIter.matID;
						NS_ENG::Material::classMaterialList.back().tUnit = MatIter.tUnit;
						NS_ENG::Material::classMaterialList.back().tPath = tmp_Path;
						//MG.
						duplicate = true;
						break;
					}

			}



			
			if (!duplicate)
			{
			FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(path, 0);
			FIBITMAP* imagen = FreeImage_Load(formato, path);

			FIBITMAP* temp = imagen;
			imagen = FreeImage_ConvertTo32Bits(imagen);
			FreeImage_Unload(temp);

			int w = FreeImage_GetWidth(imagen);
			int h = FreeImage_GetHeight(imagen);
			cout << " Loading Map into memory:" << path << endl;
			cout << " The size of the image is: " << " es " << w << "*" << h << endl; 
			GLubyte* textura = new GLubyte[4 * w*h];
			char* pixeles = (char*)FreeImage_GetBits(imagen);
			

			for (int j = 0; j<w*h; j++){
				textura[j * 4 + 0] = pixeles[j * 4 + 2];
				textura[j * 4 + 1] = pixeles[j * 4 + 1];
				textura[j * 4 + 2] = pixeles[j * 4 + 0];
				textura[j * 4 + 3] = pixeles[j * 4 + 3];
			}

			glGenTextures(1, &tmp_TName);
			glBindTexture(GL_TEXTURE_2D, tmp_TName);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			GLenum huboError = glGetError();
			if (huboError){

				cout << " There was an error loading the texture" << endl;
			}
			cout << " The Texture name is " << tmp_TName << endl << endl;
			//Mats.m_Materials.back().tUnit = tmp_TName;
			NS_ENG::Material::classMaterialList.back().tUnit = tmp_TName;
			NS_ENG::Material::classMaterialList.back().tPath = tmp_Path;
			}
		}

	}


		fclose(mtlFile);

		cout << " NR of materials: " << NS_ENG::Material::classMaterialList.size() << endl << "Done loading MTL---------" <<endl << endl;
		//cout << "NR of materials: " << Mats.m_Materials.size() << endl;
}

//}
