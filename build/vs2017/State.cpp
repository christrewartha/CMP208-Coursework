#include "State.h"

#include "StateMachine.h"


State::State()
{
}

State::~State()
{
}

void State::init(gef::Platform& platform)
{
	stateMachinePtr = getStateMachinePtr();
}

void State::update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachinePtr)
{
}

void State::render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform)
{
}

void State::release()
{
}

void State::drawHUD(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_)
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

StateMachine* State::getStateMachinePtr()
{
	return stateMachinePtr->getStateMachinePtr();
}
