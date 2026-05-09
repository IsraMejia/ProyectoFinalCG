#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

// Clase que encapsula una camara en tercera persona jerarquica
// vinculada a un personaje 3D (Jefe Maestro)
class ThirdPersonCamera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	// Inicializa la camara con parametros de control
	void Initialize(float startYaw, float startPitch, 
		float startMoveSpeed, float startTurnSpeed);

	// Actualiza la camara basandose en el input del usuario
	// keys: array de teclas presionadas
	// xChange, yChange: movimiento del mouse
	// deltaTime: tiempo transcurrido desde el ultimo frame
	// characterPosition: posicion actual del personaje (para seguirlo)
	void Update(bool* keys, float xChange, float yChange, 
		float deltaTime, const glm::vec3& characterPosition);

	// Calcula y retorna la matriz de vista para renderizado
	glm::mat4 CalculateViewMatrix() const;

	// Obtiene la posicion actual de la camara en el mundo
	glm::vec3 GetPosition() const;

	// Obtiene la direccion hacia donde apunta la camara (normalizada)
	glm::vec3 GetDirection() const;

	// Obtiene el angulo yaw actual (rotacion horizontal)
	float GetYaw() const { return yaw; }

	// Obtiene el angulo pitch actual (rotacion vertical)
	float GetPitch() const { return pitch; }

private:
	// Parametros de la camara
	glm::vec3 position;      // Posicion actual de la camara en el mundo
	glm::vec3 front;         // Vector direccion hacia donde apunta
	glm::vec3 up;            // Vector up de la camara
	glm::vec3 right;         // Vector right de la camara
	glm::vec3 worldUp;       // Vector up del mundo (0,1,0)

	// Angulos de rotacion
	float yaw;               // Rotacion horizontal (grados)
	float pitch;             // Rotacion vertical (grados)

	// Parametros de control
	float moveSpeed;         // Velocidad de movimiento del personaje
	float turnSpeed;         // Sensibilidad del mouse

	// Offset de la camara respecto al personaje
	float offsetDistance;    // Distancia detras del personaje
	float offsetHeight;      // Altura sobre el personaje

	// Sistema de tiempo independiente (como en personaje_principal.cpp)
	double lastUpdateTime;   // Ultimo tiempo de actualizacion

	// Actualiza los vectores front, right, up basandose en yaw y pitch
	void UpdateCameraVectors();

	// Maneja el movimiento del personaje con las flechas
	void HandleCharacterMovement(bool* keys, float deltaTime, glm::vec3& characterPosition);
};
