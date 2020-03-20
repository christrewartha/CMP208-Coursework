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


//#include "MenuState.h"
//#include "GameState.h"



//FD
class MenuState;
class GameState;
class GameLoader;
class State;

class StateMachine
{
	
public:

	StateMachine();
	~StateMachine();

	void init(gef::Platform& platform);
	void update(float frame_time, gef::InputManager* input_manager_);
	void render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform);
	void release();

	void changeState(State &state);

	StateMachine* getStateMachinePtr();

	State* getMenuState();
	State* getGameState();
	State* getGameLoader();


private:

	MenuState* menuState;
	GameState* gameState;
	GameLoader* gameLoader;

	State* currentState;

	StateMachine* stateMachinePtr = this;
};

