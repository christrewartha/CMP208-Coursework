#pragma once

#include <vector>
#include "State.h"
#include "Parser.h"


class GameLoader : public State
{

public:

	GameLoader();
	~GameLoader();

	void init(gef::Platform& platform) override;
	void update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachinePtr) override;
	void render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform) override;
	void release() override;

	std::vector<Model>* getModels();


private:

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	Parser parser;
	std::vector<Model> models;

	gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	gef::Scene* scene_assets_;

};

