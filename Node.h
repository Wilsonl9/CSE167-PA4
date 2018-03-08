#ifndef NODE_H
#define NODE_H

#include<glm/glm.hpp>
#include"shader_m.h"
#include"camera.h"

class Node
{
public:
	Node();
	~Node();
	virtual void draw(glm::mat4 C, Shader* shaderProgram, Camera camera, glm::mat4 projection, bool phong) = 0;
	virtual void update() = 0;

protected:

private:

};

#endif
