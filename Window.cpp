#include "Window.h"

const char* window_title = "GLFW Starter Project";
//Cube cube(5.0f);

SkyBox* skybox;

int Window::width;
int Window::height;

bool Window::myRasterizer = false;
float* Window::pixels = NULL;
float* Window::zBuffer = NULL;

Shader* shaderProgram;
// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 50.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::mat4 Window::P;
glm::mat4 Window::V;
bool Window::mouseRotate;
bool Window::mouseTranslate;
glm::vec3 Window::lastMousePosRotate;
glm::vec3 Window::lastMousePosTranslate;
bool Window::phong;
struct PointLight Window::pointLight;
struct PointLight* Window::toDisplayPointLight;
int Window::pointCount;
struct DirLight Window::dirLight;
struct DirLight* Window::toDisplayDirLight;
struct SpotLight Window::spotLight;
struct SpotLight* Window::toDisplaySpotLight;
int Window::spotCount;

#define SKYBOX_VERTEX_SHADER_PATH "../skyBoxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyBoxShader.frag"
Shader* Window::skyboxShaderProgram;
SkyBox* Window::skybox;
Camera camera(cam_pos);
Transform* Window::sceneGraph;

Geometry* androidHead;
Geometry* androidBody;
Geometry* androidLimb;

struct Window::Robot Window::robot;

glm::vec3* Window::curvePoints;
glm::mat4* Window::curveConstants;

#define CURVE_VERTEX_SHADER_PATH "../curveShader.vert"
#define CURVE_FRAGMENT_SHADER_PATH "../curveShader.frag"
Curve* Window::curves;
Shader* Window::curveShaderProgram;
unsigned int LINESEG = 1101;

Transform* tree;
TreeGenerator* Window::treeGenerator;

