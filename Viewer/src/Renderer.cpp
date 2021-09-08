#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include <iostream>



using namespace std;


Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height),
	ambient_scene(0.0f),
	wirmframe_color(1.0f),
	wirmframe(false),
	local_trans(TRUE)		// default - local transformation

{

	//InitOpenGLRendering();
	SetViewport(viewport_width, viewport_height);
}

Renderer::~Renderer()
{


}

void Renderer::SetViewport(int width, int height)
{
	SetViewportWidth(width);
	SetViewportHeight(height);
	SetViewPortMartrix();

}

void Renderer::Render(Scene& scene)
{


	int lightCount = scene.GetLightCount();
	glm::vec4 lightColors[10];
	glm::vec4 lightPos[10];
	for (int i = 0; i < lightCount; ++i)
	{
		// setup lights
		if (i < lightCount) {
			lightColors[i] = scene.GetLight(i)->getIntensity();
			if (scene.GetLight(i)->getType()) {
				lightPos[i] = scene.GetLight(i)->getLightPos();
			}
			else {
				lightPos[i] = scene.GetLight(i)->getDirection();
			}
		}
		else {
			lightColors[i] = glm::vec4(0.0f);
			lightPos[i] = glm::vec4(0.0f);
		}
	}



	int cameraCount = scene.GetCameraCount();
	if (cameraCount > 0)
	{
		const Camera& camera = scene.GetActiveCamera();
		colorShader.use();
		colorShader.setUniform("view", camera.GetViewTransformation());
		colorShader.setUniform("projection", camera.GetProjectionTransformation());
		colorShader.setUniform("eye", camera.GetEye());
		colorShader.setUniform("SceneAmbient", ambient_scene);
		colorShader.setUniform("lightColor", lightColors);
		colorShader.setUniform("lightPos", lightPos);
		colorShader.setUniform("lightsCount", lightCount);

		GLuint cur_vao;
		GLuint cur_vbo;
		std::shared_ptr<MeshModel> currentModel;

		int modelCount = scene.GetModelCount();
		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			currentModel = scene.GetModel(currentModelIndex);

			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetWorldTransform() * currentModel->GetModelTransform());
			colorShader.setUniform("wirmframe", 0);
			colorShader.setUniform("modelColor", currentModel->GetModelColor());
			colorShader.setUniform("material.AmbientColor", currentModel->ambient);
			colorShader.setUniform("material.DiffuseColor", currentModel->diffuse);
			colorShader.setUniform("material.SpecualrColor", currentModel->specular);
			colorShader.setUniform("material.KA", currentModel->k1);
			colorShader.setUniform("material.KD", currentModel->k2);
			colorShader.setUniform("material.KS", currentModel->k3);
			colorShader.setUniform("material.KSE", currentModel->k4);

			scene.applyFog(colorShader);

			// VAO = Vertex Array Objects
			// VBO = Vertex Buffer Objects
			cur_vao = currentModel->GetVAO();
			cur_vbo = currentModel->GetVBO();

			glGenVertexArrays(1, &cur_vao);
			glBindVertexArray(cur_vao);

			glGenBuffers(1, &cur_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, cur_vbo);

			glBufferData(GL_ARRAY_BUFFER, currentModel->GetVertices().size() * sizeof(Vertex), &currentModel->GetVertices()[0], GL_STATIC_DRAW);

			// Vertex Positions
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			// Normals attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);


			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(cur_vao);
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetVertices().size());
			glBindVertexArray(0);

			if (wirmframe)
			{
				colorShader.setUniform("wirmframe", 1);
				colorShader.setUniform("wirmframe_color", wirmframe_color);
				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(cur_vao);
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetVertices().size());
				glBindVertexArray(0);
			}
		}
	}

}


void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

void Renderer::SetViewPortMartrix()
{
	// CVV - cannonical view volume - move it by 1 on x and by 1 on y 
	// create a new view volume (0 to 2)
	glm::mat4x4 translate_CVV = glm::transpose(glm::mat4x4({ {1,0,0,1}, {0,1,0,1}, {0,0,1,0}, {0,0,0,1} }));
	glm::mat4x4 view_port_trans = { { viewport_width_ / 2 , 0 , 0 , 0 } ,
									{ 0 , viewport_height_ / 2 , 0 , 0 },
									{ 0 , 0 , 1 , 0 },
									{ 0 , 0 , 0 , 1 } };
	view_port_matrix = view_port_trans * translate_CVV;

}


int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}


int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}


void Renderer::SetViewportWidth(int width)
{
	viewport_width_ = width;

}


void Renderer::SetViewportHeight(int height)
{
	viewport_height_ = height;
}



const glm::mat4x4& Renderer::GetViewPortMatrix() const
{
	return view_port_matrix;
}


