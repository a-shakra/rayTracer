#pragma once

#include "glm.hpp"
#include <string>
#include "util.h"
#include <vector>
#include <fstream>

void handleCamera(std::ifstream &input, Camera &camera);
void handlePlane(std::ifstream &input, Plane &plane);
void handleSphere(std::ifstream &input, std::vector<Sphere> &sphere);
void handleLight(std::ifstream &input, std::vector<Light> &light);
void handleMesh(std::ifstream &input, Mesh &mesh);

bool parse(const std::string &path, Camera & camera, Plane & plane, std::vector<Sphere> & sphere, Mesh & mesh, std::vector<Light> & light)
{

	std::ifstream in(path);
	std::string line;

	while (std::getline(in, line))
	{
		if (line.empty()) {
			break;
		}
		if (line == "camera") {
			handleCamera(in, camera);
		}
		else if (line == "sphere") {
			handleSphere(in, sphere);
		}
		else if (line == "plane") {
			handlePlane(in, plane);
		}
		else if (line == "light") {
			handleLight(in, light); // when pos,amb,dif,spec
		}
		else if (line == "mesh") {
			handleMesh(in, mesh);
		}
		else if (line == "triangle") {}
		else {}
	}

	return true;
}

// Handle code is adapted for multiple tokens. Only one delimiter is used though so the while loop is unnecessary. I obtained this code from stackoverflow. The link to the question has since been taken down
// though so I don't have a URL to link to. Many thanks to my anonymous patron!
void handleCamera(std::ifstream &input, Camera &camera)
{
	std::map<std::string, std::string> values;
	std::string line;
	//For pos:
	std::getline(input, line);
	int pos = 0;
	std::string token;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// fov
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// f
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// a
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	camera = Camera(values);
}

void handlePlane(std::ifstream &input, Plane &plane)
{
	std::map<std::string, std::string> values;
	std::string line;
	//For normal:
	std::getline(input, line);
	int pos = 0;
	std::string token;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For point:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For ambient:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For diffuse:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For specular:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For shiny:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	plane = Plane(values);
}

void handleSphere(std::ifstream &input, std::vector<Sphere> &sphere)
{
	std::map<std::string, std::string> values;
	std::string line;
	//For pos:
	std::getline(input, line);
	int pos = 0;
	std::string token;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For rad:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For ambient:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For diffuse:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For specular:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For shiny:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	sphere.push_back(Sphere(values));
}

void handleLight(std::ifstream &input, std::vector<Light> &light)
{
	std::map<std::string, std::string> values;
	std::string line;
	//For pos:
	std::getline(input, line);
	int pos = 0;
	std::string token;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// fov
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// f
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	// a
	std::getline(input, line);
	pos = 0;
	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	light.push_back(Light(values));
}

void handleMesh(std::ifstream &input, Mesh &mesh) {
	std::map<std::string, std::string> values;
	std::string line;
	//For normal:
	std::getline(input, line);
	int pos = 0;
	std::string token;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For point:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For ambient:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For diffuse:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	//For specular:
	std::getline(input, line);
	pos = 0;

	while ((pos = line.find(":")) != std::string::npos)
	{
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 = size of ":"
	}
	values[token] = line;
	line.clear();
	mesh = Mesh(values);
}