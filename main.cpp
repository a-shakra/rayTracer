#pragma once

#include "CImg.h"
#include "glm.hpp"
#include <iostream>
#include "parser.h"
#include <fstream>
#include <cstdlib>

using namespace glm;
using namespace std;

void insert_objects(vector<Sphere> spheres, Plane plane, Mesh mesh, vector<shared_ptr<Shape>> &all_shapes); //Need to include triangle after
void setImageSize(int & width, int & height, Camera camera);
vector<glm::vec3> setPixels(int width, int height, Camera camera, vector<glm::vec3> pixelArray);
glm::vec3 generate_color(Ray a_ray, vector<shared_ptr<Shape>> &all_shapes, vector<Light> &all_lights, Camera camera);
glm::vec3 soft_shadow_illumination(glm::vec3 point_of_intersection, Sphere a_light_sphere);

int main()
{	
	// dimensions of image plane
	int WIDTH;
	int HEIGHT;
	// vector containing pixels in image plane
	vector<glm::vec3> pixels_in_image;
	// vector containing rays
	vector<Ray> vector_of_rays;
	// objects in the scene
	Camera camera;
	Plane plane;
	vector<Sphere> spheres;
	Mesh mesh;
	vector<Light> lights;
	// parsing the scene file
	parse("../sceneMap.txt", camera, plane, spheres, mesh, lights);
	// Inserting all shapes into vector
	vector<shared_ptr<Shape>> shapes; //Objects will be spheres,plane,triangles,mesh
	insert_objects(spheres, plane, mesh, shapes);
	// Determining image size
	setImageSize(WIDTH, HEIGHT, camera); 
	// Determining pixel coordinates
	pixels_in_image = setPixels(WIDTH, HEIGHT, camera, pixels_in_image);
	// Determing the ray through each pixel
	
	cimg_library::CImg<float> image(WIDTH, HEIGHT, 1, 3, 255);
	vector<vec3> pixel_color;
	
	
	/*
	for (glm::vec3 pixel : pixels_in_image) // Non-Aliasing
	{ // pixel starts off containing the 3d position of the pixel
		glm::vec3 direction(pixel[0] - camera.position[0], pixel[1] - camera.position[1], pixel[2] - camera.position[2]);// will be normalized in ray constructor
		Ray pixel_ray(camera.position, direction);
		
		vec3 color = generate_color(pixel_ray, shapes, lights, camera); // generate color and replace 3d position of pixel with color of pixel at that position
		pixel_color.push_back(color);
		
	}
	*/
	
	float scale = tan(glm::radians(camera.fov / 2));

	for (int j = 0; j < HEIGHT; ++j)
	{
		for (int i = 0; i <= WIDTH; ++i)
		{
			// base x and y coordinates of each pixel.
			float x = (2 * (i + 0.5) / (float)WIDTH - 1) * camera.aspectRatio * scale;
			float y = (1 - 2 * (j + 0.5) / (float)HEIGHT) * scale;
			// adjustments needed to space out the four rays that will be produced.
			float x_left = (2 * (i + 0.5 - 0.5) / (float)WIDTH - 1) * camera.aspectRatio * scale;
			float x_right = (2 * (i + 0.5 + 0.5) / (float)WIDTH - 1) * camera.aspectRatio * scale;
			float y_top = (1 - 2 * (j + 0.5 + 0.5) / (float)HEIGHT) * scale; 
			float y_bottom = (1 - 2 * (j + 0.5 - 0.5) / (float)HEIGHT) * scale;

			glm::vec3 pixel_left(x_left, y, -1);
			glm::vec3 pixel_right(x_right, y, -1);
			glm::vec3 pixel_top(x, y_top, -1);
			glm::vec3 pixel_bottom(x, y_bottom, -1);

			// Four rays each at different corners of the square pixel.
			glm::vec3 direction_1(pixel_left[0] - camera.position[0], pixel_left[1] - camera.position[1], pixel_left[2] - camera.position[2]);
			Ray ray_1(camera.position, direction_1); // Ray class automatically normalizes direction

			glm::vec3 direction_2(pixel_right[0] - camera.position[0], pixel_right[1] - camera.position[1], pixel_right[2] - camera.position[2]);
			Ray ray_2(camera.position, direction_2);

			glm::vec3 direction_3(pixel_top[0] - camera.position[0], pixel_top[1] - camera.position[1], pixel_top[2] - camera.position[2]);
			Ray ray_3(camera.position, direction_3);

			glm::vec3 direction_4(pixel_bottom[0] - camera.position[0], pixel_bottom[1] - camera.position[1], pixel_bottom[2] - camera.position[2]);
			Ray ray_4(camera.position, direction_4);
			// Return color produced from four different rays.
			vec3 color_sample_1 = generate_color(ray_1, shapes, lights, camera);
			vec3 color_sample_2 = generate_color(ray_2, shapes, lights, camera);
			vec3 color_sample_3 = generate_color(ray_3, shapes, lights, camera);
			vec3 color_sample_4 = generate_color(ray_4, shapes, lights, camera);
			// Average the color to obtain the final result, which should produce a less jagged and more consistent image.
			vec3 final_color = color_sample_1 + color_sample_2 + color_sample_3 + color_sample_4;
			final_color = final_color / 4.0f;
			pixel_color.push_back(final_color);
		}
	}

	int pixel_count = 0;
	
	for (int v = 0; v < HEIGHT; v++)
	{
		for (int u = 0; u <= WIDTH; u++, pixel_count++)
		{
			image.atXY(u, v, 0) = pixel_color[pixel_count][0] * 255;
			image.atXY(u, v, 1) = pixel_color[pixel_count][1] * 255;
			image.atXY(u, v, 2) = pixel_color[pixel_count][2] * 255;
		}
	}


	image.save("render.bmp");
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed()) { main_disp.wait(); }

	return 0;
}

