#pragma once

#include <box2d/Box2D.h>
#include "Player.h"
#include <set>


class ContactManager : public b2ContactListener
{

public:

	ContactManager();
	~ContactManager();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	std::set<b2Body*> getDestroyList();
	void deleteDestroyList();

private:

	//std::vector<GameObject*> destroyList;
	std::set<b2Body*> destroyList;
};

