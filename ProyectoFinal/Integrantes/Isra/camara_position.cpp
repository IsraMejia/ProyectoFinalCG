#include "camara_position.h"

CameraPositionTracker::CameraPositionTracker()
{
	lastPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	lastYaw = 0.0f;
	lastPitch = 0.0f;
	timeSinceLastMove = 0.0f;
	hasMovedSinceLastPrint = false;
	enabled = true;  // Habilitado por defecto
}

CameraPositionTracker::~CameraPositionTracker()
{
	// Guardar posicion al cerrar
	SavePosition();
}

void CameraPositionTracker::Initialize(glm::vec3 startPosition, glm::vec3 startUp,
	GLfloat startYaw, GLfloat startPitch,
	GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	// Guardar parametros para uso posterior
	this->startUp = startUp;
	this->moveSpeed = startMoveSpeed;
	this->turnSpeed = startTurnSpeed;

	// Intentar cargar posicion guardada
	glm::vec3 loadedPosition;
	GLfloat loadedYaw, loadedPitch;
	bool loaded = LoadPosition(loadedPosition, loadedYaw, loadedPitch);

	if (loaded)
	{
		// Usar posicion cargada
		camera = Camera(loadedPosition, startUp, loadedYaw, loadedPitch, startMoveSpeed, startTurnSpeed);
		lastPosition = loadedPosition;
		lastYaw = loadedYaw;
		lastPitch = loadedPitch;

		std::cout << "\n========================================" << std::endl;
		std::cout << "POSICION CARGADA DESDE ARCHIVO" << std::endl;
		std::cout << "========================================" << std::endl;
		PrintCameraInfo();
	}
	else
	{
		// Usar posicion inicial por defecto
		camera = Camera(startPosition, startUp, startYaw, startPitch, startMoveSpeed, startTurnSpeed);
		lastPosition = startPosition;
		lastYaw = startYaw;
		lastPitch = startPitch;

		std::cout << "\n========================================" << std::endl;
		std::cout << "POSICION INICIAL DE LA CAMARA" << std::endl;
		std::cout << "========================================" << std::endl;
		PrintCameraInfo();
	}

	timeSinceLastMove = 0.0f;
	hasMovedSinceLastPrint = false;
}

void CameraPositionTracker::Update(bool* keys, GLfloat xChange, GLfloat yChange, GLfloat deltaTime)
{
	// Si el tracker está deshabilitado, solo actualizar la cámara sin tracking
	if (!enabled)
	{
		camera.keyControl(keys, deltaTime);
		camera.mouseControl(xChange, yChange);
		return;
	}

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

		// Si ha pasado el tiempo suficiente y hubo movimiento previo, imprimir y guardar
		if (timeSinceLastMove >= PRINT_DELAY && hasMovedSinceLastPrint)
		{
			std::cout << "\n========================================" << std::endl;
			std::cout << "CAMARA DETENIDA - POSICION ACTUAL" << std::endl;
			std::cout << "========================================" << std::endl;
			PrintCameraInfo();
			SavePosition();  // Guardar automaticamente
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

void CameraPositionTracker::SavePosition()
{
	std::ofstream file(SAVE_FILE);
	if (file.is_open())
	{
		glm::vec3 pos = camera.getCameraPosition();
		
		// Guardar posicion y orientacion
		// Nota: Como no tenemos acceso directo a yaw y pitch, guardamos solo la posicion
		// En la proxima carga, la orientacion sera la por defecto
		file << pos.x << " " << pos.y << " " << pos.z << std::endl;
		
		file.close();
		std::cout << "[INFO] Posicion guardada en " << SAVE_FILE << std::endl;
	}
	else
	{
		std::cout << "[ERROR] No se pudo guardar la posicion en " << SAVE_FILE << std::endl;
	}
}

bool CameraPositionTracker::LoadPosition(glm::vec3& outPosition, GLfloat& outYaw, GLfloat& outPitch)
{
	std::ifstream file(SAVE_FILE);
	if (file.is_open())
	{
		float x, y, z;
		if (file >> x >> y >> z)
		{
			outPosition = glm::vec3(x, y, z);
			// Usar orientacion por defecto (se puede mejorar guardando yaw y pitch tambien)
			outYaw = -60.0f;
			outPitch = 0.0f;
			
			file.close();
			std::cout << "[INFO] Posicion cargada desde " << SAVE_FILE << std::endl;
			return true;
		}
		file.close();
	}
	
	return false;
}


void CameraPositionTracker::SetEnabled(bool enable)
{
	enabled = enable;
	
	if (!enabled)
	{
		// Al deshabilitar, resetear el estado de tracking
		timeSinceLastMove = 0.0f;
		hasMovedSinceLastPrint = false;
	}
}

bool CameraPositionTracker::IsEnabled() const
{
	return enabled;
}
