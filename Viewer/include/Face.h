#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetNumOfVertices() const;
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;
	const glm::vec4& GetFaceNormal() const;
	const glm::vec4& GetFaceCenter() const;
	void SetFaceCenter(glm::vec4 face_center);
	void SetFaceNormal(glm::vec4 face_normal);
	void SetUnitFaceNormal(glm::vec4& unit_face_normal) { unit_face_normal_ = unit_face_normal; }
	glm::vec4& GetUnitFaceNormal()  { return unit_face_normal_; }
	void UpdateD(float d);
	float GetD() const { return d_; }
private:
	std::vector<int> vertex_indices_;
	std::vector<int> normal_indices_;
	std::vector<int> texture_indices_;
	glm::vec4 face_center_;
	glm::vec4 face_normal_;
	glm::vec4 unit_face_normal_;
	float d_;
};