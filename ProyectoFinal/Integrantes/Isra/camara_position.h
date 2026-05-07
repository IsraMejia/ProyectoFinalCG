#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>
#include "../../dependencias/Camera.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

class CameraPositionTracker
{
public:
	CameraPositionTracker();
	~CameraPositionTracker();

	// Inicializa la camara con posicion inicial (o carga desde archivo si existe)
	void Initialize(glm::vec3 startPosition, glm::vec3 startUp, 
		GLfloat startYaw, GLfloat startPitch, 
		GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	// Actualiza la camara y detecta movimiento
	void Update(bool* keys, GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

	// Obtener la camara interna
	Camera* GetCamera();

	// Guardar posicion actual al archivo
	void SavePosition();

	// Habilitar/deshabilitar el tracker (para evitar conflictos con sistema de keyframes)
	void SetEnabled(bool enabled);
	bool IsEnabled() const;

private:
	Camera camera;
	glm::vec3 lastPosition;
	GLfloat lastYaw;
	GLfloat lastPitch;
	GLfloat timeSinceLastMove;
	bool hasMovedSinceLastPrint;
	bool enabled;  // Flag para habilitar/deshabilitar el tracker
	const GLfloat PRINT_DELAY = 0.5f; // 0.5 segundos
	const std::string SAVE_FILE = "camera_position.txt";

	// Parametros de inicializacion guardados
	glm::vec3 startUp;
	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void PrintCameraInfo();
	bool HasCameraMoved();
	bool LoadPosition(glm::vec3& outPosition, GLfloat& outYaw, GLfloat& outPitch);
};
