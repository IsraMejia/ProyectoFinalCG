#include "animacion_KF.h"
#include "camara_position.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <glfw3.h>

// ============================================================================
// Forward declarations for internal classes
// ============================================================================
class Keyframe_Parser;
class Keyframe_Pretty_Printer;

// ============================================================================
// Keyframe_Parser Implementation
// ============================================================================

class Keyframe_Parser {
public:
	bool parseFile(const std::string& filename, Keyframe* keyframes, int& outFrameCount, int maxFrames)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			std::cout << "Warning: Keyframe file not found: " << filename << ". Starting with empty animation." << std::endl;
			outFrameCount = 0;
			return false;
		}

		std::string line;
		int lineNumber = 0;
		int currentKeyframeIndex = -1;
		bool expectingData = false;
		outFrameCount = 0;

		while (std::getline(file, line))
		{
			lineNumber++;

			// Skip empty lines and comments
			if (line.empty() || line[0] == '#')
				continue;

			// Check for keyframe header
			if (line.find("KEYFRAME") == 0)
			{
				if (!parseLine(line, currentKeyframeIndex))
				{
					logError("Invalid keyframe header format", lineNumber);
					continue;
				}

				if (currentKeyframeIndex < 0 || currentKeyframeIndex >= maxFrames)
				{
					logError("Keyframe index out of bounds", lineNumber);
					continue;
				}

				expectingData = true;
			}
			else if (expectingData && currentKeyframeIndex >= 0)
			{
				// Parse transformation data
				if (extractTransformationData(line, keyframes[currentKeyframeIndex]))
				{
					if (currentKeyframeIndex >= outFrameCount)
					{
						outFrameCount = currentKeyframeIndex + 1;
					}
				}
				else
				{
					logError("Invalid transformation data", lineNumber);
				}
				expectingData = false;
			}
		}

		file.close();

		if (outFrameCount > 0)
		{
			std::cout << "Successfully loaded " << outFrameCount << " keyframes from " << filename << std::endl;
			return true;
		}
		else
		{
			std::cout << "Warning: No valid keyframes found in " << filename << std::endl;
			return false;
		}
	}

private:
	bool parseLine(const std::string& line, int& outIndex)
	{
		// Expected format: "KEYFRAME N"
		std::istringstream iss(line);
		std::string keyword;
		int index;

		if (iss >> keyword >> index)
		{
			if (keyword == "KEYFRAME")
			{
				outIndex = index;
				return true;
			}
		}
		return false;
	}

	bool extractTransformationData(const std::string& line, Keyframe& outKeyframe)
	{
		// Expected format: "posX=value posY=value posZ=value rotX=value rotY=value rotZ=value"
		float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
		float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

		posX = parseFloatValue(line, "posX");
		posY = parseFloatValue(line, "posY");
		posZ = parseFloatValue(line, "posZ");
		rotX = parseFloatValue(line, "rotX");
		rotY = parseFloatValue(line, "rotY");
		rotZ = parseFloatValue(line, "rotZ");

		// Store values
		outKeyframe.posX = posX;
		outKeyframe.posY = posY;
		outKeyframe.posZ = posZ;
		outKeyframe.rotX = rotX;
		outKeyframe.rotY = rotY;
		outKeyframe.rotZ = rotZ;

		return true;
	}

	float parseFloatValue(const std::string& str, const std::string& key)
	{
		// Find "key=" in the string
		size_t pos = str.find(key + "=");
		if (pos == std::string::npos)
		{
			return 0.0f;  // Default value if not found
		}

		// Move past "key="
		pos += key.length() + 1;

		// Extract the number
		size_t endPos = str.find(' ', pos);
		std::string valueStr = str.substr(pos, endPos - pos);

		try
		{
			return std::stof(valueStr);
		}
		catch (...)
		{
			return 0.0f;
		}
	}

	void logError(const std::string& message, int lineNumber)
	{
		std::cout << "Warning: " << message << " at line " << lineNumber << ". Using default value." << std::endl;
	}
};

