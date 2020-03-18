#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include <input/input_manager.h>
#include <input\keyboard.h>
#include <box2d/Box2D.h>
#include "game_object.h"

//#include "MenuState.h"
//#include "GameState.h"

#include "StateMachine.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager* input_manager_;

	

	// audio variables
	int sfx_id_;
	int sfx_voice_id_;

	float fps_;


	enum StateEnum {INIT, MENU, GAME, END};
	StateEnum state;

	/*MenuState menuState;
	GameState gameState;

	State* currentState;*/

	StateMachine* stateMachine;
};

#endif // _SCENE_APP_H