void Window::initialize_objects()
{
	shaderProgram = new Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	mouseRotate = false;
	mouseTranslate = false;
	phong = false;

	skyboxShaderProgram = new Shader(SKYBOX_VERTEX_SHADER_PATH,SKYBOX_FRAGMENT_SHADER_PATH);
	std::vector<std::string> faces
	{
		"../mp_deception/deception_pass_ft.tga",
			"../mp_deception/deception_pass_bk.tga",
			"../mp_deception/deception_pass_up.tga",
			"../mp_deception/deception_pass_dn.tga",
			"../mp_deception/deception_pass_rt.tga",
			"../mp_deception/deception_pass_lf.tga"
	};
	std::vector<std::string> faces2
	{
		"../mp_bleak/bleak-outlook_rt.tga",
		"../mp_bleak/bleak-outlook_lf.tga",
		"../mp_bleak/bleak-outlook_up.tga",
		"../mp_bleak/bleak-outlook_dn.tga",
		"../mp_bleak/bleak-outlook_ft.tga",
		"../mp_bleak/bleak-outlook_bk.tga"
	};
	skybox = new SkyBox(faces, skyboxShaderProgram);
	sceneGraph = new Transform();
	
	treeGenerator = new TreeGenerator();
	tree = treeGenerator->generateTree(2);
	sceneGraph->addChild(tree);
	/*Transform* robotNode = new Transform();
	androidHead = new Geometry();
        androidHead->init("c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA3\\CSE167StarterCode-master\\robot-parts\\head.obj");
	androidBody = new Geometry();
        androidBody->init("c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA3\\CSE167StarterCode-master\\robot-parts\\body.obj");
	androidLimb = new Geometry();
        androidLimb->init("c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA3\\CSE167StarterCode-master\\robot-parts\\limb.obj");
	//adding the body
	Transform* newChild = new Transform();
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->addChild(androidBody);
	robot.bodyTransform = newChild;
	robotNode->addChild(newChild);
	//adding the head
	newChild = new Transform();
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->translate(0.0f, 1.6f, 0.0f);
	newChild->addChild(androidHead);
	robot.headTransform = newChild;
	robotNode->addChild(newChild);
	//adding the left arm
	newChild = new Transform();
	newChild->scale(0, 0, -1.3);
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->translate(2.0f, 0.0f, 0.0f);
	newChild->addChild(androidLimb);
	robot.leftArmTransform = newChild;
	robotNode->addChild(newChild);
	//adding the right arm
	newChild = new Transform();
	newChild->scale(0, 0, -1.3);
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->translate(-0.8f, 0.0f, 0.0f);
	robot.rightArmTransform = newChild;
	newChild->addChild(androidLimb);
	robotNode->addChild(newChild);
	//adding the left leg
	newChild = new Transform();
	newChild->scale(0, 0, -3);
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->translate(0.1f, -1.7f, 0.0f);
	newChild->addChild(androidLimb);
	robot.leftLegTransform = newChild;
	robotNode->addChild(newChild);
	//adding the right leg
	newChild = new Transform();
	newChild->scale(0, 0, -3);
	newChild->rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90);
	newChild->translate(1.1f, -1.7f, 0.0f);
	newChild->addChild(androidLimb);
	robot.rightLegTransform = newChild;
	robotNode->addChild(newChild);
	robot.rightArmTransform->setAnimation(animateRight);
	robot.leftArmTransform->setAnimation(animateLeft);
	robot.rightLegTransform->setAnimation(animateLeft);
	robot.leftLegTransform->setAnimation(animateRight);

	//duplicating the robot
	//sceneGraph->addChild(robotNode);
	for(int i = 0; false && i < 5; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			newChild = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f * (float)i - 8.0f, 0.0f, -4.0f * (float)j + 8.0f)));
			newChild->addChild(robotNode);
			sceneGraph->addChild(newChild);
		}
	}*/

	//bezier curve
	curvePoints = new glm::vec3[20];
	//curvePoints[0] = glm::vec3(10.0f, 15.0f, -2.0f);
	curvePoints[0] = glm::vec3(17.5f, -5.0f, 2.5f);
	curvePoints[1] = glm::vec3(15.0f, 0.0f, 4.0f);
	curvePoints[2] = glm::vec3(-12.0f, 0.0f, 5.0f);
	//curvePoints[3] = glm::vec3(-14.0f, -7.0f, 20.0f);
	curvePoints[3] = glm::vec3(-6.5f, 1.5f, 1.0f);

	curvePoints[4] = glm::vec3(-6.5f, 1.5f, 1.0f);
	//curvePoints[4] = glm::vec3(-14.0f, -7.0f, 20.0f);
	curvePoints[5] = glm::vec3(-1.0f, 3.0f, -3.0f);
	curvePoints[6] = glm::vec3(10.0f, -9.0f, -18.0f);
	//curvePoints[7] = glm::vec3(-19.0f, 15.0f/*20.0f*/, 17.0f);
	curvePoints[7] = glm::vec3(4.0f, -10.5f, 1.0f);

	curvePoints[8] = glm::vec3(4.0f, -10.5f, 1.0f);
	//curvePoints[8] = glm::vec3(-19.0f, 15.0f/*20.0f*/, 17.0f);
	curvePoints[9] = glm::vec3(-2.0f, -12.0f, 20.0f);
	curvePoints[10] = glm::vec3(-6.0f, -18.0f, 14.0f);
	//curvePoints[11] = glm::vec3(-16.0f, -18.0f, -7.0f);
	curvePoints[11] = glm::vec3(2.5f, -6.5f, 4.5f);

	curvePoints[12] = glm::vec3(2.5f, -6.5f, 4.5f);
	//curvePoints[12] = glm::vec3(-16.0f, -18.0f, -7.0f);
	curvePoints[13] = glm::vec3(11.0f, 5.0f, -5.0f);
	curvePoints[14] = glm::vec3(4.0f, -6.0f, -15.0f);
	//curvePoints[15] = glm::vec3(-19.0f, 12.0f, -11.0f);
	curvePoints[15] = glm::vec3(6.0f, -1.5f, -11.5f);

	curvePoints[16] = glm::vec3(6.0f, -1.5f, -11.5f);
	//curvePoints[16] = glm::vec3(-19.0f, 12.0f, -11.0f);
	curvePoints[17] = glm::vec3(8.0f, 3.0f, -8.0f);
	curvePoints[18] = glm::vec3(20.0f, -10.0f, 1.0f);
	curvePoints[19] = glm::vec3(17.5f, -5.0f, 2.5f);
	//curvePoints[19] = glm::vec3(10.0f, 15.0f, -2.0f);

	curveConstants = new glm::mat4[5];
	curveConstantCalc();
	curves = createCurve();
	for(int i = 0; i < 5; ++i)
	{
		curves[i].init();
	}

	curveShaderProgram = new Shader(CURVE_VERTEX_SHADER_PATH, CURVE_FRAGMENT_SHADER_PATH);
	sceneGraph->setAnimation(animateMarch);
}

