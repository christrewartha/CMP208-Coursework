#include "Joint.h"


Joint::Joint()
{
}

Joint::~Joint()
{
}

// Pulley
void Joint::setUpJoint(std::vector<Model> models, int firstModel, int secondModel, JointType type)
{
	setJointType(type);

	b2Body* leftBrick = models[firstModel].getBody();
	b2Body* rightBrick = models[secondModel].getBody();

	b2Vec2 leftBrickAnchor = leftBrick->GetWorldCenter();
	b2Vec2 rightBrickAnchor = rightBrick->GetWorldCenter();

	b2Vec2 leftGroundAnchor(leftBrick->GetPosition().x, 26.25f);
	b2Vec2 rightGroundAnchor(rightBrick->GetPosition().x, 26.25f);

	pulleyJointDef.lengthA = 1.0f;
	pulleyJointDef.lengthB = 1.0f;

	pulleyJointDef.Initialize(leftBrick, rightBrick, leftGroundAnchor, rightGroundAnchor, leftBrickAnchor, rightBrickAnchor, 0.75f);
}

// Prismatic
void Joint::setUpJoint(std::vector<Model> models, int firstModel, int secondModel, b2Vec2 axis, JointType type)
{
	setJointType(type);

	b2Body* firstBrick = models[firstModel].getBody();
	b2Body* secondBrick = models[secondModel].getBody();

	b2Vec2 firstBrickAnchor = firstBrick->GetWorldCenter();

	b2Vec2 worldAxis = axis;

	prismaticJointDef.Initialize(firstBrick, secondBrick, firstBrickAnchor, worldAxis);
}

// Rope
void Joint::setUpJoint(std::vector<Model> models, int firstModel, int secondModel, float maxLength, Side side, JointType type)
{
	setJointType(type);

	b2Body* platform = models[firstModel].getBody();
	b2Body* roof = models[secondModel].getBody();

	float diff = roof->GetPosition().x - platform->GetPosition().x;

	ropeJointDef.bodyA = platform;
	ropeJointDef.bodyB = roof;

	ropeJointDef.maxLength = maxLength;

	if (side == LEFT)
	{
		ropeJointDef.localAnchorA = b2Vec2(-models[firstModel].getScale().x(), 0.5f);
		ropeJointDef.localAnchorB = b2Vec2(abs(diff) - models[firstModel].getScale().x(), 0.0f); // Slightly left of point above body on roof
		
	}

	else if (side == RIGHT)
	{
		ropeJointDef.localAnchorA = b2Vec2(0.0f, 0.5f); // Slightly right of center x of body
		ropeJointDef.localAnchorB = b2Vec2(abs(diff), 0.0f); // Slightly right of point above body on roof
	}
}

void Joint::setJointType(JointType type)
{
	jointType = type;
}

b2PulleyJointDef Joint::getPulleyJointDef()
{
	return pulleyJointDef;
}

b2PrismaticJointDef Joint::getPrismaticJointDef()
{
	return prismaticJointDef;
}

b2RopeJointDef Joint::getRopeJointDef()
{
	return ropeJointDef;
}
