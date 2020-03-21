#include "Player.h"


Player::Player()  :
	player_body_(NULL)
{
	set_type(PLAYER);
}


void Player::init(gef::Platform& platform_, b2World* world_)
{
	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);


	// setup the mesh for the player
	//player_.set_mesh(primitive_builder_->GetDefaultCubeMesh());
	set_mesh(primitive_builder_->GetDefaultSphereMesh());

	// create a physics body for the player
	b2BodyDef player_body_def;
	player_body_def.type = b2_dynamicBody;
	player_body_def.position = b2Vec2(32.0f, 18.0f);

	player_body_ = world_->CreateBody(&player_body_def);

	// create the shape for the player
	//b2PolygonShape player_shape;
	//player_shape.SetAsBox(0.5f, 0.5f);

	b2CircleShape player_shape;
	player_shape.m_radius = 0.5f;

	// create the fixture
	b2FixtureDef player_fixture_def;
	player_fixture_def.shape = &player_shape;
	player_fixture_def.density = 1.0f;
	// create the fixture on the rigid body
	player_body_->CreateFixture(&player_fixture_def);

	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -0.5f), 0.0f);

	// Create the sensor fixture
	b2FixtureDef playerSensorFixtureDef;
	playerSensorFixtureDef.shape = &sensorShape;
	playerSensorFixtureDef.isSensor = true;
	player_body_->CreateFixture(&playerSensorFixtureDef);

	b2MassData playerMassData;
	playerMassData.center = b2Vec2(0.0f, 0.0f);
	//playerMassData.mass = 60.f;
	playerMassData.mass = 40.f;
	playerMassData.I = 1.0f;
	player_body_->SetMassData(&playerMassData);

	// update visuals from simulation data
	UpdateFromSimulation(player_body_);

	// create a connection between the rigid body and GameObject
	player_body_->SetUserData(this);

	colliding = false;
	numberOfContacts = 0;
	footContacts = 0;
}

void Player::update()
{
	UpdateFromSimulation(player_body_);
}

void Player::handleInput(float frame_time, gef::InputManager* input_manager_)
{
	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_W) && footContacts > 0)
	{
		player_body_->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 20.0f), true);
	}

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_A))
	{
		player_body_->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}

	if (input_manager_->keyboard()->IsKeyDown(gef::Keyboard::KC_D))
	{
		player_body_->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
}

void Player::render(gef::Renderer3D* renderer_3d_)
{
	// draw player

	if (numberOfContacts > 0)
	{
		renderer_3d_->set_override_material(&primitive_builder_->red_material());
	}

	else
	{
		renderer_3d_->set_override_material(&primitive_builder_->blue_material());
	}

	renderer_3d_->DrawMesh(*this);
	renderer_3d_->set_override_material(NULL);
}

void Player::release()
{
	delete primitive_builder_;
	primitive_builder_ = NULL;
}

b2Body* Player::getBody()
{
	return player_body_;
}

void Player::startContact()
{
	numberOfContacts++;
}

void Player::endContact()
{
	numberOfContacts--;
}

int Player::getFootContacts()
{
	return footContacts;
}

void Player::setFootContacts(int fc)
{
	footContacts = fc;
}
