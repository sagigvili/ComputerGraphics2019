#include "MeshModel.h"
#include <algorithm> 
#include "Utils.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>



MeshModel::MeshModel()
{
}


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name) :
	faces_(faces),
	normals_(normals),
	model_name_(model_name),
	model_transform_(glm::mat4x4(1)),
	world_transform_(glm::mat4x4(1)),
	move_to_center_(1.0f),
	move_to_center_inv_(1.0f),
	My_Updates(MyTrans()),	
	translate_to_0(glm::mat4x4(1.0f)),
	faces_per_vertex_(vertices_.size()),
	vertices_normals_(vertices_.size())
	
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	model_color = glm::vec4(dist(mt), dist(mt), dist(mt), 1.0f);

	vertices_.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j);

			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textures_.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j);
				vertex.textureCoords = textures_[textureCoordsIndex];
			}

			vertices_.push_back(vertex);
		}
	}

	ambient = vec4(0.0f);
	diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	k1 = 0.5f;
	k2 = 0.5f;
	k3 = 0.5f;
	k4 = 1.0f;
	
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

GLuint MeshModel::GetVBO() const
{
	return vbo;
}

void MeshModel::UpdateTransformation(MyTrans& input)
{
	glm::mat4x4 model_rotate(1);
	glm::mat4x4 world_rotate(1);
	if (first_mult_ == X_FIRST)
	{
		model_rotate = input.GetModelRotateZ() * input.GetModelRotateY() * input.GetModelRotateX();
		world_rotate = input.GetWorldRotateZ() * input.GetWorldRotateY() * input.GetWorldRotateX();
	}
	else
	{
		if (first_mult_ == Y_FIRST)
		{
			model_rotate = input.GetModelRotateZ()  * input.GetModelRotateX() * input.GetModelRotateY();
			world_rotate = input.GetWorldRotateZ()  * input.GetWorldRotateX() * input.GetWorldRotateY();
		}
		else
		{
			if (first_mult_ == Z_FIRST)
			{
				model_rotate = input.GetModelRotateY() * input.GetModelRotateX() * input.GetModelRotateZ();
				world_rotate = input.GetWorldRotateY() * input.GetWorldRotateX() * input.GetWorldRotateZ();
			}
		}
	}
			model_transform_ = input.GetModelTranslate() * model_rotate  * input.GetModelScale() * translate_to_0;

	

	
	world_transform_ = input.GetWorldScale() * input.GetWorldTranslate() * world_rotate;

	My_Updates = input;
}





int MeshModel::GetFacesCount() const
{
	return faces_.size();
}


const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}


const Vertex& MeshModel::GetVertex(int index) const
{
	return vertices_[index];
}


const std::vector<Vertex> MeshModel::GetVertices() const
{

	return vertices_;
}


const std::vector<glm::vec3>& MeshModel::GetNormals() const
{
	return normals_;
}


const std::vector<glm::vec2> MeshModel::GetTextures() const
{
	return textures_;
}

void MeshModel::SetWorldTrans(const glm::mat4x4& mat)
{
	world_transform_ = mat;
}


const glm::mat4x4& MeshModel::GetWorldTransform()
{
	return world_transform_;
}


void MeshModel::SetModelColor(const glm::vec4& color)
{
	model_color = color;
}


const glm::vec4& MeshModel::GetModelColor()
{
	return model_color;
}


const glm::mat4x4& MeshModel::GetModelTransform()
{
	return model_transform_;
}


MyTrans& MeshModel::GetMyUpdates()
{
	return My_Updates;
}


int MeshModel::GetVerticesCount() const
{
	return vertices_.size();
}
