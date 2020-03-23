#include "GameState.h"


GameState::GameState() :
	world_(NULL)	
{
	
}

GameState::~GameState()
{
}

void GameState::init(gef::Platform& platform_)
{
	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	

	// load the assets in from the .scn
	const char* scene_asset_filename = "PlayerFemale.scn";
	scene_assets_ = LoadSceneAssets(platform_, scene_asset_filename);

	if (scene_assets_)
	{
		//playerMeshInstance.set_mesh(GetMeshFromSceneAssets(scene_assets_));
		//playerMeshInstance.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	}

	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}

	parser.init(scene_assets_, platform_);

	models = parser.getModelVector();

	SetupLights();

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world_ = new b2World(gravity);

	world_->SetContactListener(&contactManager);

	for (int i = 0; i < models.size(); i++)
	{
		models[i].setCollider(world_);
	}

	setUpJoints();

	player_.init(platform_, world_);

	crateTimerStarted = false;
}

void GameState::update(float frame_time, gef::InputManager* input_manager_, StateMachine* stateMachine)
{
	//const gef::SonyController* controller = input_manager_->controller_input()->GetController(0);

	handleInput(frame_time, input_manager_);

	UpdateSimulation(frame_time);
}

void GameState::render(gef::SpriteRenderer* sprite_renderer_, gef::Font* font_, float fps_, gef::Platform& platform_)
{
	// setup camera

	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(player_.getBody()->GetPosition().x, player_.getBody()->GetPosition().y, CAMERA_OFFSET_Z);
	gef::Vector4 camera_lookat(player_.getBody()->GetPosition().x, player_.getBody()->GetPosition().y, 0.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);


	// draw 3d geometry
	renderer_3d_->Begin();

	for (int i = 0; i < models.size(); i++)
	{
		models[i].render(*renderer_3d_);
	}

	player_.render(renderer_3d_);

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);

	drawHUD(sprite_renderer_, font_, fps_);

	font_->RenderText(sprite_renderer_, gef::Vector4(0, 0, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Foot Contacts: %i", player_.getFootContacts());

	font_->RenderText(sprite_renderer_, gef::Vector4(0, 15, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Impulse: %f", player_.getImpulse());

	font_->RenderText(sprite_renderer_, gef::Vector4(400, 0, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Timer:  %.3f", crateTimer.GetMilliseconds() / 1000);

	sprite_renderer_->End();
}

void GameState::release()
{
	// destroying the physics world also destroys all the objects within it
	delete world_;
	world_ = NULL;

	delete ground_mesh_;
	ground_mesh_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

void GameState::setUpJoints()
{
	// Pulley joint for bricks

	b2Body* body1 = models[48].getBody();
	b2Body* body2 = models[51].getBody();

	b2Vec2 anchor1 = body1->GetWorldCenter();
	b2Vec2 anchor2 = body2->GetWorldCenter();

	b2Vec2 groundAnchor1(body1->GetPosition().x, 26.25f);
	b2Vec2 groundAnchor2(body2->GetPosition().x, 26.25f);

	b2PulleyJointDef pulleyJointDef;
	pulleyJointDef.lengthA = 1;
	pulleyJointDef.lengthB = 1;
	pulleyJointDef.Initialize(body1, body2, groundAnchor1, groundAnchor2, anchor1, anchor2, 0.75f);

	world_->CreateJoint(&pulleyJointDef);


	// Prismatic joint for left brick

	b2PrismaticJointDef prismaticJointDef;
	b2Vec2 worldAxis(0.0f, -1.0f);
	prismaticJointDef.Initialize(body1, body2, anchor1, worldAxis);

	world_->CreateJoint(&prismaticJointDef);


	// Prismatic joint for lift

	body1 = models[56].getBody();
	body2 = models[82].getBody();

	anchor1 = body1->GetWorldCenter();
	anchor2 = body2->GetWorldCenter();

	
	b2Vec2 liftAxis(0.0f, 1.0f); 
	
	liftPrismaticJointDef.enableMotor = true;
	liftPrismaticJointDef.maxMotorForce = 1000.0f;
	liftPrismaticJointDef.motorSpeed = 10.f;

	liftPrismaticJointDef.enableLimit = true;
	liftPrismaticJointDef.lowerTranslation = -5.0f;
	liftPrismaticJointDef.upperTranslation = 8.0f;

	liftPrismaticJointDef.Initialize(body1, body2, anchor2, liftAxis);

	liftPrismaticJoint = (b2PrismaticJoint*) world_->CreateJoint(&liftPrismaticJointDef);




	// Rope joint

	body1 = models[57].getBody();
	body2 = models[62].getBody();

	anchor1 = body1->GetWorldCenter();
	anchor2 = body2->GetWorldCenter();

	b2RopeJointDef ropeJointDef;

	ropeJointDef.bodyA = body1; // Platform
	ropeJointDef.bodyB = body2; // Roof

	float diff = body2->GetPosition().x - body1->GetPosition().x;

	ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.0f);
	ropeJointDef.localAnchorB = b2Vec2(diff, 0.0f);
	ropeJointDef.maxLength = 10.0f;
	
	world_->CreateJoint(&ropeJointDef);

	ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.0f);
	ropeJointDef.localAnchorB = b2Vec2(diff + 1.0f, 0.0f);
	world_->CreateJoint(&ropeJointDef);

}

void GameState::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void GameState::UpdateSimulation(float frame_time)
{
	// update physics world
	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_->Step(timeStep, velocityIterations, positionIterations);

	if (!contactManager.getDestroyList().empty())
	{
		destroyList = contactManager.getDestroyList();

		if (!crateTimerStarted)
		{
			crateTimer.Reset();
			crateTimerStarted = true;
		}

		if (crateTimer.GetMilliseconds() / 1000 > 3.0f)
		{

			for (auto it = destroyList.begin(); it != destroyList.end();)
			{
				world_->DestroyBody(*it);
				destroyList.erase(it++);

				if (destroyList.empty())
				{
					contactManager.deleteDestroyList();
					break;
				}
			}

			crateTimerStarted = false;
		}
	}

	// update object visuals from simulation data
	player_.update();

	for (int i = 0; i < models.size(); i++)
	{
		if (models[i].getShouldUpdate())
		{
			models[i].update();
		}
	}

	if (liftTimer.GetMilliseconds() / 1000 > 3.0f)
	{
		liftPrismaticJoint->SetMotorSpeed(liftPrismaticJoint->GetMotorSpeed() * -1);
		liftTimer.Reset();
	}



	// don't have to update the ground visuals as it is static

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world_->GetContactList();
	// get contact count
	int contact_count = world_->GetContactCount();

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;

			GameObject* gameObjectA = NULL;
			GameObject* gameObjectB = NULL;

			gameObjectA = (GameObject*)bodyA->GetUserData();
			gameObjectB = (GameObject*)bodyB->GetUserData();
			

			if (gameObjectA)
			{
				if (gameObjectA->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}

			if (gameObjectB)
			{
				if (gameObjectB->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}

			/*if (player)
			{
				player->DecrementHealth();
			}*/
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

void GameState::handleInput(float frame_time, gef::InputManager* input_manager_)
{
	player_.handleInput(frame_time, input_manager_);
	// do other input here eg pausing
}

gef::Scene* GameState::LoadSceneAssets(gef::Platform& platform, const char* filename)
{
	return nullptr;
}

gef::Mesh* GameState::GetMeshFromSceneAssets(gef::Scene* scene)
{
	return nullptr;
}
