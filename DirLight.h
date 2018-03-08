#ifndef DIRLIGHT_H
#define DIRLIGHT_H
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	GLuint uDirection;
	GLuint uAmbient;
	GLuint uDiffuse;
	GLuint uSpecular;

	float angle;

	DirLight();
	DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	DirLight(const DirLight& other);
	~DirLight();

	void turn(float deg);
};

#endif
