#pragma once

#include "State.h"

#include "primitive_builder.h"
#include "load_texture.h"
#include <input/input_manager.h>
#include <input\keyboard.h>
#include <graphics/sprite.h>


class MenuState : public State
{

public:

	MenuState();
	~MenuState();

	void init(gef::Platform& platform) override;
	void update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachinePtr) override;
	void render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform) override;
	void release() override;

	bool getEnterPressed(); //TODO: look for better way to do this - maybe a checkstatechange/statechange function

private:

	gef::Texture* button_icon_;
};

