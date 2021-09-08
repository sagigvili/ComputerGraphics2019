#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "Camera.h"
#include "Light.h"






/**
 * Fields
 */
bool show_demo_window = false;
bool show_selection_window = false;
bool show_editor = false;
bool showFogWindow = false;
int fogType = 0;
int count_light = 0;

int windowWidth = 1280;
int windowHeight = 720;


glm::vec4 clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.00f);
glm::vec3 light_color = glm::vec3(0.0f, 1.0f, 0.0f);
std::vector<MeshModel> models_combo;
static const char* item_current_combo;
static const char* camera_current_combo;
bool camera_control;
MyTrans UIinput;
std::vector<std::string> lights;

GLFWwindow* window_high;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer);
float GetAspectRatio(int width, int height);
void glfw_OnFramebufferSize(GLFWwindow* window, Scene& scene, int width, int height);


/**
 * Function implementation
 */


void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{		
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;
	window_high = window;
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	int frameBufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	Renderer& renderer = Renderer(frameBufferWidth, frameBufferHeight);
	renderer.LoadShaders();
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	while (!glfwWindowShouldClose(window))    
    {
        glfwPollEvents();	
		StartFrame();
		DrawImguiMenus(io, scene ,renderer);			
		RenderFrame(window, scene, renderer, io);   
    }  


	Cleanup(window);	
    return 0;			
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0] && !scene.GetModels().empty())
		{

		}
		if (io.MouseDown[1])
		{
			show_selection_window = true;
		}
	}
	ImGui::Render();	// here
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if ((frameBufferWidth != windowWidth || frameBufferHeight != windowHeight) && (frameBufferWidth != 0 && frameBufferHeight !=0))
	{
		glfw_OnFramebufferSize(window, scene, frameBufferWidth, frameBufferHeight);
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.Render(scene);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void glfw_OnFramebufferSize(GLFWwindow* window, Scene& scene, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene.GetActiveCamera().SetAspectRatio(GetAspectRatio(width, height));
}

float GetAspectRatio(int width, int height)
{
	return static_cast<float>(width) / static_cast<float>(height);
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");   
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					scene.SetActiveModelIndex();
					UIinput = MyTrans(renderer.GetViewportWidth(), renderer.GetViewportHeight());
					scene.GetActiveModel().UpdateTransformation(UIinput);
					models_combo.push_back(scene.GetActiveModel());
					item_current_combo = scene.GetActiveModel().GetModelName().c_str();
					show_editor = TRUE;	
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Add Camera", "CTRL+T"))
			{
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 2 * M_PI);
				std::uniform_real_distribution<double> dist2(2, 10);
				double angle = dist(mt);
				double radius = dist2(mt);

				glm::vec4 eye = glm::vec4(radius * glm::cos(angle), 0, radius * glm::sin(angle), 1);
				glm::vec4 at = glm::vec4(0, 0, 0, 1);
				glm::vec4 up = glm::vec4(0, 1, 0, 1);
				scene.AddCamera(Camera(-scene.GetActiveCamera().GetEye(), at, up, GetAspectRatio(windowWidth, windowHeight)));
			}
			
			if (ImGui::MenuItem("Add Point Light", "CTRL+L"))
			{
				std::string light_name = "Point light ";
				light_name += std::to_string(count_light);
				count_light++;
				scene.AddLight(std::make_shared<Light>(1,light_name));
				scene.SetActiveLightIndex();
				lights.push_back(light_name);
			}
			if (ImGui::MenuItem("Add Parallel Light", "CTRL+O"))
			{
				std::string light_name = "Parallel light ";
				light_name += std::to_string(count_light);
				count_light++;
				scene.AddLight(std::make_shared<Light>(0,light_name));
				scene.SetActiveLightIndex();
				lights.push_back(light_name);
			}
			if (ImGui::MenuItem("Delete Active Light", "CTRL+S"))
			{
				if (scene.GetLightCount())
				{
					scene.PopLastLight();
					if (scene.GetLightCount() == 0)
						scene.SetActiveLightIndex(scene.GetLightCount());
					else {
						scene.SetActiveLightIndex(scene.GetLightCount() - 1);
					}
					count_light--;
					lights.pop_back();
				}
			}
			if (ImGui::MenuItem("Delete Active Model", "CTRL+D"))
			{
				if (!scene.GetModels().empty())
				{
					scene.PopLastModel();
					if (scene.GetModels().empty())
					{
						show_editor = false;
						scene.SetActiveModelIndex(scene.GetModels().size());
						item_current_combo = "<NONE>";
					}
					else {
						scene.SetActiveModelIndex(scene.GetModels().size() - 1);
						item_current_combo = scene.GetActiveModel().GetModelName().c_str();
						UIinput = scene.GetActiveModel().GetMyUpdates();
					}
					models_combo.pop_back();
				}
			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	if (show_selection_window)
	{
		ImGui::Begin("Select Active Model", &show_selection_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		if (models_combo.empty())
			item_current_combo = "<NONE>";
		// Here our selection is a single pointer stored outside the object.
		if (ImGui::BeginCombo("Active Model", item_current_combo)) // The second parameter is the label previewed before opening the combo.
		{
			for (std::vector<MeshModel>::iterator it = models_combo.begin(); it != models_combo.end(); ++it) {
				bool is_selected = (item_current_combo == it->GetModelName().c_str());
				if (ImGui::Selectable(it->GetModelName().c_str(), is_selected))
				{
					item_current_combo = it->GetModelName().c_str();
					scene.SetActiveModelIndex(it - models_combo.begin());
					UIinput = scene.GetActiveModel().GetMyUpdates();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::End();
	}
	

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		bool update_view = FALSE;	//  FALSE - no changes, dont update , True - update
		bool update_projection = FALSE;

		Camera& cam = scene.GetActiveCamera();
		ImGui::Begin("UI Handler Window");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

		if (ImGui::ColorEdit3("Background color", (float*)&clear_color)) // Edit 3 floats representing a color
		{
			glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		}
		

		ImGui::Checkbox("Editor", &show_editor);	// check to show model editor
		if (scene.GetModelCount() == 0)
			show_editor = FALSE;

		ImGui::ColorEdit3("Scene Ambient Color", (float*)&renderer.ambient_scene);

		ImGui::Checkbox("Add Fog Options", &showFogWindow);

		if (showFogWindow) {
			static float end = 2.0f;
			static float begin = 0.0f;
			static float density = 1.0f;
			static glm::vec4 fogColor(0.0f, 0.0f, 0.0f, 1.0f);
			ImGui::Begin("Fog Control Window", &showFogWindow);

			if (ImGui::ColorEdit3("Fog color", (float*)&fogColor)) {
				scene.setFogColor(fogColor);
			}

			ImGui::RadioButton("No fog", &fogType, 0); ImGui::SameLine();
			ImGui::RadioButton("Linear", &fogType, 1); ImGui::SameLine();
			ImGui::RadioButton("Exp fog", &fogType, 2); ImGui::SameLine();
			ImGui::RadioButton("Exp squared", &fogType, 3);

			scene.setFogType(fogType);
			if (fogType) {
				if (ImGui::SliderFloat("Fog Start", &begin, 0.0f, 10.0f)) {
					scene.setFogBegin(begin);
				}
				if (ImGui::SliderFloat("Fog End", &end, 0.0f, 20.0f)) {
					scene.setFogEnd(end);
				}
				if (ImGui::SliderFloat("Fog Density", &density, 0.0f, 20.0f)) {
					scene.setDensity(density);
				}
			}
			if (ImGui::Button("Close")) {
				showFogWindow = false;
			}
			ImGui::End();
		}

		if (ImGui::CollapsingHeader("Cameras"))
		{
			const char** items;
			std::vector<std::string> cameraStrings;
			items = new const char* [scene.GetCameraCount()];
			for (int i = 0; i < scene.GetCameraCount(); i++)
			{
				std::ostringstream s;
				s << "Camera " << i;
				std::string mystring = s.str();
				cameraStrings.push_back(mystring);
			}

			for (int i = 0; i < scene.GetCameraCount(); i++)
			{
				items[i] = cameraStrings[i].c_str();
			}

			int currentCamera = scene.GetActiveCameraIndex();
			ImGui::Combo("Active Camera", &currentCamera, items, scene.GetCameraCount());

			if (currentCamera != scene.GetActiveCameraIndex())
			{
				scene.SetActiveCameraIndex(currentCamera);
				scene.GetActiveCamera().SetAspectRatio(GetAspectRatio(windowWidth, windowHeight));
			}

			delete items;

			int newProjectionType = scene.GetActiveCamera().IsPrespective() ? 0 : 1;
			ImGui::RadioButton("Prespective", &newProjectionType, 0);
			ImGui::RadioButton("Orthographic", &newProjectionType, 1);

			if (newProjectionType == 0)
			{
				float fovy = scene.GetActiveCamera().GetFovy();
				scene.GetActiveCamera().SwitchToPrespective();

				if (ImGui::SliderFloat("Fovy", &fovy, 0.0f, M_PI))
				{
					scene.GetActiveCamera().SetFovy(fovy);
				}
			}
			else if (newProjectionType == 1)
			{
				float height = scene.GetActiveCamera().GetHeight();

				scene.GetActiveCamera().SwitchToOrthographic();

				if (ImGui::SliderFloat("Height", &height, 0.0f, 100.0f))
				{
					scene.GetActiveCamera().SetHeight(height);
				}
			}

			float zNear = scene.GetActiveCamera().GetNear();
			float zFar = scene.GetActiveCamera().GetFar();
			glm::vec3 eye = scene.GetActiveCamera().GetEye();
			glm::vec3 at = scene.GetActiveCamera().GetAt();
			glm::vec3 up = scene.GetActiveCamera().GetUp();

			if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
			{
				scene.GetActiveCamera().SetNear(zNear);
			}

			if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
			{
				scene.GetActiveCamera().SetFar(zFar);
			}
			ImGui::Checkbox("Camera Controls", &camera_control);
			if (camera_control)
			{
				// Eye controls
				float eyeX = eye.x;
				float eyeY = eye.y;
				float eyeZ = eye.z;
				if (ImGui::SliderFloat("Eye X", &eyeX, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetEye(vec3(eyeX,eyeY,eyeZ));
				}
				if (ImGui::SliderFloat("Eye Y", &eyeY, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetEye(vec3(eyeX, eyeY, eyeZ));
				}
				if (ImGui::SliderFloat("Eye Z", &eyeZ, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetEye(vec3(eyeX, eyeY, eyeZ));
				}

				// At controls
				float atX = at.x;
				float atY = at.y;
				float atZ = at.z;
				if (ImGui::SliderFloat("At X", &atX, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetAt(vec3(atX, atY, atZ));
				}
				if (ImGui::SliderFloat("At Y", &atY, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetAt(vec3(atX, atY, atZ));
				}
				if (ImGui::SliderFloat("At Z", &atZ, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetAt(vec3(atX, atY, atZ));
				}

				// Up controls
				float upX = at.x;
				float upY = up.y;
				float upZ = up.z;
				if (ImGui::SliderFloat("Up X", &upX, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetUp(vec3(upX, upY, upZ));
				}
				if (ImGui::SliderFloat("Up Y", &upY, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetUp(vec3(upX, upY, upZ));
				}
				if (ImGui::SliderFloat("Up Z", &upZ, -10.0f, 10.0f))
				{
					scene.GetActiveCamera().SetUp(vec3(upX, upY, upZ));
				}
			}
		}
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);	
		ImGui::Text("viewport width: %d",renderer.GetViewportWidth());
		ImGui::Text("viewport height %d", renderer.GetViewportHeight());
		
		ImGui::End();
	}

	// model editor - opens when the 1st model is loaded
	// handles all of the models transformations and features (bbox, normals, colors)
	static int transformation_type = 0;
	if (show_editor)
	{
				
		ImGui::Begin("Editor", &show_editor);
		bool update_model = FALSE;	// update : 0 - no transformations made | 1 - model\camera was transformed , need to update scene
	
		MeshModel& model = scene.GetActiveModel();
		// transformation_type : 0 - world transformation | 1 - model transformation
		
		ImGui::RadioButton("Local", &transformation_type, 0); ImGui::SameLine();
		ImGui::RadioButton("World", &transformation_type, 1);

		// type : 0 - scale sliders | 1 - translate sliders | 2 - rotation sliders
		static int type = 0;
		ImGui::RadioButton("Scale", &type, 0); ImGui::SameLine();
		ImGui::RadioButton("Translate", &type, 1); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &type, 2); 
		
		bool reset_model_trans = ImGui::Button("Reset model transformations"); ImGui::SameLine();
		bool reset_world_trans = ImGui::Button("Reset world transformations");
		
		if (reset_model_trans)		// reset model transformations
		{
			UIinput.SetModelScale(1, 1, 1);
			
			UIinput.SetModelTranslateX(0);
			UIinput.SetModelTranslateY(0);
			UIinput.SetModelTranslateZ(0);

			UIinput.SetModelRotateX(0);
			UIinput.SetModelRotateY(0);
			UIinput.SetModelRotateZ(0);

			update_model = TRUE;
		}
		
		if (reset_world_trans)		// reset world transformations
		{
			UIinput.SetWorldScale(1, 1, 1);

			UIinput.SetWorldTranslateX(0);
			UIinput.SetWorldTranslateY(0);
			UIinput.SetWorldTranslateZ(0);

			UIinput.SetWorldRotateX(0);
			UIinput.SetWorldRotateY(0);
			UIinput.SetWorldRotateZ(0);

			update_model = TRUE;
		}

		// type = 0 : Scaling
		if (0 == type)
		{
			static float model_uniform_scale = 1;
			static float world_uniform_scale = 1;
			float scale;
			float scale_x;
			float scale_y;
			float scale_z;

			if (0 == transformation_type)	// 0 = model transformation
			{
				scale_x = UIinput.GetModelScale()[0][0];
				scale_y = UIinput.GetModelScale()[1][1];
				scale_z = UIinput.GetModelScale()[2][2];
				scale = model_uniform_scale;
			}
			else							// 1 = world transformation
			{
				scale_x = UIinput.GetWorldScale()[0][0];
				scale_y = UIinput.GetWorldScale()[1][1];
				scale_z = UIinput.GetWorldScale()[2][2];
				scale = world_uniform_scale;
			}
		
			// scaling slides : X,Y,Z and uniform scaling
			if (ImGui::SliderFloat("Scale by X", &scale_x, 0.1f, 10.0f, "Scale = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelScale(scale_x, scale_y, scale_z);	
				else                            // world transformation
					UIinput.SetWorldScale(scale_x, scale_y, scale_z);
				update_model = TRUE;
			}
			if (ImGui::SliderFloat("Scale by Y", &scale_y, 0.1f, 10.0f, "Scale = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelScale(scale_x, scale_y, scale_z);
				else                            // world transformation
					UIinput.SetWorldScale(scale_x, scale_y, scale_z);
				update_model = TRUE;
			}
			if (ImGui::SliderFloat("Scale by Z", &scale_z, 0.1f, 10.0f, "Scale = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelScale(scale_x, scale_y, scale_z);
				else                            // world transformation
					UIinput.SetWorldScale(scale_x, scale_y, scale_z);
				update_model = TRUE;
			}
			if (ImGui::SliderFloat("Uniform Scale", &scale, 0.1f, 10.0f, "Scale = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
				{
					model_uniform_scale = scale;
					UIinput.SetModelScale(scale, scale, scale);
				}
				else                            // world transformation
				{
					world_uniform_scale = scale;
					UIinput.SetWorldScale(scale, scale, scale);
				}
				update_model = TRUE;
			}
				
			
		}

		// type = 1 : Translation
		if (1 == type)
		{
			
			float translae_x = UIinput.GetModelTranslate()[3][0];
			float translae_y = UIinput.GetModelTranslate()[3][1];
			float translae_z = UIinput.GetModelTranslate()[3][2];

			if (0 == transformation_type)	// 0 = model transformation
			{
				translae_x = UIinput.GetModelTranslate()[3][0];
				translae_y = UIinput.GetModelTranslate()[3][1];
				translae_z = UIinput.GetModelTranslate()[3][2];
			}
			else							// 1 = world transformation
			{
				translae_x = UIinput.GetWorldTranslate()[3][0];
				translae_y = UIinput.GetWorldTranslate()[3][1];
				translae_z = UIinput.GetWorldTranslate()[3][2];
			}
			

			if (ImGui::SliderFloat("Translate on X", &translae_x, -10.0f, 10.0f, "Translate = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelTranslateX(translae_x);
				else                            // world transformation
					UIinput.SetWorldTranslateX(translae_x);
				update_model = TRUE;
			}
			if (ImGui::SliderFloat("Translate on Y", &translae_y, -10.0f, 10.0f, "Translate = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelTranslateY(translae_y);
				else                            // world transformation
					UIinput.SetWorldTranslateY(translae_y);
				update_model = TRUE;
			}
			if (ImGui::SliderFloat("Translate on Z", &translae_z, -20.0f, 5.0f, "Translate = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelTranslateZ(translae_z);
				else                            // world transformation
					UIinput.SetWorldTranslateZ(translae_z);
				update_model = TRUE;
			}
		}

		// type = 2 : Rotating
		if (2 == type )
		{
			// Sliders of rotations
			float angle_x;
			float angle_y;
			float angle_z;
			
			if (0 == transformation_type)	// 0 = model transformation
			{
				angle_x = UIinput.GetModelAngleX();
				angle_y = UIinput.GetModelAngleY();
				angle_z = UIinput.GetModelAngleZ();
			}
			else							// 1 = world transformation
			{
				angle_x = UIinput.GetWorldAngleX();
				angle_y = UIinput.GetWorldAngleY();
				angle_z = UIinput.GetWorldAngleZ();
			}
			
			// rotate around X axis
			if (ImGui::SliderFloat("Rotate by X", &angle_x, -180.0f, 180.0f, "Angle = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelRotateX(angle_x);
				else                            // world transformation
					UIinput.SetWorldRotateX(angle_x);
				update_model = TRUE;
				model.first_mult_ = X_FIRST;
			}
			
			// rotate around Y axis
			if (ImGui::SliderFloat("Rotate by Y", &angle_y, -180.0f, 180.0f, "Angle = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelRotateY(angle_y);
				else                            // world transformation
					UIinput.SetWorldRotateY(angle_y);
				update_model = TRUE;
				model.first_mult_ = Y_FIRST;
			}
			
			// rotate around Z axis
			if (ImGui::SliderFloat("Rotate by Z", &angle_z, -180.0f, 180.0f, "Angle = %.3f"))
			{
				if (0 == transformation_type)	// model transformation
					UIinput.SetModelRotateZ(angle_z);
				else                            // world transformation
					UIinput.SetWorldRotateZ(angle_z);
				update_model = TRUE;
				model.first_mult_ = Z_FIRST;
			}
		}
		

		// model color
		static glm::vec4 model_color = model.GetModelColor();
		if (ImGui::ColorEdit3("Model Color", (float*)&model_color))
			scene.GetActiveModel().SetModelColor(model_color);
	


		
		if (update_model && scene.GetModelCount())
			scene.GetActiveModel().UpdateTransformation(UIinput);

		ImGui::Checkbox("Wirmframe", &renderer.wirmframe);
		if (renderer.wirmframe)
		{
			ImGui::ColorEdit3("Wirmframe Color", (float*)&renderer.wirmframe_color);
		}


		ImGui::End();

	}

	// lighting
	if (scene.GetLightCount())
	{
		ImGui::Begin("Light Control");
		int lightIndex = scene.GetActiveLightIndex();
		Light& active_light = *scene.GetLight(lightIndex);
		if (ImGui::ListBox("Lights", &lightIndex, Utils::convertStringVectorToCharArray(lights), (int)lights.size())) {
			scene.SetActiveLightIndex(lightIndex);
			Light& active_light = *scene.GetLight(lightIndex);
		}

		int lightType = active_light.getType();
		// light type
		{
			bool typeChange = false;

			typeChange |= ImGui::RadioButton("Parallel", &lightType, 0); ImGui::SameLine();
			typeChange |= ImGui::RadioButton("Point", &lightType, 1);
			if (typeChange) {
				active_light.setType(lightType);
			}
		}


		static glm::vec4 lightColor = active_light.getIntensity();
		if (ImGui::ColorEdit3("Intensity", (float*)&lightColor)) {
			active_light.setIntensity(lightColor);
		}

		if (lightType == 1) {
			static float translation[3] = { active_light.getDirection()[0], active_light.getDirection()[1], active_light.getDirection()[2] };
			if (ImGui::SliderFloat("X translation", &translation[0], -10.0f, 10.0f)) {
				active_light.setXYZ(translation);
			}
			if (ImGui::SliderFloat("Y translation", &translation[1], -10.0f, 10.0f)) {
				active_light.setXYZ(translation);
			}
			if (ImGui::SliderFloat("Z translation", &translation[2], -10.0f, 10.0f)) {
				active_light.setXYZ(translation);
			}
		}
		if (lightType == 0) {
			static float direction[3] = { active_light.getLightPos()[0], active_light.getLightPos()[1], active_light.getLightPos()[2] };
			if (ImGui::InputFloat3("Light Direction", direction, 2)) {
				active_light.setDirection(glm::vec3(direction[0], direction[1], direction[2]));
			}
		}

		MeshModel& active_model = scene.GetActiveModel();
		ImGui::ColorEdit3("Ambient Color", (float*)&active_model.ambient);
		ImGui::ColorEdit3("Specular Color", (float*)&active_model.specular);
		ImGui::ColorEdit3("Diffuse Color", (float*)&active_model.diffuse);
		ImGui::SliderFloat("Ambient level", &(active_model.k1), 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse level", &(active_model.k2), 0.0f, 1.0f);
		ImGui::SliderFloat("Specular level", &(active_model.k3), 0.0f, 1.0f);
		ImGui::SliderInt("Shining level", &(active_model.k4), 1, 50);


		ImGui::End();
	}
	
}