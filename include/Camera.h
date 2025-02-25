#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

enum class PROJ_TYPE {ORTHOGRAPHIC, PERSPECTIVE};

struct proj 
{
	proj()
	{
		type = PROJ_TYPE::PERSPECTIVE;
		screenWidth = 800.0f;
		screenHeight = 600.0f;
		fov = radians(45.0f);
		near = 0.1f;
		far = 100.0f;
	}

	PROJ_TYPE type;
	float screenWidth;
	float screenHeight;
	float fov;
	float near;
	float far;
};

#pragma once
class Camera
{
public:
	vec3 position;
	vec3 direction;
	proj projection;
	float speed;

	Camera();
	Camera(const float x, const float y, const float z);

	vec3 up() const;
	vec3 forward() const;
	vec3 right() const;

	void rotate(float yaw, float pitch);

	mat4 lookAt(); // return the transformation matrix in according to the current direction
	mat4 lookAt(const vec3 vector);
	mat4 lookAt(const float x, const float y, const float z);

	mat4 move(const vec3 vector);
	mat4 move(const float x, const float y, const float z);

	
};

