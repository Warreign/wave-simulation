#include "camera.h"
#include "object.h"

Camera* Camera::active = nullptr;

Camera::Camera(glm::vec3 position, glm::vec3 direction, float nearPlane, float farPlane, float captureAngle, float movementSpeed)
	: position(position), direction(direction), up(glm::vec3(0.0f, 1.0f, 1.0f)),
	yaw(0.0f), pitch(0.0f), sensitivity(1.5f), speed(movementSpeed), freeMode(false),
	fovAngle(captureAngle), nearPlane(nearPlane), farPlane(farPlane),
	lastActive(nullptr)
{
	updateMatrices();
}


void Camera::makeActive() {
	if (!active)
	{
		active = this;
		return;
	}
	if (active->freeMode)
		active->toggleFreeMode();
	active = this;
}

/*
*	Free motion functions
*/

void Camera::freeModeMotionCallback(int x, int y)
{
	float deltaYaw = float(GLUT_WIDTH / 2 - x) * 0.025 * active->sensitivity;
	float deltaPitch = float(GLUT_HEIGHT / 2 - y) * 0.025 * active->sensitivity;
	active->rotateView(deltaYaw, deltaPitch);
	glutWarpPointer(GLUT_WIDTH / 2, GLUT_HEIGHT / 2);
}

void Camera::toggleFreeMode()
{
	if (freeMode == true)
	{
		freeMode = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		active = lastActive;
		glutPassiveMotionFunc(nullptr);
	}
	else
	{ // freeMode == false
		makeActive();
		freeMode = true;
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(GLUT_WIDTH / 2, GLUT_HEIGHT / 2);
		lastActive = active;
		glutPassiveMotionFunc(freeModeMotionCallback);
	}
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

void Camera::moveLeft()
{
	if (freeMode)
	{
		glm::vec3 newPos = position - (glm::normalize(glm::cross(direction, up)) * (speed / refreshRate));
		position = newPos;
	}
}

void Camera::moveRight()
{
	if (freeMode)
	{
		glm::vec3 newPos = position + (glm::normalize(glm::cross(direction, up)) * (speed / refreshRate));
		position = newPos;
	}
}

void Camera::moveForward()
{
	if (freeMode)
	{
		glm::vec3 newPos = position + (glm::normalize(direction) * (speed / refreshRate));
		position = newPos;
	}
}

void Camera::moveBackward()
{
	if (freeMode)
	{
		glm::vec3 newPos = position - (glm::normalize(direction) * (speed / refreshRate));
		position = newPos;
	}
}

/*
*	View functions
*/

void Camera::updateMatrices()
{
	this->view = glm::lookAt(position, position + direction, up);
	this->projection = glm::perspective(glm::radians(fovAngle), float(GLUT_WIDTH) / float(GLUT_HEIGHT), nearPlane, farPlane);
}

glm::vec3 Camera::rayCast(glm::vec2 screenPosition) const
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float fx = (screenPosition.x / width) * 2.0f - 1.0f;
	float fy = ((height - screenPosition.y) / height) * 2.0f - 1.0f;

	//std::cout << "x: " << fx << " " << "y: " << fy << std::endl;

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

void TW_CALL Camera::SetSpeedCB(const void* value, void* clientData)
{
	auto camera = static_cast<Camera*>(clientData);
	camera->speed = *static_cast<const float*>(value);
}

void TW_CALL Camera::GetSpeedCB(void* value, void* clientData)
{
	auto camera = static_cast<const Camera*>(clientData);
	*static_cast<float*>(value) = camera->speed;
}

void TW_CALL Camera::SetFovCB(const void* value, void* clientData)
{
	auto camera = static_cast<Camera*>(clientData);
	camera->fovAngle = *static_cast<const float*>(value);
}

void TW_CALL Camera::GetFovCB(void* value, void* clientData)
{
	auto camera = static_cast<const Camera*>(clientData);
	*static_cast<float*>(value) = camera->fovAngle;
}

void TW_CALL Camera::SetSensitivityCB(const void* value, void* clientData)
{
	auto camera = static_cast<Camera*>(clientData);
	camera->sensitivity = *static_cast<const float*>(value);
}

void TW_CALL Camera::GetSensitivityCB(void* value, void* clientData)
{
	auto camera = static_cast<const Camera*>(clientData);
	*static_cast<float*>(value) = camera->sensitivity;
}

void TW_CALL Camera::SetFarPlaneCB(const void* value, void* clientData)
{
	auto camera = static_cast<Camera*>(clientData);
	camera->farPlane = *static_cast<const float*>(value);
}

void TW_CALL Camera::GetFarPlaneCB(void* value, void* clientData)
{
	auto camera = static_cast<const Camera*>(clientData);
	*static_cast<float*>(value) = camera->farPlane;
}

void TW_CALL Camera::ToggleFreeModeCB(void* p)
{
	auto camera = static_cast<Camera*>(p);
	camera->toggleFreeMode();
}


