#pragma once
#include <istream>
#include "Face.h"

Face::Face(std::istream& issLine)
{
	vertex_indices_  = { 0,0,0 };
	normal_indices_  = { 0,0,0 };
	texture_indices_ = { 0,0,0 };

	char c;
	for (int i = 0; i < 3; i++)
	{
		issLine >> std::ws >> vertex_indices_.at(i) >> std::ws;
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> std::ws;
		if (issLine.peek() == '/')
		{
			issLine >> c >> std::ws >> normal_indices_.at(i);
			continue;
		}

		issLine >> texture_indices_.at(i);
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> normal_indices_.at(i);
	}
	
	
}

void Face::SetFaceCenter(glm::vec4 face_center)
{
	face_center_ = face_center;
}

void Face::SetFaceNormal(glm::vec4 face_normal)
{
	face_normal_ = face_normal;
}


const glm::vec4& Face::GetFaceCenter() const
{
	return face_center_;
}

const glm::vec4& Face::GetFaceNormal() const
{
	return face_normal_;
}

int Face::GetVertexIndex(int internal_index) const
{
	return vertex_indices_[internal_index] - 1;
}

int Face::GetNormalIndex(int internal_index) const
{
	return normal_indices_[internal_index] - 1;
}

int Face::GetTextureIndex(int internal_index) const
{
	return texture_indices_[internal_index] - 1;
}

int Face::GetNumOfVertices() const
{
	return vertex_indices_.size();
}

void Face::UpdateD(float d) 
{ 
	d_ = -(face_normal_.x * face_center_.x + face_normal_.y * face_center_.y + face_normal_.z * face_center_.z); 
}