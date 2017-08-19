#ifndef WORLD_HPP
#define WORLD_HPP


//#include "Rendrer\gbuffer.h"
#include "util.h"
//#include "Rendrer\context.h"

#include "bass.h"

#include "sync.h"



#include "Rendrer\base_buffer.h"
#include "Rendrer\gbuffer.h"
#include "Rendrer\aobuffer.h"

#include "SceneGraph\node.h"
#include "SceneGraph\objTransform.h"

#include "SceneGraph\camera.h"
#include "SceneGraph\composite.h"
//#include "SceneGraph\modelNode.h"
#include "ShaderFu\renderPacket.h"
#include "Engine\ShaderItem.h"
//namespace PassItem = boost::property_tree;
#include "tinyxml2.h"

class PassItemnator
{
	std::string RepoName;
	
	//std::string ProductioFolder;
public:
	
	PassItemnator();
	int load(const std::string &filename);
	void save();
	void deploy();
	//std::string GetParsed

private:
	tinyxml2::XMLDocument xml_PassRepo;
	//PassItem::ptree  PassData;
	boost::filesystem::path HumbleXML;
	boost::filesystem::path AssetGlobalLocation;
	boost::filesystem::path AssetProduction;
	boost::filesystem::path ReleaseFolder;
	//NS_ENG::NS_SHADER::ShaderItem *VertexCode, *GeometryCode, *FragmentCode;
	NS_ENG::NS_SHADER::BaseShaderItem *VertexCode, *GeometryCode, *FragmentCode;

	//NS_ENG::NS_SHADER::ShaderItem VertexCode, GeometryCode, FragmentCode;

public:
	std::string Name;
	std::string Description;
	int PassValue;

	//I should consider using GUID
	std::string PacketType;
	std::string PacketName;
	int PacketIndex;

	std::string BufferType;
	std::string BufferName;
	int BufferIndex;

	std::string BufferLocalPassType;
	int BufferLocalIndex;
	int BufferLocalPassValue;



	bool hasVertexShader, hasGeometryShader, hasFragmentShader;
	


};


//this isnt nessescary
typedef boost::shared_ptr< NS_SG::composite >  compoPointer;

typedef boost::shared_ptr< NS_EFF::renderPacket >  sp_RenderPacket;
typedef boost::shared_ptr< base_buffer >  sp_Buffer;
typedef boost::shared_ptr< PassItemnator >  sp_PassItemnator;
//needs boost-fu
//typedef std::vector< NS_EFF::renderPacket* > vec_EffectPackets;
//typedef std::vector< base_buffer* > vec_BufferContainer;
//typedef std::vector< PassItemnator* > vec_PassDefinitions;

//might need to make a special template class.
typedef std::vector< sp_RenderPacket > vec_EffectPackets;
typedef std::vector< sp_Buffer > vec_BufferContainer;
typedef std::vector< sp_PassItemnator > vec_PassDefinitions;


//Not just a lazy singleton.
class DemoMeister //would inheriting from the Composite/Root Node be a good idea?
{

//All Initialisation data, custom or no clutters up the main or where ever.
//For each prod I'm working on I'll create a template in the Production Folder with a 
//simple concrete version of this class in a similarly named c++/h.


//If I want to add custom models, experiment, overload a few classes and generally fuck about then the Base code and Assets don't have
//to be modified.
//My idea is to be able to create demos with only two tabs open in the IDE.

//Further more as this is a singelton its a safe place to add and implement runtime libraries like GUIs, Bass and a place to deal with those damn.
//sync objects


//There will also be a deploy function for release using data gathered and saved with DeployDebug build.
public:

	//remember destructors, carefull with utilizing the constructor before opengl is initialized,
	//the way its setup no it wont.
	
	
	//should be protected or private, but this is only a base class...
	DemoMeister();

	virtual ~DemoMeister() {};

