#pragma once

#include "glm.hpp"
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include "objloader.h"

class Camera
{
public:
	glm::vec3 position;
	float fov;
	float focalLength;
	float aspectRatio;
	Camera() {};
	Camera(std::map<std::string, std::string> const & values)
	{
		for (auto&[key, value] : values) //Oliver fault
		{
			std::istringstream iss(value);
			if (key == "pos")
			{
				iss >> position[0] >> position[1] >> position[2];
			}
			else if (key == "fov")
			{
				iss >> fov;
			}
			else if (key == "f") { iss >> focalLength; }
			else {
				iss >> aspectRatio;
			}
		}
	}
};


class Light
{
public:
	glm::vec3 position;
	glm::vec3 ambient; // Variable that is specific to one type of scene file
	glm::vec3 diffuse; // Variable that is specific to one type of scene file
	glm::vec3 specular; // Variable that is specific to one type of scene file

	glm::vec3 color; // variable that is specific to another type of scene file -- different!

	Light() {};
	Light(std::map<std::string, std::string> const & values) {
		for (auto&[key, value] : values) //if it no work, remove &
		{
			std::istringstream iss(value);
			if (key == "pos")
			{
				iss >> position[0] >> position[1] >> position[2];
			}
			else if (key == "amb") { iss >> ambient[0] >> ambient[1] >> ambient[2]; }
			else if (key == "dif") { iss >> diffuse[0] >> diffuse[1] >> diffuse[2]; }
			else { iss >> specular[0] >> specular[1] >> specular[2]; }
		}
	}	
	
};


class Ray
{
public:
	glm::vec3 position;
	glm::vec3 direction;

	Ray(glm::vec3 position, glm::vec3 direction)
	{
		this->position = position;
		this->direction = glm::normalize(direction);
	}
};

/*
All objects in scene, besides camera and light, are children of the base class, Shape.  This made it a lot easier to pass different objects as one. Their respective attributes are accessed through
shared ptrs initialized in the main.cpp file.
*/
class Shape
{
public:
	std::string objectName;
	Shape() {};

	std::string get_name()
	{
		std::cout << "You should never see this";
	}

	virtual bool intersect(Ray a_ray, glm::vec3 &point_hit, glm::vec3 &normal_hit) { return false; };
	virtual glm::vec3 get_color()
	{
		return glm::vec3(0, 0, 0);
	}

	virtual glm::vec3 get_amb()
	{
		return glm::vec3(0, 0, 0);
	}

	virtual glm::vec3 get_dif()
	{
		return glm::vec3(0, 0, 0);
	}

	virtual glm::vec3 get_spec()
	{
		return glm::vec3(0, 0, 0);
	}

	virtual float get_shi()
	{
		return 0;
	}
};

class Plane : public Shape
{
public:
	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	Plane() {};
	Plane(std::map<std::string, std::string> const & values) {
		this->objectName = "plane";
		for (auto&[key, value] : values) //if it no work, remove &
		{
			std::istringstream iss(value);
			if (key == "nor")
			{
				iss >> normal[0] >> normal[1] >> normal[2];
			}
			else if (key == "pos")
			{
				iss >> point[0] >> point[1] >> point[2];
			}
			else if (key == "amb") { iss >> ambient[0] >> ambient[1] >> ambient[2]; }
			else if (key == "dif") { iss >> diffuse[0] >> diffuse[1] >> diffuse[2]; }
			else if (key == "spe") { iss >> specular[0] >> specular[1] >> specular[2]; }
			else {
				iss >> shiny;
			}
		}
	}

	std::string get_name()
	{
		return objectName;
	}
	
	bool intersect(Ray a_ray, glm::vec3 &point_hit, glm::vec3 &normal_hit) override
	{ //sketchy function -- produces opposite of what i want
		glm::vec3 ray_position = a_ray.position;
		glm::vec3 direction = a_ray.direction;
		glm::vec3 normal = glm::normalize(this->normal);
		glm::vec3 point_on_plane = this->point;

		/*
		float d = dot(normal, point_on_plane);
		float denominator = dot(direction, normal);
		*/
		float denominator = glm::dot(direction, normal);
		float any_small_number = 0.0000001;
		if (abs(denominator) > any_small_number)
		{
			/*
			float numerator = -(dot(ray_position, normal) + d);
			float parameter_t = numerator / denominator;
			*/
			glm::vec3 difference = point_on_plane - ray_position;
			float parameter_t = glm::dot(difference, normal) / denominator;
			if (parameter_t >= 0)
			{
				point_hit = a_ray.position + a_ray.direction * parameter_t;
				//if (point_hit[2] > 0)
				//{ // it means the intersection occured was infront of camera positon -- must be a better way to do this -- ask prof
				//	return false;
				//}
				normal_hit = this->normal; //does normal_hit need to be normalized? Or I just normalize after subtracting from the light position for more calc?
				normal_hit = glm::normalize(normal_hit);
				return true;
			}
			else
			{
				point_hit = glm::vec3(0, 0, 0);
				normal_hit = glm::vec3(0, 0, 0);
				return false;
			}
		}
		else
		{
			point_hit = glm::vec3(0, 0, 0);
			normal_hit = glm::vec3(0, 0, 0);
			return false;
		}
	}


