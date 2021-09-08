#include "Auxiliary.h"
#include <iostream>
#include <Utils.h>


//default c'tor
MyTrans::MyTrans():
	model_scale_(mat4x4(1)),
	world_scale_(mat4x4(1)),
	model_translate_(mat4x4(1)),
	model_rotate_by_x_(mat4x4(1)),
	model_rotate_by_y_(mat4x4(1)),
	model_rotate_by_z_(mat4x4(1)),
	world_rotate_by_x_(mat4x4(1)),
	world_rotate_by_y_(mat4x4(1)),
	world_rotate_by_z_(mat4x4(1)),
	world_translate_(mat4x4(1)),
	eye_({0,0,5,0}),
	at_({0,0,0,0}),
	up_({0,1,0,0}),
	model_angle_x_(0.0f),
	model_angle_y_(0.0f),
	model_angle_z_(0.0f),
	world_angle_x_(0.0f),
	world_angle_y_(0.0f),
	world_angle_z_(0.0f),
	BBox(0),
	NormalsFace(0),
	NormalsVertex(0)
	
{

}

MyTrans::MyTrans(float w, float h) :
	model_scale_(mat4x4(1)),
	world_scale_(mat4x4(1)),
	model_rotate_by_x_(mat4x4(1)),
	model_rotate_by_y_(mat4x4(1)),
	model_rotate_by_z_(mat4x4(1)),
	world_rotate_by_x_(mat4x4(1)),
	world_rotate_by_y_(mat4x4(1)),
	world_rotate_by_z_(mat4x4(1)),
	world_translate_(mat4x4(1)),
	eye_({ 0,0,5,0 }),
	at_({ 0,0,0,0 }),
	up_({ 0,1,0,0 }),
	model_angle_x_(0.0f),
	model_angle_y_(0.0f),
	model_angle_z_(0.0f),
	world_angle_x_(0.0f),
	world_angle_y_(0.0f),
	world_angle_z_(0.0f),
	BBox(0),
	NormalsFace(0),
	NormalsVertex(0),
	model_translate_(1)
{
	
}

// d'tor
MyTrans::~MyTrans()
{

}

// - - - - - - - - - - - - - model getters  - - - - - - - - - - - - - - - - -

const mat4x4& MyTrans::GetModelScale() const
{
	return model_scale_;
}

const mat4x4& MyTrans::GetModelTranslate() const
{
	return model_translate_;
}

void MyTrans::ChangeTransToViewport(int w, int h)
{
	model_translate_ = glm::transpose(glm::mat4x4({ { 1, 0 , 0 , w / 2 }, // sizing an object
							  { 0, 1 , 0 , h / 2 },				
							  { 0, 0 , 1 , 0 },
							  { 0, 0 , 0   , 1 } }));
}

const mat4x4& MyTrans::GetModelRotateX() const
{
	return model_rotate_by_x_;
}

const mat4x4& MyTrans::GetModelRotateY() const
{
	return model_rotate_by_y_;
}

const mat4x4& MyTrans::GetModelRotateZ() const
{
	return model_rotate_by_z_;
}

const float MyTrans::GetModelAngleX() const
{
	return model_angle_x_;
}

const float MyTrans::GetModelAngleY() const
{
	return model_angle_y_;
}

const float MyTrans::GetModelAngleZ() const
{
	return model_angle_z_;
}


// - - - - - - - - - - - - - model setters  - - - - - - - - - - - - - - - - -

void MyTrans::SetModelScale(float scale_x, float scale_y, float scale_z)
{
	model_scale_ = glm::mat4x4({ scale_x, 0 , 0 , 0, // sizing an object
										0, scale_y , 0 , 0,
										0, 0 , scale_z , 0,
										0, 0 , 0   , 1 });	
}

void MyTrans::SetModelTranslateX(float x)
{
	model_translate_[3][0] = x;
}

void MyTrans::SetModelTranslateY(float y)
{
	model_translate_[3][1] = y;
}

void MyTrans::SetModelTranslateZ(float z)
{
	model_translate_[3][2]= z;
}

void MyTrans::SetModelRotateX(float angle)		// Rotate model around the model X axis	
{
	model_angle_x_ = angle * ((float)3.14159265359 / 180.0f);
	model_rotate_by_x_ = mat4x4({ {1, 0 , 0 , 0},						
		{0, cos(model_angle_x_)  , -sin(model_angle_x_) , 0},
		{0, sin(model_angle_x_)  ,  cos(model_angle_x_) , 0},
		{0, 0           ,  0          , 1 } });
	model_angle_x_ = angle;
}

void MyTrans::SetModelRotateY(float angle)		// Rotate model around the model Y axis
{
	model_angle_y_ = angle * ((float)3.14159265359 / 180.0f);  
	model_rotate_by_y_ = mat4x4({ { cos(model_angle_y_), 0 , sin(model_angle_y_) , 0}, 
								{  0,          1 , 0          , 0},
								{-sin(model_angle_y_), 0 , cos(model_angle_y_) , 0},
								{  0,          0 , 0          , 1 } });
	model_angle_y_ = angle;
}

