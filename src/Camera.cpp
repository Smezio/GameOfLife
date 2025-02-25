#include "Camera.h"

Camera::Camera() 
	: position{ vec3(0.0f, 0.0f, 0.0f) }, 
	direction{ vec3(0.0f, 0.0f, -1.0f) - position },
	projection{ proj() },
	speed{1.0f}
{}

Camera::Camera(const float x, const float y, const float z) 
	: position{ vec3(x,y,z) },
	projection{ proj() },
	speed{ 1.0f }
{
	if (position == vec3())
		direction = vec3(0.0f, 0.0f, -1.0f);
	else
		direction = position - vec3(0.0f, 0.0f, 0.0f);
}


vec3 Camera::up() const
{
	return vec3(0.0f, 1.0f, 0.0f);
}

vec3 Camera::forward() const
{
	return vec3(0.0f, 0.0f, -1.0f);
}

vec3 Camera::right() const
{
	return glm::normalize(cross(forward(), up()));
}

void Camera::rotate(float yaw, float pitch)
{
	direction.x = cos(radians(yaw)) * cos(radians(pitch));
	direction.y = sin(radians(pitch));
	direction.z = sin(radians(yaw)) * cos(radians(pitch));
}

mat4 Camera::lookAt()
{
	direction = position + forward();
	return glm::lookAt(position, direction, up());
}

mat4 Camera::lookAt(const vec3 vector)
{
	direction = vector;
	return glm::lookAt(position, direction, up());
}

mat4 Camera::lookAt(const float x, const float y, const float z)
{
	direction = vec3(x, y, z);
	return glm::lookAt(position, direction, up());
}

mat4 Camera::move(const vec3 vector)
{
	return mat4();
}
mat4 Camera::move(const float x, const float y, const float z)
{
	return mat4();
}