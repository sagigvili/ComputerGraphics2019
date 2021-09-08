#pragma once
#include "Scene.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "Texture2D.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>



class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetViewportWidth(int width);
	void SetViewportHeight(int height);
	void SetViewport(int width, int height);

	
	
	const glm::mat4x4& GetViewPortMatrix() const;
	void SetViewPortMartrix();

	glm::vec4 ambient_scene;
	bool wirmframe;
	glm::vec4 wirmframe_color;
	void LoadShaders();


private:

	ShaderProgram colorShader;

	glm::mat4x4 complete_trans_;
	glm::mat4x4 view_port_matrix;

	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	bool local_trans;		// local transformtion = true , world transformtion = false
	
};