void insert_objects(vector<Sphere> spheres, Plane plane, Mesh mesh, vector<shared_ptr<Shape>> &all_shapes) 
{

	for (Sphere sphere : spheres)
	{
		all_shapes.push_back(std::make_shared<Sphere>(sphere));
	}
	all_shapes.push_back(std::make_shared<Plane>(plane));
	all_shapes.push_back(std::make_shared<Mesh>(mesh));
	
}

// Function that determins the size of the image in pixels along width and height
void setImageSize(int & width, int & height, Camera camera) 
{
	float fov = camera.fov;
	float f = camera.focalLength; 
	height = 2 * f * glm::tan(glm::radians(fov / 2)); // height and width will be truncated to closest int value
	width = camera.aspectRatio * height; // width kind of gets truncated twice because its dependent on height
};

// method designed to set pixel x,y, and z coordinates. Only used when in the non-aliasing loop in the main function
vector<glm::vec3> setPixels(int width, int height, Camera camera, vector<glm::vec3> pixelArray)
{
	//Specifying boundaries of the x dimension
	float pixelInitialX = -camera.aspectRatio * glm::tan(glm::radians(camera.fov / 2));
	float pixelFinalX = camera.aspectRatio * glm::tan(glm::radians(camera.fov / 2));
	//specifying boundaries of the y dimension
	float pixelInitialY = glm::tan(glm::radians(camera.fov / 2));
	float pixelFinalY = -glm::tan(glm::radians(camera.fov / 2));
	//Z is constant and equal to camera focal lenght
	const float pixelZ = -1.0; // equivalent to camera.focalLength in world space
	//Determing by how much x and y pixel coordinates should be incremented per pixel
	float incrementX = (pixelFinalX - pixelInitialX) / width; // +ve increment
	float incrementY = (pixelFinalY - pixelInitialY) / height;// -ve increment

	for (int v = 0; v < height; v++)
	{
		float pixelX = pixelInitialX;
		for (int u = 0; u <= width; u++) // width gets an extra increment to compensate for double truncation in setImageSize
		{ // This makes sure each corner of image size corresponds to expected values
			glm::vec3 pixel = glm::vec3(pixelX, pixelInitialY, pixelZ); // x , y , z
			pixelX += incrementX;
			pixelArray.push_back(pixel);
		}
		pixelInitialY += incrementY;
	}
	// Testing to see if the corner values are similar enough
	
	glm::vec3 corner1 = pixelArray[0];
	glm::vec3 corner2 = pixelArray[width];
	glm::vec3 corner3 = pixelArray[pixelArray.size() - width];
	glm::vec3 corner4 = pixelArray.back();
	
	return pixelArray;
};