	virtual glm::vec3 get_color() override
	{
		glm::vec3 color = this->ambient + this->diffuse + this->specular;
		return color;
	}

	virtual glm::vec3 get_amb() override
	{
		return this->ambient;
	}

	virtual glm::vec3 get_dif() override
	{
		return this->diffuse;
	}

	virtual glm::vec3 get_spec() override
	{
		return this->specular;
	}

	virtual float get_shi() override
	{
		return this->shiny;
	}
};

class Sphere: public Shape
{
public:
	glm::vec3 position;
	float radius;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiny;
	Sphere() {};
	Sphere(std::map<std::string, std::string> const & values) {
		this->objectName = "sphere";
		for (auto&[key, value] : values) //if it no work, remove &
		{
			std::istringstream iss(value);
			if (key == "pos")
			{
				iss >> position[0] >> position[1] >> position[2];
			}
			else if (key == "rad")
			{
				iss >> radius;
			}
			else if (key == "amb") { iss >> ambient[0] >> ambient[1] >> ambient[2]; }
			else if (key == "dif") { iss >> diffuse[0] >> diffuse[1] >> diffuse[2]; }
			else if (key == "spe") { iss >> specular[0] >> specular[1] >> specular[2]; }
			else {
				iss >> shiny;
			}
		}
	}

	std::string get_name()
	{
		return objectName;
	}

	bool intersect(Ray a_ray, glm::vec3 &point_hit, glm::vec3 &normal_hit) override
	{
		glm::vec3 direction = a_ray.direction;
		float radius = this->radius;

		float ray_x_minus_sphere_x = a_ray.position[0] - this->position[0];
		float ray_y_minus_sphere_y = a_ray.position[1] - this->position[1];
		float ray_z_minus_sphere_z = a_ray.position[2] - this->position[2];

		float a = glm::pow(direction[0], 2) + glm::pow(direction[1], 2) + glm::pow(direction[2], 2); // should be equal to one if properly normalized
		float b = 2 * (direction[0] * ray_x_minus_sphere_x + direction[1] * ray_y_minus_sphere_y + direction[2] * ray_z_minus_sphere_z);
		float c = glm::pow(ray_x_minus_sphere_x, 2) + glm::pow(ray_y_minus_sphere_y, 2) + glm::pow(ray_z_minus_sphere_z, 2) - glm::pow(radius, 2);

		float parameter_t = solve_quadratic(a, b, c); // Intersection parameter t found. input in equation of ray to find point of intersection

		if (parameter_t == 0)
		{ //No intersection
			point_hit = glm::vec3(0, 0, 0);
			normal_hit = glm::vec3(0, 0, 0);
			return false;
		}
		point_hit = a_ray.position + a_ray.direction * parameter_t;
		normal_hit = (glm::vec3(point_hit[0] - this->position[0], point_hit[1] - this->position[1], point_hit[2] - this->position[2]))/ this->radius; // added radius. wasn't there before
		normal_hit = glm::normalize(normal_hit);
		return true;
		//does normal_hit need to be normalized?
	}

	float solve_quadratic(float a, float b, float c)
	{
		float discriminant = glm::pow(b, 2) - 4 * a * c;
		if (discriminant < 0)
		{ // Imaginary number, no intersection
			return 0;
		}
		else if (discriminant == 0)
		{ // Only one intersection
			float t_0 = (-b + glm::pow(discriminant, 0.5)) / (2 * a);
			return t_0;
		}
		else
		{ // Two intersections, return the min
			float t_0 = (-b + glm::pow(discriminant, 0.5)) / (2 * a);
			float t_1 = (-b - glm::pow(discriminant, 0.5)) / (2 * a);
			if (t_0 < 0 && t_1 < 0)
			{ // Means the ray intersects the sphere at two points, both behind the ray origin
				return 0;
			}
			else
			{
				if (t_0 > 0 && t_1 > 0)
				{
					return glm::min(t_0, t_1);
				}
				else if (t_0 > 0 && t_1 < 0)
				{
					return t_0;
				}
				else
				{ // t_1 > 0 and t_0 < 0
					return t_1;
				}
			}
		}
	};

	virtual glm::vec3 get_color() override
	{
		glm::vec3 color = this->ambient + this->diffuse + this->specular;
		return color;
	}

	virtual glm::vec3 get_amb() override
	{
		return this->ambient;
	}

	virtual glm::vec3 get_dif() override
	{
		return this->diffuse;
	}

	virtual glm::vec3 get_spec() override
	{
		return this->specular;
	}

	virtual float get_shi() override
	{
		return this->shiny;
	}

};

