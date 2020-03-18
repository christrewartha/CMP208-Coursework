#pragma once

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include "primitive_builder.h"
#include "load_texture.h"
#include <input/input_manager.h>
#include <input\keyboard.h>
#include <graphics/sprite.h>
#include <graphics/renderer_3d.h>
#include <box2d/Box2D.h>
#include "game_object.h"
#include <maths/math_utils.h>
//#include "StateMachine.h"


// Forward Declarations
class StateMachine;


class State
{


public:

	State();
	~State();

	virtual void init(gef::Platform& platform) = 0;
	virtual void update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachinePtr) = 0;
	virtual void render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform) = 0;
	virtual void release() = 0;

	void drawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_);
	
	StateMachine* getStateMachinePtr();


protected:

	StateMachine* stateMachinePtr;

};