// ============================================================================
// Keyframe_Pretty_Printer Implementation
// ============================================================================

class Keyframe_Pretty_Printer {
public:
	bool writeToFile(const std::string& filename, const Keyframe* keyframes, int frameCount, const std::string& animationName)
	{
		std::ofstream file(filename);
		if (!file.is_open())
		{
			std::cout << "Error: Cannot write to file: " << filename << ". Keyframes not saved." << std::endl;
			return false;
		}

		// Write header
		writeHeader(file, animationName, frameCount);

		// Write all keyframes
		for (int i = 0; i < frameCount; i++)
		{
			writeKeyframe(file, keyframes[i], i);
		}

		file.close();
		std::cout << "Successfully saved " << frameCount << " keyframes to " << filename << std::endl;
		return true;
	}

private:
	void writeHeader(std::ofstream& file, const std::string& animationName, int frameCount)
	{
		file << "# Animation: " << animationName << "\n";
		file << "# Created: " << getCurrentTimestamp() << "\n";
		file << "# Total Keyframes: " << frameCount << "\n";
		file << "\n";
	}

	void writeKeyframe(std::ofstream& file, const Keyframe& kf, int index)
	{
		file << "KEYFRAME " << index << "\n";
		file << "posX=" << formatFloat(kf.posX) << " ";
		file << "posY=" << formatFloat(kf.posY) << " ";
		file << "posZ=" << formatFloat(kf.posZ) << " ";
		file << "rotX=" << formatFloat(kf.rotX) << " ";
		file << "rotY=" << formatFloat(kf.rotY) << " ";
		file << "rotZ=" << formatFloat(kf.rotZ) << "\n";
		file << "\n";
	}

	std::string formatFloat(float value, int precision = 2)
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(precision) << value;
		return oss.str();
	}

	std::string getCurrentTimestamp()
	{
		std::time_t now = std::time(nullptr);
		char buf[100];
		
		#ifdef _WIN32
		struct tm timeinfo;
		localtime_s(&timeinfo, &now);
		std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
		#else
		std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
		#endif
		
		return std::string(buf);
	}
};

// ============================================================================
// Keyframe_System Implementation
// ============================================================================

Keyframe_System::Keyframe_System(int maxKeyframes)
{
	maxFrames = maxKeyframes;
	frameCount = 0;
	currentPlayIndex = 0;
	currentInterpolationStep = 0;
	maxInterpolationSteps = 0;

	recordingMode = false;
	playbackMode = false;
	canSaveFrame = true;  // Start with ability to save

	animationTimer = 0.0f;
	frameRate = 12.0f;  // 12 FPS
	recordingStartTime = 0.0f;
	totalRecordingTime = 0.0f;

	// Initialize current transformation state
	currentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	currentRotX = 0.0f;
	currentRotY = 0.0f;
	currentRotZ = 0.0f;

	// Initialize camera tracker reference
	cameraTracker = nullptr;

	// Initialize keyframe array with default values
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		keyframes[i].posX = 0.0f;
		keyframes[i].posY = 0.0f;
		keyframes[i].posZ = 0.0f;
		keyframes[i].rotX = 0.0f;
		keyframes[i].rotY = 0.0f;
		keyframes[i].rotZ = 0.0f;
		keyframes[i].timestamp = 0.0f;
		keyframes[i].posXInc = 0.0f;
		keyframes[i].posYInc = 0.0f;
		keyframes[i].posZInc = 0.0f;
		keyframes[i].rotXInc = 0.0f;
		keyframes[i].rotYInc = 0.0f;
		keyframes[i].rotZInc = 0.0f;
	}
}

Keyframe_System::~Keyframe_System()
{
	// Nothing to clean up (no dynamic memory)
}

