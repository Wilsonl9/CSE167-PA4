#ifndef SKYBOX_H
#define SKYBOX_H

#ifdef __APPLE__
// If modern OpenGL replace gl.h with gl3.h
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include "shader.h"
#include "shader_m.h"
#include "camera.h"

class SkyBox
{
private:
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;

public:
	SkyBox(std::vector<std::string> faces, Shader* skyboxShaderProgram);
	~SkyBox();
	unsigned int loadCubemap(std::vector<std::string> faces);
	void draw(Shader* skyboxShader, Camera camera, int width, int height);
};

#endif // !SKYBOX_H
