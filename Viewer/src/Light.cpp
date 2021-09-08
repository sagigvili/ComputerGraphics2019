#include "Light.h"

Light::Light(int _type, std::string name)
{
	type = _type;
	direction = glm::vec4(1.0f, 0.0f, 10.0f, 1.0f);
	intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pos = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
}

Light::~Light()
{
}

glm::vec4 Light::getDirection() const
{
	return glm::vec4(direction, 1.0f);
}

int Light::getType() const
{
	return type;
}

void Light::setType(int _type)
{
	type = _type;
}

std::string Light::getLightName()
{
	if (type) {
		return "Point";
	}
	return "Parallel";
}

void Light::setIntensity(const glm::vec4& _intensity)
{
	intensity = _intensity;
}

const glm::vec4& Light::getIntensity() const
{
	return intensity;
}

glm::vec4 Light::getLightPos() const
{
	return pos;
}

void Light::setDirection(glm::vec3 _direction) {
	direction = _direction;
}

void Light::setXYZ(float* _pos)
{
	pos.x = _pos[0];
	pos.y = _pos[1];
	pos.z = _pos[2];
}