void Keyframe_System::toggleRecordingMode()
{
	recordingMode = !recordingMode;

	if (recordingMode)
	{
		// Entering recording mode
		playbackMode = false;  // Disable playback
		recordingStartTime = (float)glfwGetTime();  // Capturar tiempo de inicio
		
		// Deshabilitar CameraPositionTracker si está configurado
		if (cameraTracker != nullptr)
		{
			cameraTracker->SetEnabled(false);
		}
		
		printRecordingMenu();
	}
	else
	{
		// Exiting recording mode
		totalRecordingTime = (float)glfwGetTime() - recordingStartTime;  // Calcular duración total
		
		// Rehabilitar CameraPositionTracker si está configurado
		if (cameraTracker != nullptr)
		{
			cameraTracker->SetEnabled(true);
		}
		
		std::cout << "\n========================================" << std::endl;
		std::cout << "MODO CAPTURA DESACTIVADO" << std::endl;
		std::cout << "Keyframes capturados: " << frameCount << std::endl;
		std::cout << "Duracion de captura: " << totalRecordingTime << " segundos" << std::endl;
		std::cout << "========================================\n" << std::endl;
	}
}

void Keyframe_System::togglePlaybackMode()
{
	// Prevent playback if not enough keyframes
	if (!playbackMode && frameCount < 2)
	{
		std::cout << "Error: Need at least 2 keyframes to play animation. Current count: " << frameCount << std::endl;
		return;
	}

	// Prevent playback if recording is active
	if (recordingMode)
	{
		std::cout << "Error: Cannot start playback while recording mode is active." << std::endl;
		return;
	}

	playbackMode = !playbackMode;

	if (playbackMode)
	{
		// Starting playback
		std::cout << "\n=== MODO REPRODUCCION INICIADO ===\n";
		std::cout << "Reproduciendo " << frameCount << " keyframes\n";
		std::cout << "Duracion original: " << totalRecordingTime << " segundos\n";
		std::cout << "Duracion objetivo: 60 segundos (loop infinito)\n";
		std::cout << "Frame rate: 12 FPS (720 frames totales)\n" << std::endl;

		// Reset to first keyframe
		currentPlayIndex = 0;
		currentInterpolationStep = 0;
		animationTimer = 0.0f;

		// Set initial transformation from first keyframe
		currentPosition.x = keyframes[0].posX;
		currentPosition.y = keyframes[0].posY;
		currentPosition.z = keyframes[0].posZ;
		currentRotX = keyframes[0].rotX;
		currentRotY = keyframes[0].rotY;
		currentRotZ = keyframes[0].rotZ;

		// SISTEMA DE DISTRIBUCION BASADO EN PORCENTAJES
		// Calcular el porcentaje de posición de cada keyframe en la captura original
		// y mapear ese porcentaje a 60 segundos (720 frames a 12 FPS)
		
		const float TARGET_DURATION = 60.0f;  // 60 segundos
		const float TOTAL_FRAMES = TARGET_DURATION * frameRate;  // 720 frames
		
		std::cout << "=== DISTRIBUCION BASADA EN PORCENTAJES ===\n";
		
		// Calcular frames objetivo para cada keyframe basado en su porcentaje
		int targetFrames[MAX_FRAMES];
		for (int i = 0; i < frameCount; i++)
		{
			// Calcular porcentaje de este keyframe en la captura original
			float percentage = (totalRecordingTime > 0.0f) ? (keyframes[i].timestamp / totalRecordingTime) : 0.0f;
			
			// Mapear ese porcentaje a 720 frames
			targetFrames[i] = (int)(percentage * TOTAL_FRAMES);
			
			std::cout << "Keyframe " << i << ": " 
				<< std::fixed << std::setprecision(2) << (percentage * 100.0f) << "% "
				<< "-> Frame " << targetFrames[i] << " (segundo " << (targetFrames[i] / frameRate) << ")\n";
		}
		
		std::cout << "\n=== CALCULO DE FRAMES POR SEGMENTO ===\n";
		
		int totalFramesCalculated = 0;
		
		// Calcular frames entre keyframes consecutivos
		for (int i = 0; i < frameCount - 1; i++)
		{
			// Frames entre este keyframe y el siguiente
			int framesInSegment = targetFrames[i + 1] - targetFrames[i];
			if (framesInSegment < 1) framesInSegment = 1;
			
			framesPerSegment[i] = framesInSegment;
			totalFramesCalculated += framesInSegment;
			
			// Calcular incrementos para interpolación lineal
			keyframes[i].posXInc = (keyframes[i + 1].posX - keyframes[i].posX) / framesInSegment;
			keyframes[i].posYInc = (keyframes[i + 1].posY - keyframes[i].posY) / framesInSegment;
			keyframes[i].posZInc = (keyframes[i + 1].posZ - keyframes[i].posZ) / framesInSegment;
			keyframes[i].rotXInc = (keyframes[i + 1].rotX - keyframes[i].rotX) / framesInSegment;
			keyframes[i].rotYInc = (keyframes[i + 1].rotY - keyframes[i].rotY) / framesInSegment;
			keyframes[i].rotZInc = (keyframes[i + 1].rotZ - keyframes[i].rotZ) / framesInSegment;
			
			float segmentDuration = framesInSegment / frameRate;
			std::cout << "Segmento " << i << " -> " << (i+1) << ": " 
				<< framesInSegment << " frames (" 
				<< std::fixed << std::setprecision(2) << segmentDuration << "s)\n";
		}
		
		// Loop back: último keyframe al primero (completar los 720 frames)
		if (frameCount > 1)
		{
			int framesInSegment = (int)TOTAL_FRAMES - targetFrames[frameCount - 1];
			if (framesInSegment < 1) framesInSegment = 1;
			
			framesPerSegment[frameCount - 1] = framesInSegment;
			totalFramesCalculated += framesInSegment;
			
			keyframes[frameCount - 1].posXInc = (keyframes[0].posX - keyframes[frameCount - 1].posX) / framesInSegment;
			keyframes[frameCount - 1].posYInc = (keyframes[0].posY - keyframes[frameCount - 1].posY) / framesInSegment;
			keyframes[frameCount - 1].posZInc = (keyframes[0].posZ - keyframes[frameCount - 1].posZ) / framesInSegment;
			keyframes[frameCount - 1].rotXInc = (keyframes[0].rotX - keyframes[frameCount - 1].rotX) / framesInSegment;
			keyframes[frameCount - 1].rotYInc = (keyframes[0].rotY - keyframes[frameCount - 1].rotY) / framesInSegment;
			keyframes[frameCount - 1].rotZInc = (keyframes[0].rotZ - keyframes[frameCount - 1].rotZ) / framesInSegment;
			
			float segmentDuration = framesInSegment / frameRate;
			std::cout << "Segmento loop " << (frameCount-1) << " -> 0: " 
				<< framesInSegment << " frames (" 
				<< std::fixed << std::setprecision(2) << segmentDuration << "s)\n";
		}
		
		std::cout << "\n=== RESUMEN ===\n";
		std::cout << "Total frames calculados: " << totalFramesCalculated << " / " << (int)TOTAL_FRAMES << "\n";
		std::cout << "Duracion total: " << std::fixed << std::setprecision(2) << (totalFramesCalculated / frameRate) << " segundos\n";
		std::cout << "\nAnimacion lista para reproducir!\n" << std::endl;
	}
	else
	{
		// Stopping playback
		std::cout << "\n=== Playback Mode STOPPED ===\n" << std::endl;
	}
}

