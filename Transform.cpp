#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1.0f);
	totalRot = glm::mat4(1.0f);
	totalTrans = glm::mat4(1.0f);
	totalScale = glm::mat4(1.0f);
}

Transform::Transform(glm::mat4 M)
{
	this->M = M;
	totalRot = glm::mat4(1.0f);
	totalTrans = glm::mat4(1.0f);
	totalScale = glm::mat4(1.0f);
}

Transform::~Transform()
{
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (it; it != end; ++it)
	{
		delete (*it);
	}
}

void Transform::draw(glm::mat4 C, Shader* shaderProgram, Camera camera, glm::mat4 projection, bool phong)
{
	glm::mat4 totalTransformation = C * totalTrans * totalRot * totalScale * M;
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (it; it != end; ++it)
	{
		(*it)->draw(totalTransformation, shaderProgram, camera, projection, phong);
	}
}

void Transform::update()
{
	if (animate)
	{
		M = animate(M);
	}
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (it; it != end; ++it)
	{
		(*it)->update();
	}
}

void Transform::addChild(Node* node)
{
	children.push_back(node);
}

void Transform::removeChild(Node * node)
{
	children.remove(node);
}

void Transform::translate(float x, float y, float z)
{
	totalTrans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * totalTrans;
}

void Transform::scale(float amount)
{
	totalScale = glm::scale(glm::mat4(1.0f), glm::vec3(pow(1.2f, amount))) * totalScale;
}

void Transform::scale(float x, float y, float z)
{
	totalScale = glm::scale(glm::mat4(1.0f), glm::vec3(pow(1.2f, x), pow(1.2f, y), pow(1.2f, z))) * totalScale;
}

void Transform::rotate(glm::vec3 rotAxis, float deg)
{
	totalRot = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), rotAxis) * totalRot;
}

void Transform::resetTranslate()
{
	totalTrans = glm::mat4(1.0f);
}

void Transform::resetScale()
{
	totalScale = glm::mat4(1.0f);
}

void Transform::setAnimation(glm::mat4 (*animate)(glm::mat4 M))
{
	this->animate = animate;
}
