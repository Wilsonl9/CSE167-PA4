#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
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
struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	float cutoff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	GLuint uPosition;
	GLuint uDirection;
	GLuint uAmbient;
	GLuint uCutoff;
	GLuint uDiffuse;
	GLuint uSpecular;
	GLuint uConstant;
	GLuint uLinear;
	GLuint uQuadratic;

	glm::mat4 totalRot;
	float x;
	float y;
	float z;

	SpotLight();
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	SpotLight(const SpotLight& other);
	~SpotLight();
	SpotLight SpotLight::operator=(const SpotLight& other);

	void translate(float x, float y, float z);
	void increaseCutoff(float n);
	void rotate(glm::vec3 rotAxis, float deg);
	void update();
};

#endif
