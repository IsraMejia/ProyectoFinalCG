#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>
#include "../../dependencias/Camera.h"
#include <iostream>
#include <iomanip>

class CameraPositionTracker
{
public:
	CameraPositionTracker();
	~CameraPositionTracker();

	// Inicializa la camara con posicion inicial
	void Initialize(glm::vec3 startPosition, glm::vec3 startUp, 
		GLfloat startYaw, GLfloat startPitch, 
		GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	// Actualiza la camara y detecta movimiento
	void Update(bool* keys, GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

	// Obtener la camara interna
	Camera* GetCamera();

private:
	Camera camera;
	glm::vec3 lastPosition;
	GLfloat lastYaw;
	GLfloat lastPitch;
	GLfloat timeSinceLastMove;
	bool hasMovedSinceLastPrint;
	const GLfloat PRINT_DELAY = 0.5f; // 0.5 segundos

	void PrintCameraInfo();
	bool HasCameraMoved();
};