void Keyframe_System::resetAnimation()
{
	currentPlayIndex = 0;
	currentInterpolationStep = 0;
	playbackMode = false;
	animationTimer = 0.0f;

	// Reset to first keyframe if keyframes exist
	if (frameCount > 0)
	{
		currentPosition.x = keyframes[0].posX;
		currentPosition.y = keyframes[0].posY;
		currentPosition.z = keyframes[0].posZ;
		currentRotX = keyframes[0].rotX;
		currentRotY = keyframes[0].rotY;
		currentRotZ = keyframes[0].rotZ;
	}

	std::cout << "\n=== Animation RESET ===\n";
	std::cout << "Returned to first keyframe. Press SPACE to play again.\n" << std::endl;
}

int Keyframe_System::getKeyframeCount() const
{
	return frameCount;
}

void Keyframe_System::clearKeyframes()
{
	frameCount = 0;
	currentPlayIndex = 0;
	currentInterpolationStep = 0;
	playbackMode = false;
	recordingMode = false;

	std::cout << "All keyframes cleared. Animation reset." << std::endl;
}

void Keyframe_System::saveCurrentKeyframe()
{
	if (!canSaveFrame)
	{
		std::cout << "Presiona 'P' primero para habilitar el guardado del siguiente keyframe." << std::endl;
		return;
	}

	if (frameCount >= maxFrames)
	{
		std::cout << "Advertencia: Capacidad maxima de keyframes (" << maxFrames << ") alcanzada." << std::endl;
		return;
	}

	// El keyframe ya fue copiado en handleRecordingInput
	std::cout << "\n>>> KEYFRAME " << frameCount << " GUARDADO <<<" << std::endl;
	std::cout << "Tiempo: " << std::fixed << std::setprecision(2) << keyframes[frameCount].timestamp << " segundos" << std::endl;
	std::cout << "Posicion: (" << std::fixed << std::setprecision(2) 
		<< keyframes[frameCount].posX << ", " 
		<< keyframes[frameCount].posY << ", " 
		<< keyframes[frameCount].posZ << ")" << std::endl;
	std::cout << "Rotacion Y: " << keyframes[frameCount].rotY << " grados" << std::endl;

	frameCount++;
	canSaveFrame = false;

	std::cout << "\nTotal keyframes: " << frameCount << "/" << maxFrames << std::endl;
	std::cout << "Presiona 'P' para habilitar el siguiente guardado.\n" << std::endl;
	
	// Mostrar menu de nuevo
	printRecordingMenu();
}