void Window::clean_up()
{
	delete shaderProgram;
	delete skybox;
	delete skyboxShaderProgram;
	delete curvePoints;
	delete curveConstants;
	delete curves;
	delete sceneGraph;
	delete treeGenerator;
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		//P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		P = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 1000.0f);
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		V = camera.GetViewMatrix();
	}
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.

	//currObj->update();
	P = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 1000.0f);
	sceneGraph->update();

	dirLight.turn(0.001f);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	shaderProgram->use();

	// Render the cube
	if(mouseRotate)
	{
		float cos;
		float angle;
		glm::vec3 newPos = trackballMapping(window);
		glm::vec3 rotAxis = glm::cross(lastMousePosRotate, newPos);
		cos = (dot(newPos, lastMousePosRotate) / (glm::length(newPos) * glm::length(lastMousePosRotate)));
		angle = glm::acos(cos);
		if(cos < 1)
		{
			//rotateNode->rotate(rotAxis, angle * 50);
			camera.rotate(rotAxis, angle * 50);
		}
		lastMousePosRotate = newPos;
	}
	if(mouseTranslate)
	{
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		glm::vec3 newPos = glm::vec3(x, y, 0.0f);
		glm::vec3 dist = newPos - lastMousePosTranslate;
		sceneGraph->translate(dist.x / 20.0f, dist.y / -20.0f, 0);
		lastMousePosTranslate = newPos;
	}
	if(sceneGraph)
	{
		sceneGraph->draw(glm::mat4(1.0f), shaderProgram, camera, P, phong);
	}
	skybox->draw(skyboxShaderProgram, camera, width, height);
	for(int i = 0; i < 5; ++i)
	{
		curves[i].draw(glm::mat4(1.0f), curveShaderProgram, camera, P, phong);
	}
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		switch (key)
		{
		case GLFW_KEY_P:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
//				currObj->scalePoint(1);
			}
			else
			{
//				currObj->scalePoint(-1);
			}
			break;
		}
		case GLFW_KEY_A:
		{
			sceneGraph->translate(-1, 0, 0);
			break;
		}
		case GLFW_KEY_D:
		{
			sceneGraph->translate(1, 0, 0);
			break;
		}
		case GLFW_KEY_W:
		{
			sceneGraph->translate(0, 1, 0);
			break;
		}
		case GLFW_KEY_S:
		{
			sceneGraph->translate(0, -1, 0);
			break;
		}
		case GLFW_KEY_Z:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				sceneGraph->translate(0, 0, 1);
			}
			else
			{
				sceneGraph->translate(0, 0, -1);
			}
			break;
		}
		case GLFW_KEY_C:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				sceneGraph->scale(1);
			}
			else
			{
				sceneGraph->scale(-1);
			}
			break;
		}
		case GLFW_KEY_R:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
//				currObj->resetScalePoint();
				sceneGraph->resetScale();
			}
			else
			{
				sceneGraph->resetTranslate();
			}
			break;
		}
		case GLFW_KEY_M:
		{
			switchDisplay();
			break;
		}
		case GLFW_KEY_F1:
		{
//			currObj = dragon;
			break;
		}
		case GLFW_KEY_F2:
		{
//			currObj = bunny;
			break;
		}
		case GLFW_KEY_F3:
		{
//			currObj = bear;
			break;
		}
		case GLFW_KEY_N:
		{
			phong = !phong;
			break;
		}
		case GLFW_KEY_1:
		{
			toDisplayPointLight = NULL;
			pointCount = 0;
			toDisplaySpotLight = NULL;
			spotCount = 0;
			break;
		}
		case GLFW_KEY_2:
		{
			toDisplayPointLight = &pointLight;
			pointCount = 1;
			toDisplaySpotLight = NULL;
			spotCount = 0;
			break;
		}
		case GLFW_KEY_3:
		{
			toDisplayPointLight = NULL;
			pointCount = 0;
			toDisplaySpotLight = &spotLight;
			spotCount = 1;
			break;
		}
		case GLFW_KEY_4:
		{
			if (toDisplayDirLight)
			{
				toDisplayDirLight = NULL;
			}
			else
			{
				toDisplayDirLight = &dirLight;
			}
			break;
		}
		case GLFW_KEY_G:
		{
			sceneGraph->removeChild(tree);
			delete tree;
			tree = treeGenerator->generateTree(5);
			sceneGraph->addChild(tree);
		}
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			lastMousePosRotate = trackballMapping(window);
			mouseRotate = true;
		}
		if (button == GLFW_MOUSE_BUTTON_2)
		{
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);
			lastMousePosTranslate = glm::vec3(x, y, 0.0f);
			//lastMousePos = trackballMapping(window);
			mouseTranslate = true;
		}
	}
	else if(action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			mouseRotate = false;
		}
		if (button == GLFW_MOUSE_BUTTON_2)
		{
			mouseTranslate = false;
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void Window::switchDisplay()
{
	myRasterizer = !myRasterizer;
	return;
}

// Clear frame buffer
void Window::clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<width*height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
		zBuffer[i] = FLT_MAX;
	}
}

