#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>

using namespace glm;

class MyTrans
{
public:
	MyTrans();
	MyTrans(float w, float h);
	virtual ~MyTrans();

	// model setters
	void SetModelScale(float scale_x, float scale_y, float scale_z);

	void SetModelTranslateX(float x);
	void SetModelTranslateY(float y);
	void SetModelTranslateZ(float z);

	void SetModelRotateX(float angle);
	void SetModelRotateY(float angle);
	void SetModelRotateZ(float angle);
	
	// model getters
	const mat4x4& GetModelScale() const;

	const mat4x4& GetModelTranslate() const;
	void ChangeTransToViewport(int w, int h);

	const mat4x4& GetModelRotateX() const;
	const mat4x4& GetModelRotateY() const;
	const mat4x4& GetModelRotateZ() const;

	const float GetModelAngleX() const;
	const float GetModelAngleY() const;
	const float GetModelAngleZ() const;
	
	// world setters
	void SetWorldScale(float scale_x, float scale_y, float scale_z);

	void SetWorldTranslateX(float x);
	void SetWorldTranslateY(float y);
	void SetWorldTranslateZ(float z);

	void SetWorldRotateX(float angle);
	void SetWorldRotateY(float angle);
	void SetWorldRotateZ(float angle);


	// world getters
	const mat4x4& GetWorldScale() const;

	const mat4x4& GetWorldTranslate() const;

	const mat4x4& GetWorldRotateX() const;
	const mat4x4& GetWorldRotateY() const;
	const mat4x4& GetWorldRotateZ() const;

	const float GetWorldAngleX() const;
	const float GetWorldAngleY() const;
	const float GetWorldAngleZ() const;


	const vec4& GetEye();
	void SetEye(glm::vec4 eye);
	void SetEyeX(float x);
	void SetEyeY(float y);
	void SetEyeZ(float z);

	const vec4& GetAt();
	void SetAt(glm::vec4 at);
	void SetAtX(float x);
	void SetAtY(float y);
	void SetAtZ(float z);

	const vec4& GetUp();
	void SetUp(glm::vec4 up);
	void SetUpX(float x);
	void SetUpY(float y);
	void SetUpZ(float z);

	bool BBox;
	bool NormalsFace;
	bool NormalsVertex;

private:
	mat4x4 model_scale_;
	mat4x4 model_translate_;
	mat4x4 model_rotate_by_x_;
	mat4x4 model_rotate_by_y_;
	mat4x4 model_rotate_by_z_;
	
	float model_angle_x_;
	float model_angle_y_;
	float model_angle_z_;

	mat4x4 world_scale_;
	mat4x4 world_translate_;
	mat4x4 world_rotate_by_x_;
	mat4x4 world_rotate_by_y_;
	mat4x4 world_rotate_by_z_;
	
	float world_angle_x_;
	float world_angle_y_;
	float world_angle_z_;
	
	glm::vec4 eye_;
	glm::vec4 at_;
	glm::vec4 up_;
				
};



