#include "StateMachine.h"

#include "MenuState.h"
#include "GameState.h"


StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}


void StateMachine::init(gef::Platform& platform)
{
	menuState = new MenuState();
	gameState = new GameState();


	currentState = &*menuState;
	stateMachinePtr = this;

	//currentState->init(platform);

	menuState->init(platform);
	gameState->init(platform);
}

void StateMachine::update(float frame_time, gef::InputManager* input_manager_)
{
	currentState->update(frame_time, input_manager_, stateMachinePtr);
}

void StateMachine::render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform_)
{
	currentState->render(sprite_renderer_, font_, fps_, platform_);
}

void StateMachine::release()
{
	currentState->release();
}

StateMachine* StateMachine::getStateMachinePtr()
{
	return stateMachinePtr;
}

State* StateMachine::getMenuState()
{
	return menuState;
}

State* StateMachine::getGameState()
{
	return gameState;
}

void StateMachine::changeState(State &state)
{
	currentState = &state;
}