// Draw a point into the frame buffer
void Window::drawPoint(int x, int y, float z, int pointSize, float r, float g, float b)
{
	int xCord;
	int yCord;
	for (int i = 0; i < pointSize; ++i)
	{
		for (int j = 0; j < pointSize; ++j)
		{
			yCord = (y + j);
			xCord = (x + i);
			if (yCord < 0 || yCord > height - 1 || xCord < 0 || xCord > width - 1)
			{
				continue;
			}
			int offset = yCord * width * 3 + xCord * 3;
			if (z < zBuffer[yCord * width + xCord])
			{
				pixels[offset] = r;
				pixels[offset + 1] = g;
				pixels[offset + 2] = b;
				zBuffer[yCord * width + xCord] = z;
			}
		}
	}
}

glm::vec3 Window::trackballMapping(GLFWwindow* window)
{
	double x;
	double y;
	float d;
	glfwGetCursorPos(window, &x, &y);
	glm::vec3 newPos((2.0f * x - width) / width, (height - 2.0f * y) / height, 0.0f);
	d = glm::length(newPos);
	d = (d < 1.0) ? d : 1.0;
	newPos.z = sqrt(1.001 - d * d);
	newPos = glm::normalize(newPos);
	return newPos;
}
glm::mat4 Window::animateRight(glm::mat4 M)
{
	static bool rightForward = true;
	static int rightForwardCount = 120;
	float deg = 0.5f;
	if (rightForwardCount >= 240)
	{
		rightForwardCount = 0;
		rightForward = !rightForward;
	}
	++rightForwardCount;
	if (!rightForward)
	{
		deg *= -1;
	}
	M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -.9f)) * M;
	M = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * M;
	M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.9f)) * M;
	return M;
}

glm::mat4 Window::animateLeft(glm::mat4 M)
{
	static bool leftForward = false;
	static int leftForwardCount = 120;
	float deg = 0.5f;
	if (leftForwardCount >= 240)
	{
		leftForwardCount = 0;
		leftForward = !leftForward;
	}
	++leftForwardCount;
	if (!leftForward)
	{
		deg *= -1;
	}
	M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.9f)) * M;
	M = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * M;
	M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.9f)) * M;
	return M;
}

void Window::curveConstantCalc()
{
	glm::mat4 Bernstein(-1.0f, 3.0f, -3.0f, 1.0f,
						3.0f, -6.0f, 3.0f, 0.0f,
						-3.0f, 3.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < 5; ++i)
	{
		curveConstants[i] = glm::mat4(glm::vec4(curvePoints[(i * 4) + 0], 1), glm::vec4(curvePoints[(i * 4) + 1], 1), glm::vec4(curvePoints[(i * 4) + 2], 1), glm::vec4(curvePoints[(i * 4) + 3], 1));
		curveConstants[i] = curveConstants[i] * Bernstein;
	}
}

Curve* Window::createCurve()
{
	Curve* curves = new Curve[5];
	int N = LINESEG;
	for(int curve = 0; curve < 5; ++curve)
	{
		std::vector<glm::vec3>* vertices = curves[curve].getVertices();
		for(int i = 0; i < N; ++i)
		{
			vertices->push_back(glm::vec3(curveConstants[curve] * glm::vec4((float)(i * i * i) / (float)(N * N * N), (float)(i * i) / (float)(N * N), (float)i / (float)N, 1.0f)));
		}
	}
	return curves;
}

glm::mat4 Window::animateMarch(glm::mat4 M)
{
	static unsigned int marchCount = 0;
	if (marchCount >= (LINESEG - 1) * 5)
	{
		marchCount = 0;
	}
	std::vector<glm::vec3>* vertices = curves[marchCount / (LINESEG - 1)].getVertices();
	glm::mat4 nextM = glm::translate(glm::mat4(1.0f), (*vertices)[marchCount % (LINESEG - 1)]);
	++marchCount;
	return nextM;
}
