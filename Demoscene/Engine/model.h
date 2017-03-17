#ifndef MODEL_HPP
#define MODEL_HPP


#include <string>
#include <vector>
#include <deque> 
#include <map>


#include "mesh.h"
#include "materials.h"
#include "../Effect/GeomPacket.h"
#include "../util.h"

//#include "../Rendrer/context.h"



//http://stackoverflow.com/questions/16501419/creating-a-class-with-a-static-member-container-holding-all-instances-of-said-cl
//Hurm isnt it safer with a composite class?

namespace NS_ENG{

	//AHAAA! JEG KAN GENNE FRA GRUPPE, BUFFER GRUPPENE TIL MODDELEN FLERE NODER! 
	//Alt ligger p� p� plass! og model noden trenger jo bare � kj�re addchildren eller gi den til faren

	struct buffer_Group
	{
		buffer_Group()
		{
			IBO.clear();
			tex = NULL;
		}
		GLuint vao;
		//I should have called this IBO
		GLuint vbo;
		//And This IBO_Data
		std::vector<unsigned int> IBO;
		//std::vector<unsigned short> IBO;
		s_ModelAid ModelAidChild;

		//been to long...
		//VEC3 amb;
		GLfloat amb[4];
		NS_VEC::VEC3 dif;
		NS_VEC::VEC3 spec;
		NS_VEC::VEC3 emi;
		NS_VEC::VEC3 shiny;
		//GLuint tex;
		GLint tex;
	};

	struct PackedVertex{
		NS_VEC::VEC3 position;
		NS_VEC::VEC2 uv;
		NS_VEC::VEC3 normal;
		bool operator<(const PackedVertex that) const{
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
		};
	};




	class model : public asset
	{
	private:
		//Rename to model Iter
		std::list <model*>::iterator iter;
	public:
		static std::list <model*> classModelList;
		//heh should be private
		std::vector<NS_VEC::VEC3> Sort_Pos;
		std::vector<NS_VEC::VEC3> Sort_Norms;
		std::vector<NS_VEC::VEC2> Sort_Uvs;
		std::vector<NS_VEC::VEC3> Sort_Tang;



		std::vector<buffer_Group> Sort_Groups;

		s_ModelAid ModelAidRoot;
		//GLuint vao_model;
		
		
		GLuint vbo_vertices;
		GLuint vbo_normals;
		GLuint vbo_uv;

		GLuint vbo_BaseTool;
		GLuint vbo_ChildTool;

		//should be grouped per face..so array....
		GLuint vbo_indices;
		model();
		//might need later 
		//model(model const &from)
		model( std::string obj, std::string mtl, bool UV = true, bool Tangent = true);
		~model();
		

		/*
		model &operator=(model const &src) {
			return *this;
		
		*/



		void Draw();
		void Draw(int instances);
	public:



		NS_MESH::MESH meshy;
		//hmmm should I maybe not care how many instances of palette there is...if only I can ID them?
		//NS_MAT
		NS_ENG::MATERIALS palette;
		NS_VEC::VEC3 color;

	private:
		bool getSimilarVertexIndex_fast(
			PackedVertex & packed,
			std::map<PackedVertex, unsigned int> & VertexToOutIndex,
			unsigned int & result
			)
		{

			std::map<PackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
			if (it == VertexToOutIndex.end()){
				return false;
			}
			else{
				result = it->second;
				return true;
			}
		};

		void DumpBuffer();

		void BufferLog();





	};
	//const void loadBuffer(Model &mModel, renderPacket &mPacket);
}
#endif