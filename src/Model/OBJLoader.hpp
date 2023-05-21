#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/vec2.hpp> 
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/Buffer/Vertex/Vertex.hpp"
//~GLM


/*
*	.obj File Loader (Blender Models...)
* Blender -> File -> Export as .obj
*/

class OBJLoader
{ 
public:
	static std::vector<Vertex> LoadOBJ(const std::string& filename)
	{
		// Vertex Portions
		std::vector<glm::fvec3> positions;
		std::vector<glm::fvec2> texcoords;
		std::vector<glm::fvec3> normals;

		// Face Vectors
		std::vector<GLint> position_indices;
		std::vector<GLint> normal_indices;
		std::vector<GLint> texcoord_indices;
	
		// Vertex Array Output
		std::vector<Vertex> vertices;

		std::stringstream ss;
		std::string line;
		std::string prefix;

		glm::vec3 temp_vec3;
		glm::vec2 temp_vec2;
		GLint temp_glint = 0;
		if (std::ifstream ifs{ filename, std::ios::in })
		{
			//Read line by line
			while (std::getline(ifs, line))
			{
				//Get line prefix
				ss.clear();
				ss.str(line);
				ss >> prefix;

				//Check what prefix is
				if (prefix == "#") // Comment, ignore it
				{
					continue;
				}
				else if (prefix == "o")
				{
					continue;
				}
				else if (prefix == "s") // Smooth shading on/off
				{
					continue;
				}
				else if (prefix == "use_mtl") // 
				{
					continue;
				}
				else if (prefix == "v") // Vertex position
				{
					ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
					positions.push_back(temp_vec3);
				}
				else if (prefix == "vt") // Texture vertex
				{
					ss >> temp_vec2.x >> temp_vec2.y;
					texcoords.push_back(temp_vec2);
				}
				else if (prefix == "vn") //  Vertex normal
				{
					ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
					normals.push_back(temp_vec3);
				}
				else if (prefix == "f") //  
				{
					int counter = 0;
					while (ss >> temp_glint)
					{
						if (counter == 0) // first position for position
							position_indices.push_back(temp_glint);
						else if(counter == 1) // second position for texcoord
							texcoord_indices.push_back(temp_glint);
						else if(counter == 2) // third position for normal
							normal_indices.push_back(temp_glint);

						//Check if next character if its '/'
						if (ss.peek() == '/')
						{
							++counter; //move forward, there is something after /
							ss.ignore(1, '/'); // ignore /
						}
						else if (ss.peek() == ' ')
						{
							++counter;
							ss.ignore(1, ' ');
						}
						
						//Reset counter
						if (counter > 2) counter = 0;
					}
				}
				else
				{
					continue;
				}
			}

			//Build final vertex array (mesh)
			//resize vertices with the needed space
			vertices.resize(position_indices.size(), Vertex());
			//Load in all indices
			for (size_t i = 0; i < vertices.size(); ++i)
			{
				vertices[i].position = positions[position_indices[i] - 1];
				vertices[i].texture_coords = texcoords[texcoord_indices[i] - 1];
				vertices[i].normal = normals[normal_indices[i] - 1];
				vertices[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
			}

      ifs.close();
		}
		else throw std::runtime_error("Failed to open obj file " + filename);
		
		std::cout << filename << " Loaded Successfully.\n";
		std::cout << "Loaded " << vertices.size() << " Vertices.\n";
		return vertices;
	}
};
