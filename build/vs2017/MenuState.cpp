#include "MenuState.h"

#include "StateMachine.h"



MenuState::MenuState() : button_icon_(NULL)
{
}

MenuState::~MenuState()
{
}

void MenuState::init(gef::Platform& platform)
{
	button_icon_ = CreateTextureFromPNG("playstation-cross-dark-icon.png", platform);
}

void MenuState::update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachine)
{
	//const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_RETURN))
	{
		//enterPressed = true;

		stateMachine->changeState(*stateMachine->getGameState());

		release();
	}
}

void MenuState::render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform_)
{
	sprite_renderer_->Begin();

	// render "PRESS" text
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f - 56.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"PRESS");

	// Render button icon
	gef::Sprite button;
	button.set_texture(button_icon_);
	button.set_position(gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f, -0.99f));
	button.set_height(32.0f);
	button.set_width(32.0f);
	sprite_renderer_->DrawSprite(button);


	// render "TO START" text
	font_->RenderText(
		sprite_renderer_,
		gef::Vector4(platform_.width() * 0.5f, platform_.height() * 0.5f + 32.0f, -0.99f),
		1.0f,
		0xffffffff,
		gef::TJ_CENTRE,
		"TO START");

	drawHUD(sprite_renderer_, font_, fps_);

	sprite_renderer_->End();
}

void MenuState::release()
{
	delete button_icon_;
	button_icon_ = NULL;
}
