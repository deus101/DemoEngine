#ifndef RENDRER_HPP
#define RENDRER_HPP
//   model kaller p� rendrer eller loop henter shader, matrial og modell indexer
#include "../SceneGraph/composite.h"
#include "../SceneGraph/camera.h"
#include "../Math/math3d.h"
#include "../Rendrer/context.h"
#include "../Rendrer/callbacks.h"

#include "../SceneGraph/modelNode.h"
#include "../SceneGraph/pointLightNode.h"
#include "../SceneGraph/dirLightNode.h"
#include "model.h"
//rendrern can ta over for compositt klassen... eller kansje ikke hva har man namespaces til...
//men er konteinere I namespaces en god ide? 
namespace NS_ENG
{ 


	struct RendrerItem
	{
		//RendrerItem()
		//{
		//	
		//}

		//VEC3 m_vNor;
		//referanse til objekt referanse burde v�rt her
		//burde v�rt detrte men pr�ver mer direkt metode f�rst
		//NS_SG::assetNode *sNode;
		//shared_ptr damnit
		NS_SG::modelNode *sNode;
		M3DMatrix44f sTransform;
		M3DMatrix44f sWVP;
	};

	

	struct PointLightItem
	{
		NS_SG::pointLightNode *sNode;
		PointLight sPL;
		M3DMatrix44f sWVP;



	};

	struct DirLightItem
	{
		NS_SG::dirLightNode *sNode;
		DirectionalLight sDL;
		M3DMatrix44f sWVP;
	};

class rendrer : public ICallbacks
{
public:
	//                                should this just be name?  
	rendrer(NS_SG::composite *_scene, NS_SG::camera *_camera, NS_ENG::model * _sphereL, NS_ENG::model * _sphereN, NS_ENG::model * _quad) : scene(_scene), kamera(_camera), sphere_light(_sphereL), sphere_null(_sphereN), quad(_quad)
	{

		unsigned int w = GetPixelWidth();
		unsigned int h = GetPixelHeight();
		mgBuffer = new GBuffer();
		mgBuffer->Init(w, h);

		M3DMatrix44f inverseView, viewT;
		m3dLoadIdentity44(view);
		kamera->getProjection(projection);
		//kamera->getAbsoluteTransform(view);
		kamera->getParent()->getLocalTransform(view);
		//m3dTransposeMatrix44(viewT, view);
		//m3dInvertMatrix44(inverseView, view);
		//view[0] = 1.0f;
		//view[13]
		//view[14] =
		//M3DVector4f vect, eyePos;
		//vect[0] = 0.0f;
		//vect[1] = 0.0f;
		//vect[2] = 0.0f;
		//vect[3] = 1.0f;
		//m3dTransformVector4(eyePos, vect, inverseView);

		//m3dTransla
		//kamera->getAbsoluteTransform(view);
		//m3dTransposeMatrix44(, inversLookat);
		//m3dInvertMatrix44(view, view);
		//kamera->getParent()->getLocalTransform(view);
		std::cout << "[" << projection[0] << "] ";
		std::cout << "[" << projection[1] << "] ";
		std::cout << "[" << projection[2] << "] ";
		std::cout << "[" << projection[3] << "] " << endl;
		std::cout << "[" << projection[4] << "] ";
		std::cout << "[" << projection[5] << "] ";
		std::cout << "[" << projection[6] << "] ";
		std::cout << "[" << projection[7] << "] " << endl;
		std::cout << "[" << projection[8] << "] ";
		std::cout << "[" << projection[9] << "] ";
		std::cout << "[" << projection[10] << "] ";
		std::cout << "[" << projection[11] << "] " << endl;
		std::cout << "[" << projection[12] << "] ";
		std::cout << "[" << projection[13] << "] ";
		std::cout << "[" << projection[14] << "] ";
		std::cout << "[" << projection[15] << "] " << endl;


		std::cout << "[" << view[0] << "] ";
		std::cout << "[" << view[1] << "] ";
		std::cout << "[" << view[2] << "] ";
		std::cout << "[" << view[3] << "] " << endl;
		std::cout << "[" << view[4] << "] ";
		std::cout << "[" << view[5] << "] ";
		std::cout << "[" << view[6] << "] ";
		std::cout << "[" << view[7] << "] " << endl;
		std::cout << "[" << view[8] << "] ";
		std::cout << "[" << view[9] << "] ";
		std::cout << "[" << view[10] << "] ";
		std::cout << "[" << view[11] << "] " << endl;
		std::cout << "[" << view[12] << "] ";
		std::cout << "[" << view[13] << "] ";
		std::cout << "[" << view[14] << "] ";
		std::cout << "[" << view[15] << "] " << endl;




		//		NS_ENG::model sphere = NS_ENG::model(mContext, std::string("../Mesh/sphere.obj"), std::string("../Mesh/sphere.mtl"));

	}
	//using nodePtr might be a mistake
	void visit(NS_SG::node *Node, M3DMatrix44f world);
	//void draw();
	void Run();
	virtual void RenderSceneCB();
	//virtual void RenderSceneCB();

	float CalcPointLightBSphere(const PointLight& Light);

private:
	NS_SG::composite *scene;
	NS_SG::camera *kamera;
	//NS_REND::context *mContext;

	NS_ENG::model *sphere_light;
	NS_ENG::model *sphere_null;
	NS_ENG::model *quad;
	GBuffer *mgBuffer;

	std::list< struct RendrerItem> Visible;
	typedef std::list< struct RendrerItem>::iterator vIT;
	typedef std::list< struct RendrerItem>::const_iterator vITc;
	vIT beginVisible(){ return Visible.begin(); }
	vIT endVisible(){ return Visible.end(); }
	vITc beginVisible() const{ return Visible.begin(); }
	vITc endVisible()  const { return Visible.end(); }


	std::list< struct PointLightItem> VisiblePoint;
	typedef std::list< struct PointLightItem>::iterator vPIT;
	typedef std::list< struct PointLightItem>::const_iterator vPITc;
	vPIT beginVisiblePoint(){ return VisiblePoint.begin(); }
	vPIT endVisiblePoint(){ return VisiblePoint.end(); }
	vPITc beginVisiblePoint() const{ return VisiblePoint.begin(); }
	vPITc endVisiblePoint()  const { return VisiblePoint.end(); }


	std::list< struct DirLightItem> VisibleDir;
	typedef std::list< struct DirLightItem>::iterator vDIT;
	typedef std::list< struct DirLightItem>::const_iterator vDITc;
	vDIT beginVisibleDir(){ return VisibleDir.begin(); }
	vDIT endVisibleDir(){ return VisibleDir.end(); }
	vDITc beginVisibleDir() const{ return VisibleDir.begin(); }
	vDITc endVisibleDir()  const { return VisibleDir.end(); }


	//std::list< struct RendrerItem> Visible;
	//std::list< struct RendrerItem>::iterator vIT;

	//std::list< struct PointLightItem> VisiblePoint;
	//std::list< struct PointLightItem>::iterator vPIT;

	//std::list< struct DirLightItem> VisibleDir;
	//std::list< struct DirLightItem>::iterator vDIT;


	//typedef std::list<node*>::iterator child_iterator;
	//typedef std::list<node *>::const_iterator child_const_iterator;
	//child_iterator beginChildren() { return children.begin(); }
	//child_iterator endChildren() { return children.end(); }
	//child_const_iterator beginChildren() const { return children.begin(); }
	//child_const_iterator endChildren() const { return children.end(); }

public:
	M3DMatrix44f view, projection;


};
}
#endif