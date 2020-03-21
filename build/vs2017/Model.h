#pragma once


#include <string>

//#include <maths/vector2.h>
#include "graphics/scene.h"
//#include <graphics/mesh_instance.h>
#include <maths\matrix44.h>
#include <graphics/renderer_3d.h>
#include "primitive_builder.h"
#include <system/debug_log.h>
#include <maths/math_utils.h>
#include <box2d\box2d.h>

#include "game_object.h"



// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Renderer3D;
}



class Model : public GameObject
{

public:

	void init(gef::Scene* sceneAssets, gef::Platform& platform);
	void update();
	void render(gef::Renderer3D& renderer3D);
	gef::Scene* loadAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromAssets(gef::Scene* scene);

	void setNumber(int n);
	void setName(std::string n);
	gef::Vector4 getPosition();
	void setPosition(gef::Vector4 p);
	void setRotation(gef::Vector4 r);
	void setScale(gef::Vector4 s);
	void setSize(gef::Vector4 s);
	std::string getName();

	void setCollider(b2World* world);
	void offsetBodyPositions();

private:


	int number;
	std::string name;
	
	gef::Vector4 position;
	gef::Vector4 rotation;
	gef::Vector4 scale;

	gef::Matrix44 translateMatrix;
	gef::Vector4 translateVect;
	gef::Matrix44 rotateMatrix;
	gef::Vector4 rotateVect;

	gef::Matrix44 rotateMatrixX;
	gef::Matrix44 rotateMatrixY;
	gef::Matrix44 rotateMatrixZ;

	gef::Vector4 scaleVect;
	gef::Matrix44 scaleMatrix;

	gef::Vector4 size;

	std::string fileName;

	b2Body* body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;
	b2MassData massData;

	float positionOffset;
};