	//all these should be virtual, the Accessors will have one set of global classes and prototyes for the base function
	//If I want to add more in the production Zones Ill just overload it and make sure Ill call the original.

	//These must have error codes.
	void AddPass();


	void AddPass(const std::string &filename);

	

	void AddNode();

	//returns the index which it was placed, if type is not found, it returns -2, if name is not found returns -1.
	//If the Name and Type(better just make it Name) allready exsist, its index is returned.
	//Just remember to catch them.
	//int AddEffect(std::string Type, std::string Name);
	size_t  AddEffect(const std::string& Type, const std::string& Name);


	//Strings for now, these are used for initialization anyway so speed is no issue.
	//things are gonna get confusing pretty quick however. Which Is why I plan to use a prototype pattern 
	//and a quantifiable identifier. 

	
	//This is one of the main reasons Im doing this, I want a material system
	void AddAsset();

	//This is one of the main reasons Im doing this, I want a material system
	void AddMaterial();

	//maybe not virtualize the getters.

	//Use default argument for both of these, error if both are not filled in.
	//The way I'm doing it now you only need one instance of each RenderPacket so type would be enough.
	//However even though I've gone to the trouble making them individual packets can be repurpused depending on the pass
	size_t RetriveEffectID(const std::string &Type, const std::string &Name);
	//http://boost-spirit.com/dl_more/fusion_v2/libs/fusion/doc/html/fusion/container/map.html
	//https://stackoverflow.com/questions/4195100/templates-for-setters-and-getters
	//http://www.boost.org/doc/libs/1_62_0/doc/html/boost_typeindex/getting_started.html


	//I sorely need a more durable and conscice ID, what more it should store a direct access index.
	//http://en.cppreference.com/w/cpp/types/ptrdiff_t
	//Need a solution to this, I should use size_t but I really want my negatives for error.
	//Eh pass by reference.

	//sp_RenderPacket RetriveEffect(const std::string &Type, const std::string &Name);
	sp_RenderPacket RetriveEffect(size_t idx);



	//I allready have static class factory methods for most Assets.
	//But could still be usefull.  Maybe it can maintain the shoddy container.

	//Ah yes, I can use these to implement changes I want in the asset.
	//again this can become stupid complicated why am i doing this!
	//This would work with a model-view-controller
	void RetriveAsset();

	void RetriveMaterial();

	//Mesh o

	//Model

	//Texture

	//Shader

	//EffectGraph

	//I want something to check for duplicates, materials and textures(materials can be different but use the same)
	//at some point this might be what I need the DemoMeister class for, a hackers space so I dont have to create a class for
	//everything like procedural textures.

	//either initialization of passes, packets and loading on models can be done on the fly
	//virtual void PreInit() = 0;
	//virtual void Init() = 0;
	//virtual void AfterInit() = 0;
	//I could set the soundtrack setup here but leave that for later

	//with an id argument perhaps, thinking gnu rocket should sync up here
	//virtual void HackerSpace() = 0;


	virtual void PreInit() = 0;
	virtual void Init() = 0;
	virtual void AfterInit() = 0;
	//I could set the soundtrack setup here but leave that for later

	//with an id argument perhaps, thinking gnu rocket should sync up here
	virtual void HackerSpace() = 0;


//private:
	//containers for textures, assets, packets, buffers,



//Should be private, need Getters and prototypes
public:
	vec_BufferContainer  MasterList_Buffers;

	vec_EffectPackets MasterList_Packets;

	vec_PassDefinitions  MasterList_Passes;
	//int WindowID;
	boost::shared_ptr<NS_SG::composite>  o_loader;
	//boost::weak_ptr<NS_SG::node>  anchor;
	//NS_SG::composite o_loader;
	// void keyDown(int key, int x, int y);
	//
	// void keyRelease(int key, int x, int y);
	//




	unsigned int ResolutionX;
	unsigned int ResolutionY;

	//void AddCue();


};



#endif
