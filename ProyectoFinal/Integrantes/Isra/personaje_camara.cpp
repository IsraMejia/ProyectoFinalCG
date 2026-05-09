#include "personaje_camara.h"
#include <algorithm>  // Para std::max y std::min

ThirdPersonCamera::ThirdPersonCamera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	
	yaw = 0.0f;      // Yaw del personaje (no de la cámara)
	pitch = 0.0f;    // Pitch de la cámara (vertical)
	
	moveSpeed = 0.3f;
	turnSpeed = 0.5f;
	
	// Offset de la camara: 1 unidad atras, 1 unidad arriba
	offsetDistance = 1.0f;
	offsetHeight = 1.0f;
	
	lastUpdateTime = 0.0;
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Initialize(float startYaw, float startPitch,
	float startMoveSpeed, float startTurnSpeed)
{
	yaw = startYaw;
	pitch = startPitch;
	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	
	// Inicializar tiempo
	lastUpdateTime = glfwGetTime();
	
	UpdateCameraVectors();
}

void ThirdPersonCamera::Update(bool* keys, float xChange, float yChange,
	float deltaTime, const glm::vec3& characterPosition)
{
	// Usar tiempo real de GLFW (independiente del deltaTime del main)
	double currentTime = glfwGetTime();
	
	// Primera vez que se llama
	if (lastUpdateTime == 0.0)
	{
		lastUpdateTime = currentTime;
		return;
	}
	
	// Calcular deltaTime real
	double realDeltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;
	
	// ========== CONTROL DEL MOUSE ==========
	// Mouse horizontal: actualiza el YAW del personaje (rotación en Y)
	// Invertimos xChange para que mouse izquierda = giro izquierda
	xChange *= turnSpeed;
	yaw -= xChange;  // Restamos en lugar de sumar para invertir
	
	// Mouse vertical: actualiza el PITCH de la cámara (mirar arriba/abajo)
	yChange *= turnSpeed;
	pitch += yChange;
	
	// Limitar pitch para evitar gimbal lock
	if (pitch > 45.0f)  // Limitar a 45 grados arriba
	{
		pitch = 45.0f;
	}
	if (pitch < -45.0f)  // Limitar a 45 grados abajo
	{
		pitch = -45.0f;
	}
	
	// ========== POSICIONAMIENTO DE LA CAMARA ==========
	// La cámara está anclada a la espalda del personaje
	// Calcular offset rotado por el yaw del personaje
	
	// Vector hacia adelante del personaje (basado en su yaw)
	// Invertimos la fórmula para que coincida con la rotación del modelo
	float yawRadians = glm::radians(yaw);
	glm::vec3 characterForward(-sin(yawRadians), 0.0f, -cos(yawRadians));
	
	// La cámara está DETRÁS, así que usamos el vector hacia atrás
	glm::vec3 characterBack = -characterForward;
	
	// Posición de la cámara: detrás y arriba del personaje
	position = characterPosition + characterBack * offsetDistance;
	position.y += offsetHeight;
	
	// ========== OBJETIVO DE LA CAMARA (LOOKAT) ==========
	// La cámara mira hacia un punto adelante del personaje
	// ajustado por el pitch (para mirar arriba/abajo)
	glm::vec3 lookAtTarget = characterPosition;
	lookAtTarget.y += 0.5f;  // Mirar al centro del personaje (altura del torso)
	
	// Ajustar el target con el pitch
	float pitchRadians = glm::radians(pitch);
	lookAtTarget += characterForward * cos(pitchRadians) * 2.0f;  // Punto adelante del personaje
	lookAtTarget.y += sin(pitchRadians) * 2.0f;
	
	// Calcular el vector front desde la posición de la cámara al target
	front = glm::normalize(lookAtTarget - position);
	
	// Actualizar vectores right y up
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 ThirdPersonCamera::CalculateViewMatrix() const
{
	// La cámara mira desde su posición hacia adelante
	return glm::lookAt(position, position + front, up);
}

glm::vec3 ThirdPersonCamera::GetPosition() const
{
	return position;
}

glm::vec3 ThirdPersonCamera::GetDirection() const
{
	return glm::normalize(front);
}

void ThirdPersonCamera::UpdateCameraVectors()
{
	// Este método ya no se usa en el nuevo sistema
	// Los vectores se calculan en Update()
}