glm::vec3 generate_color(Ray a_ray, vector<shared_ptr<Shape>> &all_shapes, vector<Light> &all_lights, Camera camera)
{
	Shape* object = NULL;
	float min_distance = INFINITY;
	glm::vec3 point_hit, normal_hit;

	for (int i = 0; i < all_shapes.size(); i++)
	{
		glm::vec3 test_point_hit, test_normal_hit;
		if (all_shapes[i]->intersect(a_ray, test_point_hit, test_normal_hit))
		{ // return true if there is a point of intersection
			float distance = glm::distance(test_point_hit, a_ray.position);
			if (distance < min_distance)
			{
				point_hit = test_point_hit;
				normal_hit = test_normal_hit;
				object = all_shapes[i].get();
				min_distance = distance;
			}
		}
	}
	if (object == NULL)
	{ // No intersection of ray with any of the objects in scene
		return vec3(0, 0, 0);// return the color black - probably should return ambient color
	}

	float bias = 0.001; // just to make sure it doesn't hit something directly next to it, like space it out a bit at 0.0001, i have shadow acne at small circle in scene 2
	bool inside = false; // only useful if i do reflect or refraction --- probably no time
	if (glm::dot(a_ray.direction, normal_hit) > 0)
	{ // Again this won't be used unless extra features implemented
		normal_hit = -normal_hit;
	}
	// Now create a shadow ray at this point for each light to see if the point is in shadow
	glm::vec3 color(0,0,0);
	glm::vec3 ambient_component(0, 0, 0);
	glm::vec3 total_diffuse_component(0, 0, 0);
	glm::vec3 total_specular_component(0, 0, 0);
	bool at_least_one_light = false;
	vec3 total_ambiance;
	for (Light a_light : all_lights)
	{
		total_ambiance += a_light.ambient;
	}
	ambient_component += total_ambiance * object->get_amb();
	int number_of_shadow_rays = 10;
	for (Light a_light : all_lights)
	{ 
		//Soft Shadows implementation
		glm::vec3 diffuse_component(0, 0, 0);
		glm::vec3 specular_component(0, 0, 0);
		Sphere spherical_light = Sphere(); spherical_light.ambient = a_light.ambient; spherical_light.diffuse = a_light.diffuse; spherical_light.specular = a_light.specular; spherical_light.position = a_light.position;
		spherical_light.radius = 1; // initializing the spherical light with light attributes plus radius assigned randomly = 1; // Won't be using radius for calculations
		
		for (int i = 0; i < number_of_shadow_rays; i++)
		{
			bool in_shadow = false;
			glm::vec3 position = point_hit + normal_hit * bias;
			glm::vec3 direction = soft_shadow_illumination(point_hit, spherical_light);
			Ray shadow_ray(position, direction); 
			for (int i = 0; i < all_shapes.size(); i++)
			{
				glm::vec3 test_point_hit;
				glm::vec3 test_point_normal;
				if (all_shapes[i]->intersect(shadow_ray, test_point_hit, test_point_normal))
				{ // It means this point does not receive light from the given light source
					float distance_between_object_poi = glm::distance(test_point_hit, shadow_ray.position);
					float distance_between_poi_light = glm::distance(a_light.position, shadow_ray.position);
					if (distance_between_object_poi >= distance_between_poi_light)
					{

					}
					else {
						in_shadow = true;
						break;
					}
				}
			}// If it goes through all objects without intersecting them, it means it has clear access to light source so take light into account for color
			if (!in_shadow)
			{
				at_least_one_light = true;
				float dot_light_normal = dot(shadow_ray.direction, normal_hit);
				
				if (dot_light_normal < 0)
				{
					dot_light_normal = 0; // clamping to zero
				}
				glm::vec3 reflection_vector = (2 * dot_light_normal * normal_hit) - shadow_ray.direction; // r = 2( l.n ) * n - l
				reflection_vector = glm::normalize(reflection_vector);
				glm::vec3 view_vector = glm::normalize(camera.position - shadow_ray.position);
				float dot_reflect_view = dot(reflection_vector, view_vector);
				if (dot_reflect_view < 0)
				{
					dot_reflect_view = 0; // clamping to zero
				}

				diffuse_component += a_light.diffuse * object->get_dif() * dot_light_normal;
				specular_component += a_light.specular * object->get_spec() * glm::pow(dot_reflect_view, object->get_shi());
			}
		}
		diffuse_component = diffuse_component / (float)number_of_shadow_rays;
		specular_component = specular_component / (float)number_of_shadow_rays;
		total_diffuse_component += diffuse_component;
		total_specular_component += specular_component;
	}
		
		if (at_least_one_light == false)
		{ //point of intersection is in shadow from all lights
			color = object->get_amb();
			return color;
		}
		color = ambient_component + total_diffuse_component + total_specular_component;
		if (color[0] > 1.0)
		{
			color[0] = 1.0;
		}
		if (color[1] > 1.0)
		{
			color[1] = 1.0;
		}
		if (color[2] > 1.0)
		{
			color[2] = 1.0;
		}
		return color;

}


