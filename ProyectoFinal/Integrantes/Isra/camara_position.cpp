#include "camara_position.h"

CameraPositionTracker::CameraPositionTracker()
{
	lastPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	lastYaw = 0.0f;
	lastPitch = 0.0f;
	timeSinceLastMove = 0.0f;
	hasMovedSinceLastPrint = false;
}

CameraPositionTracker::~CameraPositionTracker()
{
}

void CameraPositionTracker::Initialize(glm::vec3 startPosition, glm::vec3 startUp,
	GLfloat startYaw, GLfloat startPitch,
	GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	camera = Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed);
	lastPosition = startPosition;
	lastYaw = startYaw;
	lastPitch = startPitch;
	timeSinceLastMove = 0.0f;
	hasMovedSinceLastPrint = false;

	// Imprimir posicion inicial
	std::cout << "\n========================================" << std::endl;
	std::cout << "POSICION INICIAL DE LA CAMARA" << std::endl;
	std::cout << "========================================" << std::endl;
	PrintCameraInfo();
}

void CameraPositionTracker::Update(bool* keys, GLfloat xChange, GLfloat yChange, GLfloat deltaTime)
{
	// Guardar posicion antes del movimiento
	glm::vec3 positionBefore = camera.getCameraPosition();

	// Actualizar camara con controles
	camera.keyControl(keys, deltaTime);
	camera.mouseControl(xChange, yChange);

	// Verificar si hubo movimiento
	if (HasCameraMoved())
	{
		timeSinceLastMove = 0.0f;
		hasMovedSinceLastPrint = true;
		lastPosition = camera.getCameraPosition();
	}
	else
	{
		// Acumular tiempo sin movimiento
		timeSinceLastMove += deltaTime;

		// Si ha pasado el tiempo suficiente y hubo movimiento previo, imprimir
		if (timeSinceLastMove >= PRINT_DELAY && hasMovedSinceLastPrint)
		{
			std::cout << "\n========================================" << std::endl;
			std::cout << "CAMARA DETENIDA - POSICION ACTUAL" << std::endl;
			std::cout << "========================================" << std::endl;
			PrintCameraInfo();
			hasMovedSinceLastPrint = false;
		}
	}
}

Camera* CameraPositionTracker::GetCamera()
{
	return &camera;
}

void CameraPositionTracker::PrintCameraInfo()
{
	glm::vec3 pos = camera.getCameraPosition();
	glm::vec3 dir = camera.getCameraDirection();

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Posicion:" << std::endl;
	std::cout << "  glm::vec3(" << pos.x << "f, " << pos.y << "f, " << pos.z << "f)" << std::endl;
	std::cout << "\nDireccion (front):" << std::endl;
	std::cout << "  X: " << dir.x << ", Y: " << dir.y << ", Z: " << dir.z << std::endl;
	std::cout << "\nPara usar en codigo:" << std::endl;
	std::cout << "  position = glm::vec3(" << pos.x << "f, " << pos.y << "f, " << pos.z << "f);" << std::endl;
	std::cout << "========================================\n" << std::endl;
}

bool CameraPositionTracker::HasCameraMoved()
{
	glm::vec3 currentPos = camera.getCameraPosition();
	
	// Comparar con tolerancia pequeña para evitar problemas de precision flotante
	const float EPSILON = 0.001f;
	
	bool moved = (glm::length(currentPos - lastPosition) > EPSILON);
	
	return moved;
}
