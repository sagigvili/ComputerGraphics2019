#pragma once
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>
#include <memory>
#include "Face.h"
#include "Auxiliary.h"

#define X_FIRST  0
#define Y_FIRST  1
#define Z_FIRST  2

#define AMBIENT  0
#define DIFFUSE  1
#define SPECURAL  2

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
public:
	MeshModel();
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name);
	GLuint GetVAO() const;
	GLuint GetVBO() const;
	virtual ~MeshModel();
	Face& GetFace(int index)  { return faces_[index]; }
	int GetFacesCount() const;
	int GetVerticesCount() const;
	const std::string& GetModelName() const;

	std::vector<Face>& GetFacesVector() { return faces_; }




	const Vertex& GetVertex(int index) const;
	const std::vector<Vertex> GetVertices() const;
	const std::vector<glm::vec3>& GetNormals() const;
	const std::vector<glm::vec2> GetTextures() const;
	glm::vec4& GetVertexNormal(int index)  { return vertices_normals_[index]; }


	void SetWorldTrans(const glm::mat4x4& mat);
	const glm::mat4x4& GetWorldTransform();

	void SetModelColor(const glm::vec4& color);
	const glm::vec4& GetModelColor();
	const glm::mat4x4& GetModelTransform();
	void UpdateTransformation(MyTrans& input);

	int first_mult_;

	bool GetBBox()										{ return My_Updates.BBox; }
	void UpdateBBox(bool input)							{ My_Updates.BBox = input; }

	MyTrans& GetMyUpdates();
	
	glm::vec4 ambient;
	glm::vec4 diffuse;
    glm::vec4 specular;
	float k1;
	float k2;
	float k3;
	int k4;

protected:
	std::string model_name_;
	glm::vec4 model_color;
	GLuint vbo;
	GLuint vao;

private:
	std::vector<Face> faces_;
	std::vector<Vertex> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> textures_;
	std::vector<std::vector<int>> faces_per_vertex_;	// each verex holds the indices of faces he's in
	std::vector<glm::vec4> vertices_normals_;

	glm::mat4x4 world_transform_;
	glm::mat4x4 model_transform_;
	glm::mat4x4 translate_to_0;
	glm::mat4x4 move_to_center_;
	glm::mat4x4 move_to_center_inv_;

	MyTrans My_Updates;		

};
