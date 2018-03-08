#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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
#include <vector>
#include <cstring>
#include <iostream>
#include <cfloat>
#include <algorithm>
#include <string>
#include "Window.h"
#include "DirLight.h"
#include "PointLight.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

class OBJObject
{
private:
/*	struct face
	{
		int vertexIndex[3];
		int textureIndex[3];
		int normalIndex[3];
		face(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3)
		{
			vertexIndex[0] = v1;
			vertexIndex[1] = v2;
			vertexIndex[2] = v3;
			textureIndex[0] = t1;
			textureIndex[1] = t2;
			textureIndex[2] = t3;
			normalIndex[0] = n1;
			normalIndex[1] = n2;
			normalIndex[2] = n3;
		}
	};*/
	std::vector<unsigned int> indices;
	std::vector<unsigned int> nindices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
//	std::vector<face> faces;
	glm::mat4 toWorld;
	glm::mat4 totalRot;
	glm::mat4 rotOffset;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;
	GLuint NBO;
	GLuint uProjection, uModel, uView;
	GLuint uLightColor;
	GLuint uLightPos;
	GLuint uViewPos;
	GLuint uPhong;
	GLuint uPointCount;
	GLuint uSpotCount;
	GLuint clearLight;
	int direction;
	float scalePointAmount;
	float scaleAmount;
	float baseX;
	float baseY;
	float baseZ;
	float x;
	float y;
	float z;
	bool transFirst;
	void applyTranslate();
	void applyScale();
	void centerAndResize();
	void applyRotate();
	
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		GLuint uAmbient;
		GLuint uDiffuse;
		GLuint uSpecular;
		GLuint uShininess;
	};
	struct Material material;

public:
	float angle;

	OBJObject(const char* filepath);
	~OBJObject();

	void parse(const char* filepath);
	void draw(GLuint shaderProgram, glm::vec3 viewPos, bool phong, struct DirLight* dirLight, struct PointLight* pointLight, int pointCount, struct SpotLight* spotLight, int spotCount);
	void update();
	void spin(float deg);
	void setBaseCoordinate(float x, float y, float z);
	void setDirection(int direction);
	void scalePoint(float amount);
	void resetScalePoint();
	void translate(float x, float y, float z);
	void resetTranslate();
	void scale(float amount);
	void resetScale();
	glm::vec3 getVertex(int ver);
	glm::vec3 getNormal(int norm);
	float getScaleAmount();
	void rasterize(int width, int height);
	void rotate(glm::vec3 rotAxis, float deg);
	void setMatrial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	void setRotOffset();

};

#endif