bool Keyframe_System::isPlaying() const
{
	return playbackMode;
}

bool Keyframe_System::isRecording() const
{
	return recordingMode;
}

void Keyframe_System::printRecordingMenu()
{
	std::cout << "\n========================================\n";
	std::cout << "=== MODO CAPTURA DE KEYFRAMES ===\n";
	std::cout << "========================================\n";
	std::cout << "CONTROLES DE VUELO:\n";
	std::cout << "  UP Arrow    : Avanzar hacia adelante\n";
	std::cout << "  DOWN Arrow  : Retroceder\n";
	std::cout << "  RIGHT Arrow : Girar a la derecha\n";
	std::cout << "  LEFT Arrow  : Girar a la izquierda\n";
	std::cout << "  N           : Subir (Y+)\n";
	std::cout << "  M           : Bajar (Y-)\n";
	std::cout << "\nGUARDAR KEYFRAMES:\n";
	std::cout << "  P : Habilitar guardado\n";
	std::cout << "  L : Guardar keyframe actual\n";
	std::cout << "\nSALIR:\n";
	std::cout << "  K : Salir del modo captura\n";
	std::cout << "========================================\n";
	std::cout << "Keyframes guardados: " << frameCount << "/" << maxFrames << "\n";
	std::cout << "========================================\n" << std::endl;
}

void Keyframe_System::calculateInterpolation(int fromIndex, int toIndex)
{
	if (maxInterpolationSteps <= 0)
		return;

	// Calculate increments for linear interpolation
	keyframes[fromIndex].posXInc = (keyframes[toIndex].posX - keyframes[fromIndex].posX) / maxInterpolationSteps;
	keyframes[fromIndex].posYInc = (keyframes[toIndex].posY - keyframes[fromIndex].posY) / maxInterpolationSteps;
	keyframes[fromIndex].posZInc = (keyframes[toIndex].posZ - keyframes[fromIndex].posZ) / maxInterpolationSteps;
	keyframes[fromIndex].rotXInc = (keyframes[toIndex].rotX - keyframes[fromIndex].rotX) / maxInterpolationSteps;
	keyframes[fromIndex].rotYInc = (keyframes[toIndex].rotY - keyframes[fromIndex].rotY) / maxInterpolationSteps;
	keyframes[fromIndex].rotZInc = (keyframes[toIndex].rotZ - keyframes[fromIndex].rotZ) / maxInterpolationSteps;
}