void MyTrans::SetModelRotateZ(float angle)		// Rotate model around the model Y axis
{
	model_angle_z_ = angle * ((float)3.14159265359 / 180.0f);
	model_rotate_by_z_ = mat4x4({ { cos(model_angle_z_), -sin(model_angle_z_) , 0 , 0}, 
		{ sin(model_angle_z_), cos(model_angle_z_)  , 0 , 0},
		{0,          0           , 1 , 0},
		{0,          0           , 0 , 1 } });
	model_angle_z_ = angle;
}

// - - - - - - - - - - - - - world getters  - - - - - - - - - - - - - - - - -

const mat4x4& MyTrans::GetWorldScale() const
{
	return world_scale_;
}

const mat4x4& MyTrans::GetWorldTranslate() const
{
	return world_translate_;
}

const mat4x4& MyTrans::GetWorldRotateX() const
{
	return world_rotate_by_x_;
}

const mat4x4& MyTrans::GetWorldRotateY() const
{
	return world_rotate_by_y_;
}

const mat4x4& MyTrans::GetWorldRotateZ() const
{
	return world_rotate_by_z_;
}

const float MyTrans::GetWorldAngleX() const
{
	return world_angle_x_;
}

const float MyTrans::GetWorldAngleY() const
{
	return world_angle_y_;
}

const float MyTrans::GetWorldAngleZ() const
{
	return world_angle_z_;
}


// - - - - - - - - - - - - - world setters  - - - - - - - - - - - - - - - - -

void MyTrans::SetWorldScale(float scale_x, float scale_y, float scale_z)	// sizing an object
{
	world_scale_ = glm::mat4x4({ scale_x, 0 , 0 , 0, 
										0, scale_y , 0 , 0,
										0, 0 , scale_z , 0,
										0, 0 , 0   , 1 });;
}

void MyTrans::SetWorldTranslateX(float x)
{
	world_translate_[3][0] = x;
}

void MyTrans::SetWorldTranslateY(float y)
{
	world_translate_[3][1] = y;
}

void MyTrans::SetWorldTranslateZ(float z)
{
	world_translate_[3][2] = z;
}

void MyTrans::SetWorldRotateX(float angle)		// Rotate model around the world X axis
{
	world_angle_x_ = angle * ((float)3.14159265359 / 180.0f);
	world_rotate_by_x_ = mat4x4({ {1, 0 , 0 , 0},
		{0, cos(world_angle_x_)  , -sin(world_angle_x_) , 0},
		{0, sin(world_angle_x_)  ,  cos(world_angle_x_) , 0},
		{0, 0           ,  0          , 1 } });
	world_angle_x_ = angle;
}

void MyTrans::SetWorldRotateY(float angle)		// Rotate model around the world Y axis
{
	world_angle_y_ = angle * ((float)3.14159265359 / 180.0f);
	world_rotate_by_y_ = mat4x4({ { cos(world_angle_y_), 0 , sin(world_angle_y_) , 0}, 
					{  0,          1 , 0          , 0},
					{-sin(world_angle_y_), 0 , cos(world_angle_y_) , 0},
					{  0,          0 , 0          , 1 } });
	world_angle_y_ = angle;
}

void MyTrans::SetWorldRotateZ(float angle)		// Rotate model around the world Z axis
{
	world_angle_z_ = angle * ((float)3.14159265359 / 180.0f);
	world_rotate_by_z_ = mat4x4({ { cos(world_angle_z_), -sin(world_angle_z_) , 0 , 0},
		{ sin(world_angle_z_), cos(world_angle_z_)  , 0 , 0},
		{0,          0           , 1 , 0},
		{0,          0           , 0 , 1 } });
	world_angle_z_ = angle;
}

// camera parameters 

const vec4& MyTrans::GetEye()
{
	return eye_;
}

void MyTrans::SetEyeX(float x)
{
	eye_.x = x;
}

void MyTrans::SetEyeY(float y)
{
	eye_.y = y;
}

void MyTrans::SetEyeZ(float z)
{
	eye_.z = z;
}

const vec4& MyTrans::GetAt()
{
	return at_;
}

void MyTrans::SetAtX(float x)
{
	at_.x = x;
}

void MyTrans::SetAtY(float y)
{
	at_.y = y;
}

void MyTrans::SetAtZ(float z)
{
	at_.z = z;
}

const vec4& MyTrans::GetUp()
{
	return up_;
}

void MyTrans::SetUpX(float x)
{
	up_.x = x;
}

void MyTrans::SetUpY(float y)
{
	up_.y = y;
}

void MyTrans::SetUpZ(float z)
{
	up_.z = z;
}

void MyTrans::SetEye(glm::vec4 eye)
{
	eye_ = eye;
}

void MyTrans::SetAt(glm::vec4 at)
{
	at_ = at;
}

void MyTrans::SetUp(glm::vec4 up)
{
	up_ = up;
}