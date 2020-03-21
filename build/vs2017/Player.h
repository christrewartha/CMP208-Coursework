#pragma once

#include "primitive_builder.h"
#include "game_object.h"
#include <graphics/renderer_3d.h>
#include <input/input_manager.h>
#include <input\keyboard.h>
#include "StateMachine.h"


class Player : public GameObject
{

public:

	Player();

	void init(gef::Platform& platform_, b2World* world_);
	void update();
	void handleInput(float frame_time, gef::InputManager* input_manager_);
	void render(gef::Renderer3D* renderer_3d_);
	void release();

	b2Body* getBody();


private:

	PrimitiveBuilder* primitive_builder_;

	b2Body* player_body_;

	bool colliding;
};

