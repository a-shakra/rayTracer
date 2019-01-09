# rayTracer

Ray Tracer developed for the COMP 371 course at Concordia University. The ray tracer performs as follows:
- A file describing the scene is given as input following a predetermined format. A sample file is included in the project.
- The data in the file is used to generate the objects in the scene, in addition to determining the size of the image plane
- Using this information, a scene is geneated using the backwards ray tracing method

The Ray Tracer is capable of handling:
- Spheres
- Triangles
- Planes
- Polygons (Mesh)

The Ray Tracer implements Anti-Aliasing and Soft Shadows.

The project contains:
parser.h: Used to read the given file and declare and initialize the necessary objects in the scene
objLoader.h: Used to read and obj file. This file is based on pre-exisiting external code with some personal modifications to suit the project.
util.h: a util file that essentially contains the classes of all potential objects in the scene, each with their own methods and member variables.
main.cpp: main file where all the magic happens :)

