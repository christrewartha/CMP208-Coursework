#include "Model.h"
#include "maths/quaternion.h"
void Model::init(gef::Scene* sceneAssets, gef::Platform& platform)
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

	finalTransform = scaleMatrix * rotateMatrix * translateMatrix;
	set_transform(finalTransform);

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

	positionOffset = 0.0f;
	shouldUpdate = false;
}

void Model::update()
{
	// deal with moving models in here
	UpdateFromSimulation(body);

	//testUpdate();
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

void Model::UpdateFromSimulation(const b2Body* body)
{
	gef::Matrix44 newRotate;
	newRotate.RotationZ(body->GetAngle());

	gef::Matrix44 newTransform = scaleMatrix * rotateMatrix * newRotate * translateMatrix;

	set_transform(newTransform);
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
	offsetBodyPositions();


	// What the player walks on
	if (name == "Terrain_Path_Flat")
	{
		set_type(PATH);

		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x(), position.y() - 0.5f);
		body = world->CreateBody(&bodyDef);

		shape.SetAsBox(size.x() / 2, size.y() / 2);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		// Create sensor
		shape.SetAsBox((size.x() / 2.f) - 0.4f, size.y() / 4, b2Vec2(0.0f, size.y() / 4), 0); // size.y / 2 maybe for centre
		fixtureDef.isSensor = true;
		body->CreateFixture(&fixtureDef);

		body->SetUserData(this);
	}

	// Edges of platforms
	else if (name == "Terrain_Grass_Flat")
	{
		set_type(WALL);

		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x(), position.y() - positionOffset);
		body = world->CreateBody(&bodyDef);

		shape.SetAsBox(size.x() / 2, size.y() / 2);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		body->SetUserData(this);
	}

	// Platforms that move
	else if (name == "Prop_Block_2x3")
	{
		set_type(PATH);

		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x(), position.y());
		bodyDef.angle = gef::RadToDeg(0.0f);
		body = world->CreateBody(&bodyDef);

		body->SetTransform(body->GetPosition(), body->GetAngle());

		shape.SetAsBox(size.x() / 2, size.y() / 2, b2Vec2(-size.x() / 4, size.y() / 2), 0.0f);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		// Create sensor
		shape.SetAsBox(size.x() / 2.5f, size.y() / 4, b2Vec2(0.0f, size.y() / 4), 0);
		fixtureDef.isSensor = true;
		body->CreateFixture(&fixtureDef);

		body->SetUserData(this);
		shouldUpdate = true;
	}

	// Platforms that move by rope
	else if (name == "Prop_Block_Brick")
	{
		set_type(PATH);

		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x(), position.y() + positionOffset);
		body = world->CreateBody(&bodyDef);

		shape.SetAsBox(size.x() / 2, size.y() / 2);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		body->SetUserData(this);
	}

	// Platforms that move by player collision
	else if (name == "Prop_Crate")
	{
	
			set_type(CRATE);

			bodyDef.type = b2_dynamicBody;
			bodyDef.position = b2Vec2(position.x(), position.y());
			body = world->CreateBody(&bodyDef);

			shape.SetAsBox(size.x() / 2, size.y() / 2, b2Vec2(0.0f, 0.5f), 0.0f); // TODO: do this if shape has to update as moving it doesnt work then, update moves it y

			fixtureDef.density = 0.0f;
			fixtureDef.shape = &shape;
			fixtureDef.friction = 0.25f; // between 0 and 1
			body->CreateFixture(&fixtureDef);

			massData.center = b2Vec2(0.0f, 0.0f);
			massData.mass = 0.f;
			massData.I = 1.0f;
			body->SetMassData(&massData);

			body->SetUserData(this);
			shouldUpdate = true;
		
	}

	else if (name == "Prop_Slide_Top")
	{
		/*b2Vec2 v1(1.0f, 0.0f);
		b2Vec2 v2(21.0f, -20.0f);

		b2EdgeShape edge;
		edge.Set(v1, v2);*/

		set_type(PATH);

		b2Vec2 vs[4];
		vs[0].Set(0.0f, 0.0f);
		vs[1].Set(1.0f, -0.5f);
		vs[2].Set(17.0f, -16.0f);
		vs[3].Set(25.0f, -21.0f);

		b2ChainShape chain;
		chain.CreateChain(vs, 4);

		bodyDef.type = b2_staticBody;
		bodyDef.position = b2Vec2(position.x() - (size.x() / 2), position.y() + size.y());
		body = world->CreateBody(&bodyDef);

		//shape.SetAsBox(size.x() / 2, size.y() / 2);

		fixtureDef.density = 1.0f;
		fixtureDef.shape = &chain;
		fixtureDef.friction = 0.5f; // between 0 and 1
		body->CreateFixture(&fixtureDef);

		body->SetUserData(this);
	}
}

void Model::offsetBodyPositions()
{
	switch (number)
	{

	case 3:
		positionOffset = 2.5f;
		break;

	case 4: case 53: case 56: case 57:
		positionOffset = 1.55f;
		break;

	case 5: case 58:
		positionOffset = 0.95f;
		break;

	case 48: case 51:
		positionOffset = 1.5f;
		break;

	case 60:
		positionOffset = 8.5f;
		break;

	case 63:
		positionOffset = 8.0f;
		break;

	case 62:
		positionOffset = 0.4f;
		break;

	case 65: case 74: case 77: case 78: case 84:
		positionOffset = 1.0f;
		break;

	case 66:
		positionOffset = 2.0f;
		break;

	case 67:
		positionOffset = 0.5f;
		break;

	case 68:
		positionOffset = 3.9f;
		break;

	case 69:
		positionOffset = 0.65f;
		break;

	case 70:
		positionOffset = 4.4f;
		break;

	case 71: case 79:
		positionOffset = 0.9f;
		break;

	case 72:
		positionOffset = 4.5f;
		break;

	case 73:
		positionOffset = 5.9f;
		break;

	case 75:
		positionOffset = 2.4f;
		break;

	case 76:
		positionOffset = 3.35f;
		break;

	case 82:
		positionOffset = 6.0f;
		break;

	case 83:
		positionOffset = 3.5f;
		break;

	default:
		break;
	}
}

bool Model::getShouldUpdate()
{
	return shouldUpdate;
}

void Model::testUpdate()
{
	if (shouldUpdate)
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
	}
}
