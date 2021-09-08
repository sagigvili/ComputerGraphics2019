#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index_(0),	
	active_model_index_(0),	// PROBLEM -  0 indicates existence of models even if non were added
	active_light_index_(0),
	background_color_(0.2f, 0.2f, 0.2f, 1.00f)
{
	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	Camera camera = Camera(eye, at, up, static_cast<float>(1280) / static_cast<float>(720));
	AddCamera(camera);
	SetActiveCameraIndex(active_camera_index_);

	
}

std::vector<Camera>& Scene::GetCameras()
{
	return cameras_;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

const std::vector<std::shared_ptr<MeshModel>> Scene::GetModels()
{
	return mesh_models_;
}

const std::shared_ptr<MeshModel>& Scene::GetModel(int index) const
{
	return mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

const std::shared_ptr<Light>& Scene::GetActiveLight() const
{
	return lights_[active_light_index_];
}

void Scene::AddCamera(Camera camera)		
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

const Camera& Scene::GetCamera(int index) const
{
	return cameras_[index];
}

Camera& Scene::GetActiveCamera()
{
	return cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex()
{
	active_model_index_ = mesh_models_.size() - 1;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

void Scene::PopLastModel()
{
	mesh_models_.pop_back();
}

void Scene::PopLastLight()
{
	lights_.pop_back();
}

void Scene::setFogType(int type)
{
	this->fogType = type;
}

void Scene::setFogColor(const glm::vec4& color)
{
	this->fogColor = color;
}

void Scene::setFogBegin(float begin)
{
	this->fogStart = begin;
}

void Scene::setFogEnd(float end)
{
	this->fogEnd = end;
}

void Scene::setDensity(float den)
{
	this->fogDensity = den;
}

void Scene::applyFog(ShaderProgram& shader) const
{
	shader.setUniform("fog.fogType", fogType);
	shader.setUniform("fog.fogColor", fogColor);
	shader.setUniform("fog.density", fogDensity);
	shader.setUniform("fog.start", fogStart);
	shader.setUniform("fog.end", fogEnd);
}