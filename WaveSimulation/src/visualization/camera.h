#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <tuple>


//#define GLUT_WIDTH glutGet(GLUT_WINDOW_WIDTH)
//#define GLUT_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)


class Camera
{
public:

	Camera(glm::vec3 position, glm::vec3 direction, float nearPlane, float farPlane, float captureAngle, float movementSpeed);

public:
	static Camera& get() { return *s_active; }
	static void freeModeMotionCallback(int x, int y);

	Camera* lastActive;

	// Projection parameters
	float fovAngle;
	float farPlane;
	float nearPlane;

	// Free mode parameters
	float yaw;
	float pitch;
	float speed;
	float sensitivity;

	// View matrices
	glm::mat4 view;
	glm::mat4 projection;

public:
	static int refreshRate;
	bool freeMode;

	glm::vec3 up;
	glm::vec3 direction;
	glm::vec3 position;
	float targetDistance;

	void makeActive();

	void setProjectionParameters(float angle, float nearPlane, float farPlane);

	void toggleFreeMode();
	void moveLeft();
	void moveRight();
	void moveForward();
	void moveBackward();
	void rotateView(float dyaw, float dpitch);

	void updateMatrices();
	glm::vec3 rayCast(glm::vec2 screenPosition) const;
	glm::vec3 intersectPlane(glm::vec3 planeNormal, glm::vec3 planeOrigin, glm::vec2 screenPosition) const;

	const glm::mat4& viewMatrix() const;
	const glm::mat4& projectMatrix() const;

private:
	static Camera* s_active;
};

#endif