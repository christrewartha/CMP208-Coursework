#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <system/debug_log.h>
#include <vector>
#include <sstream>
#include <map>
#include <cstdio>

#include "Model.h"


class Parser
{

public:

	void init(gef::Scene* sceneAssets, gef::Platform& platform);
	void initModels(gef::Scene* sceneAssets, gef::Platform& platform);
	void readFile(gef::Scene* sceneAssets, gef::Platform& platform);
	std::vector<Model> getModelVector();
	

private:

	void fillVector();
	void setCollisionType();


	//std::ifstream file;
	std::string line;

	std::vector<Model> models;
	Model emptyModel;

	char* fileName = "Objects.txt";

	FILE* file = fopen("Objects.txt", "r");
};