// Recording mode input handling implementation
void Keyframe_System::handleRecordingInput(bool* keys, float mouseX, float mouseY, float deltaTime,
	glm::vec3& modelPosition, float& modelRotX, float& modelRotY, float& modelRotZ)
{
	if (!recordingMode)
		return;

	const float moveSpeed = 0.3f;  // Velocidad de movimiento
	const float rotationSpeed = 2.0f;  // Velocidad de rotación
	const float verticalSpeed = 0.2f;  // Velocidad vertical

	// Convertir rotación Y a radianes para cálculos direccionales
	float yawRadians = modelRotY * (3.14159265f / 180.0f);
	
	// Calcular vectores de dirección basados en la rotación Y del modelo
	float forwardX = sin(yawRadians);
	float forwardZ = cos(yawRadians);
	float rightX = cos(yawRadians);
	float rightZ = -sin(yawRadians);

	// CONTROLES DE MOVIMIENTO DIRECCIONAL (basados en rotación Y)
	
	// UP Arrow: Avanzar hacia adelante
	if (keys[GLFW_KEY_UP])
	{
		modelPosition.x += forwardX * moveSpeed;
		modelPosition.z += forwardZ * moveSpeed;
	}

	// DOWN Arrow: Retroceder
	if (keys[GLFW_KEY_DOWN])
	{
		modelPosition.x -= forwardX * moveSpeed;
		modelPosition.z -= forwardZ * moveSpeed;
	}

	// RIGHT Arrow: Girar a la derecha (aumentar rotación Y)
	if (keys[GLFW_KEY_RIGHT])
	{
		modelRotY += rotationSpeed;
	}

	// LEFT Arrow: Girar a la izquierda (disminuir rotación Y)
	if (keys[GLFW_KEY_LEFT])
	{
		modelRotY -= rotationSpeed;
	}

	// CONTROLES VERTICALES
	
	// N: Subir (Y+)
	static bool nKeyPressed = false;
	if (keys[GLFW_KEY_N])
	{
		if (!nKeyPressed)
		{
			nKeyPressed = true;
		}
		modelPosition.y += verticalSpeed;
	}
	else
	{
		nKeyPressed = false;
	}

	// M: Bajar (Y-)
	static bool mKeyPressed = false;
	if (keys[GLFW_KEY_M])
	{
		if (!mKeyPressed)
		{
			mKeyPressed = true;
		}
		modelPosition.y -= verticalSpeed;
	}
	else
	{
		mKeyPressed = false;
	}

	// CONTROLES DE GUARDADO DE KEYFRAMES
	
	static bool lKeyPressed = false;
	if (keys[GLFW_KEY_L] && !lKeyPressed)
	{
		lKeyPressed = true;
		
		// Copiar transformación actual del modelo al buffer de keyframes
		if (frameCount < maxFrames && canSaveFrame)
		{
			// Capturar timestamp actual (tiempo desde que se inició la grabación)
			float currentTime = (float)glfwGetTime();
			float elapsedTime = currentTime - recordingStartTime;
			
			keyframes[frameCount].posX = modelPosition.x;
			keyframes[frameCount].posY = modelPosition.y;
			keyframes[frameCount].posZ = modelPosition.z;
			keyframes[frameCount].rotX = modelRotX;
			keyframes[frameCount].rotY = modelRotY;
			keyframes[frameCount].rotZ = modelRotZ;
			keyframes[frameCount].timestamp = elapsedTime;  // Guardar timestamp
		}
		
		saveCurrentKeyframe();
	}
	else if (!keys[GLFW_KEY_L])
	{
		lKeyPressed = false;
	}

	// P: Habilitar guardado
	static bool pKeyPressed = false;
	if (keys[GLFW_KEY_P] && !pKeyPressed)
	{
		pKeyPressed = true;
		canSaveFrame = true;
		std::cout << "\n[LISTO] Presiona 'L' para guardar el siguiente keyframe.\n" << std::endl;
	}
	else if (!keys[GLFW_KEY_P])
	{
		pKeyPressed = false;
	}
}

