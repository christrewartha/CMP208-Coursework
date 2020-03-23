#pragma once

#include "box2d/box2d.h"
#include "Model.h"


class Joint
{

public:

	enum JointType
	{
		PULLEY,
		PRISMATIC,
		ROPE
	};

	enum Side
	{
		LEFT,
		RIGHT
	};


	Joint();
	~Joint();

	void setUpJoint(std::vector<Model> models, int firstModel, int secondModel, JointType type);
	void setUpJoint(std::vector<Model> models, int firstModel, int secondModel, b2Vec2 axis, JointType type);
	void setUpJoint(std::vector<Model> models, int firstModel, int secondModel, b2Vec2 axis, JointType type, bool enableMotor, float maxForce, float motorSpeed, bool enableLimit, float lowerLimit, float upperLimit);
	void setUpJoint(std::vector<Model> models, int firstModel, int secondModel, float maxLength, Side side, JointType type);
	void setJointType(JointType type);
	b2PulleyJointDef getPulleyJointDef();
	b2PrismaticJointDef getPrismaticJointDef();
	b2RopeJointDef getRopeJointDef();

private:

	JointType jointType;
	//Side side;

	b2PulleyJointDef pulleyJointDef;
	b2PrismaticJointDef prismaticJointDef;
	b2RopeJointDef ropeJointDef;
};

