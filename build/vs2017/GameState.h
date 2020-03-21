#pragma once

#include "State.h"

#include "primitive_builder.h"
#include "load_texture.h"
#include <input/input_manager.h>
#include <input\keyboard.h>
#include <graphics/sprite.h>
#include <graphics/renderer_3d.h>
#include <box2d/Box2D.h>
#include <maths/math_utils.h>
#include <vector>
#include "Parser.h"
#include "Player.h"



class GameState : public State
{

public:

	GameState();
	~GameState();

	void init(gef::Platform& platform) override;
	void update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachinePtr) override;
	void render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform) override;
	void release() override;


private:

	void SetupLights();
	void UpdateSimulation(float frame_time);
	void handleInput(float frame_time, gef::InputManager* input_manager_);

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);


	gef::Renderer3D* renderer_3d_;
	

	// create the physics world
	b2World* world_;

	// player variables
	Player player_;
	

	// ground variables
	const gef::Mesh* ground_mesh_;
	GameObject ground_;
	b2Body* ground_body_;

	gef::Scene* scene_assets_;

	Parser parser;

	std::vector<Model> models;

	const float CAMERA_OFFSET_Y = 10.0f;
	const float CAMERA_OFFSET_Z = 20.0f;
};

