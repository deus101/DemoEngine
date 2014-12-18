#ifndef MODEL_HPP
#define MODEL_HPP


#include <string>
#include <vector>
#include <deque> 
#include <map>
//#include "world.h"
#include "actors.h"
#include "mesh.h"
#include "materials.h"

using namespace NS_MESH;
using namespace NS_MAT;
using namespace sg;
using namespace noder;



struct buffer_Group
{
	buffer_Group()
	{
		IBO.clear();
	}
	GLuint vao;
	GLuint vbo;
	vector<unsigned short> IBO;


	VEC3 amb;
	VEC3 dif;
	VEC3 spec;
	VEC3 emi;
	VEC3 shiny;

};

struct PackedVertex{
	VEC3 position;
	VEC2 uv;
	VEC3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};





class model : public actors
{
public:
	vector<VEC3> Sort_Pos;
	vector<VEC3> Sort_Norms;
	vector<VEC2> Sort_Uvs;




	vector<buffer_Group> Sort_Groups;



	GLuint vao_model;
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uv;

	//should be grouped per face..so array....
	GLuint vbo_indices;
	model();
	model(GLfloat, GLfloat, GLfloat, bool, string, string, VEC3);

public:

	virtual void draw();

public:



	MESH meshy;
	MATERIALS palette;
	VEC3 color;

private:
	bool getSimilarVertexIndex_fast(
		PackedVertex & packed,
		std::map<PackedVertex, unsigned short> & VertexToOutIndex,
		unsigned short & result
		)
	{
		std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
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



	//skal man lage en konteiner for alle VAO objektene i composite klassen?

	//GLuint vao_model, vbo_vertices, vbo_normals, vbo_indices;


};



#endif