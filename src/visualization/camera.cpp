#include "camera.h"

#include "visualization/objects/object.h"
#include "application/window.h"
#include "application/application.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

Camera* Camera::s_active = nullptr;
int Camera::refreshRate = 30;

Camera::Camera(glm::vec3 position, glm::vec3 direction, float nearPlane, float farPlane, float captureAngle, float movementSpeed)
	: position(position), direction(direction), up(glm::vec3(0.0f, 1.0f, 1.0f)),
	yaw(0.0f), pitch(0.0f), sensitivity(1.5f), speed(movementSpeed), freeMode(false),
	fovAngle(captureAngle), nearPlane(nearPlane), farPlane(farPlane),
	lastActive(nullptr)
{
}


void Camera::makeActive() {
	s_active = this;
}

void Camera::rotateView(float dyaw, float dpitch)
{
	yaw = glm::mod((yaw + dyaw), 360.0f);
	pitch = glm::max(glm::min(pitch + dpitch, 90.0f), -90.0f);

	direction = glm::vec3(
		glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
		glm::sin(glm::radians(pitch)),
		glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
	);

	up = glm::vec3(
		-glm::sin(glm::radians(yaw)) * glm::sin(glm::radians(pitch)),
		glm::cos(glm::radians(pitch)),
		-glm::cos(glm::radians(yaw)) * glm::sin(glm::radians(pitch))
	);
}

void Camera::rotateViewAbs(float yaw, float pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;
	direction = glm::vec3(
		glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
		glm::sin(glm::radians(pitch)),
		glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
	);

	up = glm::vec3(
		-glm::sin(glm::radians(yaw)) * glm::sin(glm::radians(pitch)),
		glm::cos(glm::radians(pitch)),
		-glm::cos(glm::radians(yaw)) * glm::sin(glm::radians(pitch))
	);
}

/*
*	View functions
*/

void Camera::updateMatrices()
{
	this->view = glm::lookAt(position, position + direction, up);

	Application& app = Application::getInstance();
	Window& window = app.getWindow();
	this->projection = glm::perspectiveFov(fovAngle, float(window.getWidth()), float(window.getHeight()), nearPlane, farPlane);
}

glm::vec3 Camera::rayCast(glm::vec2 screenPosition) const
{
	Application& app = Application::getInstance();
	Window& window = app.getWindow();
	int width = window.getWidth();
	int height = window.getHeight();
	float fx = (screenPosition.x / width) * 2.0f - 1.0f;
	float fy = ((height - screenPosition.y) / height) * 2.0f - 1.0f;

	std::cout << "x: " << fx << " " << "y: " << fy << std::endl;

	glm::mat4 V = glm::lookAt(glm::vec3(0), direction, up);
	glm::vec4 worldPos = glm::inverse(projection * V) * glm::vec4(fx, fy, 1.0f, 1.0f);
	return glm::normalize(glm::vec3(worldPos));
}

glm::vec3 Camera::intersectPlane(glm::vec3 planeNormal, glm::vec3 planeOrigin, glm::vec2 screenPosition) const
{
	glm::vec3 dir = rayCast(screenPosition);

	float distance;
	glm::intersectRayPlane(position, dir, planeOrigin, planeNormal, distance);
	return position + dir * distance;
}

void Camera::setProjectionParameters(float angle, float nearPlane, float farPlane)
{
	this->fovAngle = angle;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

const glm::mat4& Camera::viewMatrix() const
{
	return view;
}

const glm::mat4& Camera::projectMatrix() const
{
	return projection;
}

