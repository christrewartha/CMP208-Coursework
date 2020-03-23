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

	//setUpJoints();

	player_.init(platform_, world_);

	crateTimerStarted = false;




	//setUpJoints();

	// joint stuff

	pulleyBrickJointDef.setUpJoint(models, 48, 51, Joint::PULLEY);
	pulleyBrickJoint = (b2PulleyJoint*)world_->CreateJoint(&pulleyBrickJointDef.getPulleyJointDef());

	prismaticBrickJointDef.setUpJoint(models, 48, 51, b2Vec2(0.0f, -1.0f), Joint::PRISMATIC);
	prismaticBrickJoint = (b2PrismaticJoint*)world_->CreateJoint(&prismaticBrickJointDef.getPrismaticJointDef());

	prismaticLiftJointDef.setUpJoint(models, 82, 56, b2Vec2(0.0f, 1.0f), Joint::PRISMATIC, true, 1000.0f, 10.0f, true, -8.0f, 8.0f);
	prismaticLiftJoint = (b2PrismaticJoint*)world_->CreateJoint(&prismaticLiftJointDef.getPrismaticJointDef());

	ropePlatformOneLeftJointDef.setUpJoint(models, 57, 62, 11.5f, Joint::LEFT, Joint::ROPE);
	ropePlatformOneLeftJoint = (b2RopeJoint*)world_->CreateJoint(&ropePlatformOneLeftJointDef.getRopeJointDef());

	ropePlatformOneRightJointDef.setUpJoint(models, 57, 62, 11.5f, Joint::RIGHT, Joint::ROPE);
	ropePlatformOneRightJoint = (b2RopeJoint*)world_->CreateJoint(&ropePlatformOneRightJointDef.getRopeJointDef());

	ropePlatformTwoLeftJointDef.setUpJoint(models, 58, 62, 7.0f, Joint::LEFT, Joint::ROPE);
	ropePlatformTwoLeftJoint = (b2RopeJoint*)world_->CreateJoint(&ropePlatformTwoLeftJointDef.getRopeJointDef());

	ropePlatformTwoRightJointDef.setUpJoint(models, 58, 62, 7.0f, Joint::RIGHT, Joint::ROPE);	
	ropePlatformTwoRightJoint = (b2RopeJoint*)world_->CreateJoint(&ropePlatformTwoRightJointDef.getRopeJointDef());	
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
	//// Pulley joint for bricks

	//b2Vec2 groundAnchor1(body1->GetPosition().x, 26.25f);
	//b2Vec2 groundAnchor2(body2->GetPosition().x, 26.25f);

	//b2PulleyJointDef pulleyJointDef;
	//pulleyJointDef.lengthA = 1;
	//pulleyJointDef.lengthB = 1;
	//pulleyJointDef.Initialize(body1, body2, groundAnchor1, groundAnchor2, anchor1, anchor2, 0.75f);

	//world_->CreateJoint(&pulleyJointDef);



	//// Prismatic joint for left brick

	//b2PrismaticJointDef prismaticJointDef;
	//b2Vec2 worldAxis(0.0f, -1.0f);
	//prismaticJointDef.Initialize(body1, body2, anchor1, worldAxis);

	//world_->CreateJoint(&prismaticJointDef);


	// Prismatic joint for lift

	// switch these around pls
	b2Body* body1 = models[56].getBody();
	b2Body* body2 = models[82].getBody();

	b2Vec2 anchor1 = body1->GetWorldCenter();
	b2Vec2 anchor2 = body2->GetWorldCenter();

	
	b2Vec2 liftAxis(0.0f, 1.0f); 
	
	b2PrismaticJointDef liftPrismaticJointDef;

	liftPrismaticJointDef.enableMotor = true;
	liftPrismaticJointDef.maxMotorForce = 1000.0f;
	liftPrismaticJointDef.motorSpeed = 10.f;

	liftPrismaticJointDef.enableLimit = true;
	liftPrismaticJointDef.lowerTranslation = -5.0f;
	liftPrismaticJointDef.upperTranslation = 8.0f;

	liftPrismaticJointDef.Initialize(body1, body2, anchor2, liftAxis);

	b2PrismaticJoint* liftPrismaticJoint;
	liftPrismaticJoint = (b2PrismaticJoint*) world_->CreateJoint(&liftPrismaticJointDef);




	//// Rope joint

	//b2Body* body1 = models[57].getBody(); // Platform 1
	//b2Body* body2 = models[62].getBody(); // Roof

	//b2RopeJointDef ropeJointDef;

	//ropeJointDef.bodyA = body1; // Platform
	//ropeJointDef.bodyB = body2; // Roof

	//float diff = body2->GetPosition().x - body1->GetPosition().x; // Difference from center of roof to body

	//ropeJointDef.localAnchorA = b2Vec2(-3.0f, 0.5f); // Slightly left of center x of body
	//ropeJointDef.localAnchorB =  b2Vec2(abs(diff) - 3.0f, 0.0f); // Slightly left of point above body on roof
	//ropeJointDef.maxLength = 11.5f;

	//b2RopeJoint* ropeJointA = (b2RopeJoint*) world_->CreateJoint(&ropeJointDef);

	//ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.5f); // Slightly right of center x of body
	//ropeJointDef.localAnchorB = b2Vec2(abs(diff), 0.0f); // Slightly right of point above body on roof
	//b2RopeJoint* ropeJointB = (b2RopeJoint*) world_->CreateJoint(&ropeJointDef);


	//b2Body* body3 = models[58].getBody();

	//ropeJointDef.bodyA = body3; // Platform 2

	//diff = body2->GetPosition().x - body3->GetPosition().x;

	//// left rope
	//ropeJointDef.localAnchorA = b2Vec2(-2.0f, 0.5f);
	//ropeJointDef.localAnchorB = b2Vec2(abs(diff) - 2.0f, 0.0f);
	//ropeJointDef.maxLength = 7.0f;

	//ropeJointC = (b2RopeJoint*)world_->CreateJoint(&ropeJointDef);

	//// right rope
	//ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.5f); // Slightly right of center x of body
	//ropeJointDef.localAnchorB = b2Vec2(abs(diff), 0.0f); // Slightly right of point above body on roof
	//ropeJointD = (b2RopeJoint*)world_->CreateJoint(&ropeJointDef);

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
		prismaticLiftJoint->SetMotorSpeed(prismaticLiftJoint->GetMotorSpeed() * -1);
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
