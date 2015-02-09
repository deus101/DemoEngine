#ifndef RENDRER_HPP
#define RENDRER_HPP
//   model kaller p� rendrer eller loop henter shader, matrial og modell indexer
#include "composite.h"
#include "camera.h"
#include "math3d.h"
//rendrern can ta over for compositt klassen... eller kansje ikke hva har man namespaces til...
//men er konteinere I namespaces en god ide? 

class rendrer
{
public:

	rendrer(sg::noder::composite *_scene, sg::noder::camera *_camera) : scene(_scene), kamera(_camera)
	{

		
	}
	
	void visit(nodePtr Node, M3DMatrix44f world);
	void draw();

private:
	composite *scene;
	camera *kamera;
public:
	M3DMatrix44f view, projection;

};

#endif