#include "animKFcamara.h"
#include <iostream>

Animation_Camera::Animation_Camera()
{
	// Initialize camera position
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	// Initialize camera vectors
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = worldUp;
	right = glm::vec3(1.0f, 0.0f, 0.0f);

	// Initialize follow parameters
	followDistance = 30.0f;  // Distancia más alejada para mejor vista
	followHeight = 10.0f;     // Más alto para mejor perspectiva
	followAngle = 0.0f;      // Start behind the model
	followModeActive = false;

	// Smooth camera movement
	smoothSpeed = 2.0f;
}

Animation_Camera::~Animation_Camera()
{
	// Nothing to clean up
}

void Animation_Camera::enableFollowMode(bool enable)
{
	// Solo imprimir si el estado cambia
	if (followModeActive != enable)
	{
		followModeActive = enable;

		if (enable)
		{
			std::cout << "Animation camera: Follow mode ENABLED" << std::endl;
		}
		else
		{
			std::cout << "Animation camera: Follow mode DISABLED" << std::endl;
		}
	}
}

void Animation_Camera::setFollowTarget(const glm::vec3& targetPos)
{
	targetPosition = targetPos;
}

void Animation_Camera::setFollowDistance(float distance)
{
	followDistance = distance;
}

void Animation_Camera::setFollowHeight(float height)
{
	followHeight = height;
}

void Animation_Camera::update(float deltaTime)
{
	if (followModeActive)
	{
		calculateFollowPosition();
		updateCameraVectors();
	}
}

glm::mat4 Animation_Camera::getViewMatrix() const
{
	return glm::lookAt(position, targetPosition, up);
}

glm::vec3 Animation_Camera::getPosition() const
{
	return position;
}

glm::vec3 Animation_Camera::getDirection() const
{
	return front;
}

void Animation_Camera::syncWithMainCamera(Camera& mainCamera)
{
	// Copy relevant state from main camera when entering follow mode
	position = mainCamera.getCameraPosition();
	// Note: We don't copy direction since we'll be looking at the target
}

void Animation_Camera::transferControlToMainCamera(Camera& mainCamera)
{
	// Transfer camera state back to main camera when exiting follow mode
	// The main camera will resume from where the animation camera left off
	// Note: This is optional - main camera can also just resume from its last state
}

void Animation_Camera::calculateFollowPosition()
{
	// Calculate camera position behind and above the target
	// Using spherical coordinates for smooth orbiting

	// Calculate offset from target
	float offsetX = followDistance * sin(followAngle);
	float offsetZ = followDistance * cos(followAngle);

	// Set camera position
	glm::vec3 desiredPosition;
	desiredPosition.x = targetPosition.x + offsetX;
	desiredPosition.y = targetPosition.y + followHeight;
	desiredPosition.z = targetPosition.z + offsetZ;

	// Smooth camera movement (lerp)
	// position = glm::mix(position, desiredPosition, smoothSpeed * deltaTime);
	// For now, just set directly for immediate response
	position = desiredPosition;
}

void Animation_Camera::updateCameraVectors()
{
	// Calculate direction vector from camera to target
	front = glm::normalize(targetPosition - position);

	// Calculate right and up vectors
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
