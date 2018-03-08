#include "DirLight.h"

DirLight::DirLight()
{

}

DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

DirLight::DirLight(const DirLight& other)
{
	this->direction = other.direction;
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->specular = other.specular;
}

DirLight::~DirLight()
{
}

void DirLight::turn(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	direction = glm::vec3(glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0f));

}