void Keyframe_System::updatePlayback(float deltaTime)
{
	if (!playbackMode || frameCount < 2)
		return;

	// Usar tiempo real en lugar de deltaTime del programa
	// Esto asegura que la animación vaya a 12 FPS independientemente del framerate del programa
	static double lastUpdateTime = 0.0;
	double currentTime = glfwGetTime();
	
	// Primera vez que se llama después de iniciar playback
	if (lastUpdateTime == 0.0)
	{
		lastUpdateTime = currentTime;
		return;
	}
	
	double realDeltaTime = currentTime - lastUpdateTime;
	
	// Update animation timer con tiempo real
	animationTimer += (float)realDeltaTime;

	// Check if enough time has passed for next frame (1/12 second)
	float frameTime = 1.0f / frameRate;
	if (animationTimer >= frameTime)
	{
		animationTimer -= frameTime;
		lastUpdateTime = currentTime;

		// Apply increments to current transformation
		currentPosition.x += keyframes[currentPlayIndex].posXInc;
		currentPosition.y += keyframes[currentPlayIndex].posYInc;
		currentPosition.z += keyframes[currentPlayIndex].posZInc;
		currentRotX += keyframes[currentPlayIndex].rotXInc;
		currentRotY += keyframes[currentPlayIndex].rotYInc;
		currentRotZ += keyframes[currentPlayIndex].rotZInc;

		// Increment interpolation step
		currentInterpolationStep++;

		// Check if reached end of current keyframe pair (usar frames específicos del segmento)
		int currentSegmentFrames = framesPerSegment[currentPlayIndex];
		if (currentInterpolationStep >= currentSegmentFrames)
		{
			// Move to next keyframe
			currentPlayIndex++;
			currentInterpolationStep = 0;

			// Handle loop-back
			if (currentPlayIndex >= frameCount)
			{
				currentPlayIndex = 0;
				// Reset to first keyframe position
				currentPosition.x = keyframes[0].posX;
				currentPosition.y = keyframes[0].posY;
				currentPosition.z = keyframes[0].posZ;
				currentRotX = keyframes[0].rotX;
				currentRotY = keyframes[0].rotY;
				currentRotZ = keyframes[0].rotZ;
			}
		}
	}
	else
	{
		// Actualizar lastUpdateTime incluso si no avanzamos frame
		lastUpdateTime = currentTime;
	}
}

void Keyframe_System::applyTransformationToModel(glm::vec3& position, float& rotX, float& rotY, float& rotZ)
{
	if (playbackMode)
	{
		// Copy current interpolated transformation to model
		position = currentPosition;
		rotX = currentRotX;
		rotY = currentRotY;
		rotZ = currentRotZ;
	}
}

bool Keyframe_System::loadKeyframesFromFile(const std::string& filename)
{
	Keyframe_Parser parser;
	int loadedCount = 0;

	bool success = parser.parseFile(filename, keyframes, loadedCount, maxFrames);

	if (success && loadedCount > 0)
	{
		frameCount = loadedCount;
		return true;
	}

	return false;
}

bool Keyframe_System::saveKeyframesToFile(const std::string& filename)
{
	if (frameCount == 0)
	{
		std::cout << "Warning: No keyframes to save." << std::endl;
		return false;
	}

	// Extract animation name from filename (remove path and extension)
	std::string animName = filename;
	size_t lastSlash = animName.find_last_of("/\\");
	if (lastSlash != std::string::npos)
	{
		animName = animName.substr(lastSlash + 1);
	}
	size_t lastDot = animName.find_last_of('.');
	if (lastDot != std::string::npos)
	{
		animName = animName.substr(0, lastDot);
	}

	Keyframe_Pretty_Printer printer;
	return printer.writeToFile(filename, keyframes, frameCount, animName);
}

void Keyframe_System::setCameraPositionTracker(CameraPositionTracker* tracker)
{
	cameraTracker = tracker;
}
