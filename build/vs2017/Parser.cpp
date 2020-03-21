#include "Parser.h"

float num;
std::string txt;
gef::Vector4 vect;
char text[256];


void Parser::init(gef::Scene* sceneAssets, gef::Platform& platform)
{
	readFile(sceneAssets, platform);
}

void Parser::initModels(gef::Scene* sceneAssets, gef::Platform& platform)
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i].init(sceneAssets, platform);
	}
}


void Parser::readFile(gef::Scene* sceneAssets, gef::Platform& platform)
{
	if (file == NULL)
	{
		std::cout << "Unable to open file." << std::endl;
	}

	else
	{
		fillVector();
		initModels(sceneAssets, platform);
		std::cout << "Test"; //TODO: delete
	}
}

std::vector<Model> Parser::getModelVector()
{
	return models;
}

void Parser::fillVector()
{
	/*while (std::getline(file, line))
	{
		models.push_back(emptyModel);

		if (line.compare("ObjectNumber"))
		{

		}
	}*/

	//models.push_back(emptyModel);

	while (true)
	{
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
		{
			break;
		}

		else
		{
			if (strcmp(lineHeader, "-") == 0)
			{
				models.push_back(emptyModel);
			}

			else if (strcmp(lineHeader, "ObjectNumber:") == 0)
			{
				fscanf(file, "%f", &num);

				models[models.size() - 1].setNumber(num);
			}

			else if (strcmp(lineHeader, "ObjectName:") == 0)
			{
				fscanf(file, "%s", &text);

				models[models.size() - 1].setName(text);
			}

			else if (strcmp(lineHeader, "PositionX:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_x(num);
			}

			else if (strcmp(lineHeader, "PositionY:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_y(num);
			}

			else if (strcmp(lineHeader, "PositionZ:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_z(-num);

				models[models.size() - 1].setPosition(vect);
			}

			else if (strcmp(lineHeader, "RotationX:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_x(num);
			}

			else if (strcmp(lineHeader, "RotationY:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_y(num);
			}

			else if (strcmp(lineHeader, "RotationZ:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_z(num);

				models[models.size() - 1].setRotation(vect);
			}
			else if (strcmp(lineHeader, "RotationW:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_w(num);

				models[models.size() - 1].setRotation(vect);
			}

			if (strcmp(lineHeader, "ScaleX:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_x(num);
			}

			else if (strcmp(lineHeader, "ScaleY:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_y(num);
			}

			else if (strcmp(lineHeader, "ScaleZ:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_z(num);

				models[models.size() - 1].setScale(vect);
			}

			if (strcmp(lineHeader, "SizeX:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_x(num);
			}

			else if (strcmp(lineHeader, "SizeY:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_y(num);
			}

			else if (strcmp(lineHeader, "SizeZ:") == 0)
			{
				fscanf(file, "%f", &num);

				vect.set_z(num);

				models[models.size() - 1].setSize(vect);
			}


		}
	}
}

void Parser::setCollisionType()
{
}
