#include "Model.h"
#include "maths/quaternion.h"
void Model::init(PrimitiveBuilder* primitiveBuilder, gef::Scene* sceneAssets, gef::Platform& platform)
{
	translateVect = gef::Vector4(position);
	rotateVect = gef::Vector4(rotation);

	translateMatrix.SetIdentity();
	translateMatrix.SetTranslation(translateVect);

	rotateMatrix.SetIdentity();
	rotateMatrixX.RotationX(gef::DegToRad(rotation.x()));

	if (rotation.y() == 180)
	{
		rotateMatrixY.RotationY(gef::DegToRad(1.0f));
	}

	else if (rotation.y() == 0)
	{
		rotateMatrixY.RotationY(gef::DegToRad(180.0f));
	}

	else
	{
		rotateMatrixY.RotationY(gef::DegToRad(rotation.y()));
	}
	
	rotateMatrixZ.RotationZ(gef::DegToRad(-rotation.z()));
	rotateMatrix = rotateMatrixX * rotateMatrixY * rotateMatrixZ;

	scaleVect = gef::Vector4(scale);
	scaleMatrix.Scale(scaleVect);

	gef::Matrix44 rotate;
	rotate.SetIdentity();
	rotate.RotationY(gef::DegToRad(180));

	gef::Matrix44 finalTrans = scaleMatrix * rotateMatrix * translateMatrix;
	set_transform(finalTrans);

	/*translateVect = gef::Vector4(0.0f, 0.0f, 0.0f);
	translateMatrix.SetIdentity();
	translateMatrix.SetTranslation(translateVect); 
	set_transform(translateMatrix);*/


	// load the assets in from the .scn
	std::string fileName = getName();
	fileName = fileName.append(".scn");
	const char* scene_asset_filename = strdup(fileName.c_str());
	sceneAssets = loadAssets(platform, scene_asset_filename);

	if (sceneAssets)
	{
		set_mesh(GetMeshFromAssets(sceneAssets));
	}

	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}

	body = NULL;
}

void Model::update()
{
	// deal with moving models in here

}

void Model::render(gef::Renderer3D& renderer3D)
{
	renderer3D.DrawMesh(*this);
}

gef::Scene* Model::loadAssets(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();

	if (scene->ReadSceneFromFile(platform, filename))
	{
		// if scene file loads successful
		// create material and mesh resources from the scene data
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
	{
		delete scene;
		scene = NULL;
	}

	return scene;
}

gef::Mesh* Model::GetMeshFromAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}

void Model::setNumber(int n)
{
	number = n;
}

void Model::setName(std::string n)
{
	name = n;
}

gef::Vector4 Model::getPosition()
{
	return position;
}

void Model::setPosition(gef::Vector4 p)
{
	position = p;
}

void Model::setRotation(gef::Vector4 r)
{
	rotation = r;
}

void Model::setScale(gef::Vector4 s)
{
	scale = s;
}

void Model::setSize(gef::Vector4 s)
{
	size = s;
}

std::string Model::getName()
{
	return name;
}

void Model::setCollider(b2World* world)
{
	if (name == "Terrain_Path_Flat")
	{
		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x(), position.y());
		body = world->CreateBody(&bodyDef);

		//shape.SetAsBox(0.5f, 0.1f);
		shape.SetAsBox(size.x() / 2, 0.1);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		massData.center = b2Vec2(0.0f, 0.0f);
		massData.mass = 1.0f;
		massData.I = 1.0f;
		body->SetMassData(&massData);
	}
}