glm::vec3 soft_shadow_illumination(glm::vec3 point_of_intersection, Sphere a_light_sphere)
{
	glm::vec3 direction;
	glm::vec3 random_sampler(((rand() / (float)RAND_MAX) - 0.5f), ((rand() / (float)RAND_MAX) - 0.5f), ((rand() / (float)RAND_MAX) - 0.5f));
	direction = (a_light_sphere.position + random_sampler) - point_of_intersection; // Sampling all across the generated light sphere for consistent illumination
	return direction;
}

// Special thanks to scratchapixel.com. I used one of their intersection algorithms (i.e. with triangle) to then adapt my ray tracer to polygon meshes.
// Special thanks to Oliver Tome, my good friend who provided useful pointers on how to more cleanly implement the code using some modern C++ features.

//Pre-Soft Shadow code, code below will produce hard shadows!
/*
		bool in_shadow = false;
		glm::vec3 position = point_hit + normal_hit * bias;
		glm::vec3 direction(a_light.position[0] - position[0], a_light.position[1] - position[1], a_light.position[2] - position[2]);
		Ray shadow_ray(position, direction);
		for (int i = 0; i < all_shapes.size(); i++)
		{
			glm::vec3 test_point_hit;
			glm::vec3 test_point_normal;
			if (all_shapes[i]->intersect(shadow_ray, test_point_hit, test_point_normal))
			{ // It means this point does not receive light from the given light source
				float distance_between_object_poi = glm::distance(test_point_hit, shadow_ray.position);
				float distance_between_poi_light = glm::distance(a_light.position, shadow_ray.position);
				if (distance_between_object_poi >= distance_between_poi_light)
				{

				}
				else {
					in_shadow = true;
					break;
				}
			}
		}// If it goes through all objects without intersecting them, it means it has clear access to light source so take light into account for color
		if (!in_shadow)
		{
			at_least_one_light = true;
			float dot_light_normal = dot(shadow_ray.direction, normal_hit);
			if (dot_light_normal < 0)
			{
				dot_light_normal = 0; // clamping to zero
			}
			glm::vec3 reflection_vector = (2 * dot_light_normal * normal_hit) - shadow_ray.direction; // r = 2( l.n ) * n - l
			reflection_vector = glm::normalize(reflection_vector);
			glm::vec3 view_vector = glm::normalize(camera.position - shadow_ray.position);
			float dot_reflect_view = dot(reflection_vector, view_vector);
			if (dot_reflect_view < 0)
			{
				dot_reflect_view = 0; // clamping to zero
			}
			//else{
			//	cout << "hello";
			//}

			diffuse_component += a_light.diffuse * object->get_dif() * dot_light_normal;
			specular_component += a_light.specular * object->get_spec() * glm::pow(dot_reflect_view, object->get_shi());

			// below goes like this: light amb,diff, or spec component multiplied by the objects corresponding amb,diff or spec component
			// glm::vec3 lighting = a_light.diffuse * diffuse_component + a_light.specular * specular_component;
			//glm::vec3 lighting = a_light.diffuse * object->get_dif() * dot_light_normal;// a_light.ambient * object->get_amb();// +a_light.diffuse * object->get_dif() * dot_light_normal;// +a_light.specular * object->get_spec() * glm::pow(dot_reflect_view, object->get_shi());
			//color += object->get_color() * a_light.get_color() * std::max(0.0f, glm::dot(normal_hit,shadow_ray.direction));
		}
	}
	if (at_least_one_light == false)
	{ //point of intersection is in shadow from all lights
		color = object->get_amb();
		return color;
	}
	color = ambient_component + diffuse_component + specular_component;
	if (color[0] > 1.0)
	{
		color[0] = 1.0;
	}
	if (color[1] > 1.0)
	{
		color[1] = 1.0;
	}
	if (color[2] > 1.0)
	{
		color[2] = 1.0;
	}
	return color;
	*/