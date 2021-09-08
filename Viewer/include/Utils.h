#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static glm::vec4 Vec4FromVec3(const glm::vec3& other);
	static char** convertStringVectorToCharArray(std::vector<std::string> vec);
	static std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices, std::vector<Face> faces);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);
};