class Mesh : public Shape
{
public:
	std::string fileName;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::vector<glm::vec3> vertices; //List of all vertices
	std::vector<glm::vec3> normals; //initialized to satsify loadObj, not needed
	std::vector<glm::vec2> uvs; //initialized to satsify loadObj, not needed
	std::vector<int> vertex_index; //Order of vertices that make triangle
	std::vector<glm::vec3> triangles; // list of triangles arranged in groups of three vertices vector(v0,v1,v2,v0,v1,v2,etc...);
	int number_of_triangles;
	float shiny;
	Mesh() {};
	Mesh(std::map<std::string, std::string> const & values) {
		this->objectName = "mesh";
		for (auto&[key, value] : values) //if it no work, remove &
		{
			std::istringstream iss(value);
			if (key == "file")
			{
				iss >> fileName;
			}
			else if (key == "amb") { iss >> ambient[0] >> ambient[1] >> ambient[2]; }
			else if (key == "dif") { iss >> diffuse[0] >> diffuse[1] >> diffuse[2]; }
			else if (key == "spe") { iss >> specular[0] >> specular[1] >> specular[2]; }
			else {
				iss >> shiny;
			}
		}
		std::string file_input;
		file_input = "../" + fileName;
		const char * c = file_input.c_str();
		loadOBJ(c, vertices, normals, uvs, vertex_index);
		set_number_of_triangles();
	}
	std::string get_name()
	{
		return objectName;
	}

	void set_number_of_triangles()
	{
		number_of_triangles = vertex_index.size() / 3;
	}

	bool intersect(Ray a_ray, glm::vec3 &point_hit, glm::vec3 &normal_hit) {
		int j = 0;
		bool intersect = false;
		float min_distance = INFINITY;
		for (int i = 0; i < number_of_triangles; i++)
		{
			glm::vec3 vertex_1 = vertices[j]; //Represent the vertices of a triangle
			glm::vec3 vertex_2 = vertices[j + 1];
			glm::vec3 vertex_3 = vertices[j + 2];
			glm::vec3 test_point_hit;
			glm::vec3 test_normal_hit;
			if (interesect_with_triangle(a_ray, vertex_1, vertex_2, vertex_3, test_point_hit, test_normal_hit))
			{
				float distance = glm::distance(test_point_hit, a_ray.position);
				if (distance < min_distance)
				{
					min_distance = distance;
					point_hit = test_point_hit;
					normal_hit = glm::normalize(test_normal_hit);
					// add something to find triangle index?
					intersect = true;
				}
			}
			j += 3;
		}
		
		return intersect;
	};

	bool interesect_with_triangle(Ray a_ray, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 &point_hit, glm::vec3 &normal_hit)
	{ // consider setting point_hit and normal_hit to (0,0,0) if false
		glm::vec3 vertex_1 = v1;
		glm::vec3 vertex_2 = v2;
		glm::vec3 vertex_3 = v3;

		//Computing Normal
		glm::vec3 V1_V2 = vertex_2 - vertex_1;
		glm::vec3 V1_V3 = vertex_3 - vertex_1;

		glm::vec3 normal = glm::cross(V1_V2, V1_V3); // should i normalize?

		//Find the point of intersection

		float direction_dot_normal = glm::dot(a_ray.direction, normal);
		float any_small_number = 0.0000001;
		if (abs(direction_dot_normal) < any_small_number) // intersection with plane, just have just called that function but its ok
		{
			return false;
		}

		float d = glm::dot(normal, vertex_1);

		//Computing the t parameter
		float numerator = glm::dot(normal, a_ray.position) + d;
		float t = numerator / direction_dot_normal;
		if (t < 0)
		{ // No intersection with plane containing triangle
			return false;
		}
		point_hit = a_ray.position + t * a_ray.direction;

		//starting inside out test to see if above point is in triangle
		glm::vec3 vec_perp_tri_plane;

		//edge 1
		glm::vec3 edge_1 = vertex_2 - vertex_1;
		glm::vec3 test_1 = point_hit - vertex_1;
		vec_perp_tri_plane = glm::cross(edge_1, test_1);
		if (glm::dot(normal, vec_perp_tri_plane) < 0)
		{
			return false;
		}

		//edge 2
		glm::vec3 edge_2 = vertex_3 - vertex_2;
		glm::vec3 test_2 = point_hit - vertex_2;
		vec_perp_tri_plane = glm::cross(edge_2, test_2);
		if (glm::dot(normal, vec_perp_tri_plane) < 0)
		{
			return false;
		}
		//edge 3
		glm::vec3 edge_3 = vertex_1 - vertex_3;
		glm::vec3 test_3 = point_hit - vertex_3;
		vec_perp_tri_plane = glm::cross(edge_3, test_3);
		if (glm::dot(normal, vec_perp_tri_plane) < 0)
		{
			return false;
		}
		normal_hit = glm::normalize(normal);
		return true;
	}

	virtual glm::vec3 get_amb() override
	{
		return this->ambient;
	}

	virtual glm::vec3 get_dif() override
	{
		return this->diffuse;
	}

	virtual glm::vec3 get_spec() override
	{
		return this->specular;
	}

	virtual float get_shi() override
	{
		return this->shiny;
	}

};
