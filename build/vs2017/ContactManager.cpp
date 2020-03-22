#include "ContactManager.h"


ContactManager::ContactManager()
{
}

ContactManager::~ContactManager()
{
}


void ContactManager::BeginContact(b2Contact* contact)
{
	// Get colliding fixtures
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	// Get colliding bodies
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	// Set player pointers to null
	Player* playerA = NULL;
	Player* playerB = NULL;

	// Set game object pointers to null
	Player* objectA = NULL;
	Player* objectB = NULL;

	// Store user data
	objectA = (Player*)bodyA->GetUserData();
	objectB = (Player*)bodyB->GetUserData();


	// If object A is the player
	if (objectA->type() == PLAYER)
	{
		if (fixtureA->IsSensor())
		{
			if (fixtureB->IsSensor())
			{
				if (objectB->type() != WALL)
				{
					objectA->setFootContacts(objectA->getFootContacts() + 1);
				}
			}
		}

		if (objectB->type() == CRATE)
		{
			destroyList.insert(bodyB);
		}

		objectA->startContact();
	}

	// If object B is the player
	else if (objectB->type() == PLAYER)
	{
		if (fixtureB->IsSensor())
		{
			if (fixtureA->IsSensor())
			{
				if (objectA->type() != WALL)
				{
					objectB->setFootContacts(objectB->getFootContacts() + 1);
				}
			}
		}

		if (objectA->type() == CRATE)
		{
			destroyList.insert(bodyA);
		}

		objectB->startContact();
	}
	
}

void ContactManager::EndContact(b2Contact* contact)
{
	// Get colliding fixtures
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	// Get colliding bodies
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	// Set player pointers to null
	Player* objectA = NULL;
	Player* objectB = NULL;

	// Check if either object has user data - this will be the player
	objectA = (Player*)bodyA->GetUserData();
	objectB = (Player*)bodyB->GetUserData();

	// If object A is the player
	if (objectA->type() == PLAYER)
	{
		if (fixtureA->IsSensor())
		{
			if (fixtureB->IsSensor())
			{
				if (objectB->type() != WALL)
				{
					objectA->setFootContacts(objectA->getFootContacts() - 1);
				}
			}
		}

		objectA->endContact();
	}

	// If object B is the player
	else if (objectB->type() == PLAYER)
	{
		if (fixtureB->IsSensor())
		{
			if (fixtureA->IsSensor())
			{
				if (objectA->type() != WALL)
				{
					objectB->setFootContacts(objectB->getFootContacts() - 1);
				}
			}
		}

		objectB->endContact();
	}
}

std::set<b2Body*> ContactManager::getDestroyList()
{
	return destroyList;
}

void ContactManager::deleteDestroyList()
{
	// TODO: clean up pointers?

	destroyList.clear();
}
