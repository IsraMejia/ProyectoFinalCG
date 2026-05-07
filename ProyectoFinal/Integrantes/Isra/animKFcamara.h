#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Camera.h"

// Animation camera that follows a target model during recording mode
class Animation_Camera {
public:
	Animation_Camera();
	~Animation_Camera();

	// Follow mode control
	void enableFollowMode(bool enable);
	void setFollowTarget(const glm::vec3& targetPosition);
	void setFollowDistance(float distance);
	void setFollowHeight(float height);

	// Camera updates
	void update(float deltaTime);
	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;

	// Integration with main camera
	void syncWithMainCamera(Camera& mainCamera);
	void transferControlToMainCamera(Camera& mainCamera);

private:
	glm::vec3 position;
	glm::vec3 targetPosition;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float followDistance;
	float followHeight;
	float followAngle;  // Angle around target
	bool followModeActive;

	// Smooth camera movement
	float smoothSpeed;

	void calculateFollowPosition();
	void updateCameraVectors();
};
