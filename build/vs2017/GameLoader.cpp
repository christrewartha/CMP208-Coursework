#include "GameLoader.h"

#include "StateMachine.h"


GameLoader::GameLoader()
{
}

GameLoader::~GameLoader()
{
}

void GameLoader::init(gef::Platform& platform)
{
	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform);

	// load the assets in from the .scn
	const char* scene_asset_filename = "PlayerFemale.scn";
	scene_assets_ = LoadSceneAssets(platform, scene_asset_filename);


	parser.init(scene_assets_, platform);

	models = parser.getModelVector();
}

void GameLoader::update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachine)
{
	if (models.size() > 0)
	{
		stateMachine->changeState(*stateMachine->getGameState());
	}
}

void GameLoader::render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform)
{
}

void GameLoader::release()
{
}

std::vector<Model>* GameLoader::getModels()
{
	return &models;
}

gef::Scene* GameLoader::LoadSceneAssets(gef::Platform& platform, const char* filename)
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

gef::Mesh* GameLoader::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
