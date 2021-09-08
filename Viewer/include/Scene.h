#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Light.h"

class Scene {
public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	const std::vector<std::shared_ptr<MeshModel>> GetModels();


	const std::shared_ptr<MeshModel>& Scene::GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	void SetActiveModelIndex();
	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	void PopLastModel();

	void PopLastLight();
	
	void AddCamera(Camera camera);
	int GetCameraCount() const;
	const Camera& GetCamera(int index) const;
	Camera& GetActiveCamera();
	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void AddLight(const std::shared_ptr<Light>& light)					{ lights_.push_back(light); }
	void SetActiveLightIndex(int index)			{ active_light_index_ = index; }
	void SetActiveLightIndex() { active_light_index_ = lights_.size() - 1; }
	int GetActiveLightIndex() const { return active_light_index_; }
	const std::shared_ptr<Light>& GetActiveLight() const;
	const std::shared_ptr<Light>& GetLight(int index) const		{ return lights_[index]; }
	int GetLightCount() const { return lights_.size(); }


	std::vector<Camera>& GetCameras();
	const glm::vec4& GetBackgroundColor() const { return background_color_; }

	int lighting_type = 0;

	void setFogType(int type);
	void setFogColor(const glm::vec4& color);

	void setFogBegin(float begin);
	void setFogEnd(float end);
	void setDensity(float den);

	void applyFog(ShaderProgram& shader) const;

private:

	int fogType;
	glm::vec4 fogColor;
	float fogEnd;
	float fogStart;
	float fogDensity;

	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<Camera> cameras_;
	std::vector<std::shared_ptr<Light>> lights_;

	int active_camera_index_;
	int active_model_index_;
	int active_light_index_;

	glm::vec4 background_color_;
	
};