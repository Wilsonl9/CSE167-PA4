#include "SpotLight.h"

SpotLight::SpotLight()
{
	cutoff = 10;
	totalRot = glm::mat4(1.0f);
	x = 0;
	y = 0;
	z = 0;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	this->x = position.x;
	this->y = position.y;
	this->z = position.z;
	this->position = position;
	this->direction = direction;
	cutoff = 10;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	totalRot = glm::mat4(1.0f);
}

SpotLight::SpotLight(const SpotLight& other)
{
	this->x = other.position.x;
	this->y = other.position.y;
	this->z = other.position.z;
	this->position = other.position;
	this->direction = other.direction;
	cutoff = other.cutoff;
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->specular = other.specular;
	this->constant = other.constant;
	this->linear = other.linear;
	this->quadratic = other.quadratic;
	totalRot = other.totalRot;
}

SpotLight::~SpotLight()
{
}

SpotLight SpotLight::operator=(const SpotLight& other)
{
	this->x = other.position.x;
	this->y = other.position.y;
	this->z = other.position.z;
	this->position = other.position;
	this->direction = other.direction;
	cutoff = other.cutoff;
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->specular = other.specular;
	this->constant = other.constant;
	this->linear = other.linear;
	this->quadratic = other.quadratic;
	totalRot = other.totalRot;
	return *this;
}

void SpotLight::translate(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void SpotLight::increaseCutoff(float n)
{
	cutoff += n;
}

void SpotLight::rotate(glm::vec3 rotAxis, float deg)
{
	totalRot = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), rotAxis) * totalRot;
}

void SpotLight::update()
{
	position = totalRot * glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * glm::vec4(0.0f);
}
