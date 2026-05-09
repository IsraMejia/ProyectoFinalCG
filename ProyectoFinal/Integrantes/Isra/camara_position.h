#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>
#include "../../dependencias/Camera.h"
#include "personaje_camara.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

// Enum para identificar el tipo de camara activa
enum class CameraMode
{
	DEVELOPMENT,      // Camara de desarrollo (tecla 1)
	THIRD_PERSON      // Camara en tercera persona (tecla 2)
};

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
	// characterPosition: posicion del personaje (para camara en tercera persona)
	void Update(bool* keys, GLfloat xChange, GLfloat yChange, GLfloat deltaTime,
		const glm::vec3& characterPosition = glm::vec3(0.0f));

	// Obtener la camara interna (camara de desarrollo)
	Camera* GetCamera();

	// Obtener la camara en tercera persona
	ThirdPersonCamera* GetThirdPersonCamera();

	// Obtener matriz de vista de la camara activa
	glm::mat4 GetViewMatrix();

	// Obtener posicion de la camara activa
	glm::vec3 GetCameraPosition();

	// Obtener direccion de la camara activa
	glm::vec3 GetCameraDirection();

	// Obtener modo de camara actual
	CameraMode GetCameraMode() const { return currentMode; }

	// Guardar posicion actual al archivo
	void SavePosition();

	// Habilitar/deshabilitar el tracker (para evitar conflictos con sistema de keyframes)
	void SetEnabled(bool enabled);
	bool IsEnabled() const;

private:
	Camera camera;                          // Camara de desarrollo (WASD)
	ThirdPersonCamera thirdPersonCamera;    // Camara en tercera persona (flechas)
	CameraMode currentMode;                 // Modo de camara actual
	
	glm::vec3 lastPosition;
	GLfloat lastYaw;
	GLfloat lastPitch;
	GLfloat timeSinceLastMove;
	bool hasMovedSinceLastPrint;
	bool enabled;  // Flag para habilitar/deshabilitar el tracker
	
	// Control de teclas de cambio de camara
	bool key1Pressed;
	bool key2Pressed;
	
	const GLfloat PRINT_DELAY = 0.5f; // 0.5 segundos
	const std::string SAVE_FILE = "camera_position.txt";
	const std::string SAVE_FILE_THIRD_PERSON = "camera_position_personaje.txt";

	// Parametros de inicializacion guardados
	glm::vec3 startUp;
	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void PrintCameraInfo();
	bool HasCameraMoved();
	bool LoadPosition(glm::vec3& outPosition, GLfloat& outYaw, GLfloat& outPitch);
};
