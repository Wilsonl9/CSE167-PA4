#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#ifdef __APPLE__
// If modern OpenGL replace gl.h with gl3.h
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Cube.h"
#include "OBJObject.h"
#include "shader.h"
#include "DirLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SkyBox.h"
#include "shader_m.h"
#include "camera.h"
#include "Transform.h"
#include "Node.h"
#include "Geometry.h"
#include "Curve.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	//myRasterizer
	static void switchDisplay();
	static void clearBuffer();
	static void drawPoint(int x, int y, float z, int pointSize, float r, float g, float b);

private:
	struct Color    // generic color class
	{
		float r, g, b;  // red, green, blue
	};
	static bool myRasterizer;
	static float* pixels;
	static float* zBuffer;
	static bool mouseRotate;
	static bool mouseTranslate;
	static glm::vec3 lastMousePosRotate;
	static glm::vec3 lastMousePosTranslate;
	static bool phong;
	static struct PointLight pointLight;
	static struct PointLight* toDisplayPointLight;
	static int pointCount;
	static struct DirLight dirLight;
	static struct DirLight* toDisplayDirLight;
	static struct SpotLight spotLight;
	static struct SpotLight* toDisplaySpotLight;
	static int spotCount;

	static Shader* skyboxShaderProgram;
	static SkyBox* skybox;
	static Transform* sceneGraph;

	struct Robot
	{
		Transform* headTransform;
		Transform* bodyTransform;
		Transform* rightArmTransform;
		Transform* leftArmTransform;
		Transform* rightLegTransform;
		Transform* leftLegTransform;
	};

	static struct Robot robot;

	static glm::vec3* curvePoints;
	static glm::mat4* curveConstants;
	static Curve* curves;
	static Shader* curveShaderProgram;

	static glm::vec3 trackballMapping(GLFWwindow* window);

	static glm::mat4 animateRight(glm::mat4 M);
	static glm::mat4 animateLeft(glm::mat4 M);

	static void curveConstantCalc();
	static Curve* createCurve();
	static glm::mat4 animateMarch(glm::mat4 M);
};


#endif
