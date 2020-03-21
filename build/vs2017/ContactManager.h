#pragma once

#include <box2d/Box2D.h>
#include "Player.h"


class ContactManager : public b2ContactListener
{

public:

	ContactManager();
	~ContactManager();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
};

