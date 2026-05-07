#pragma once

#include <string>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#define MAX_FRAMES 100

// Forward declaration
class CameraPositionTracker;

// Keyframe structure to store transformation data and interpolation increments
struct Keyframe {
	// Transformation data
	float posX;
	float posY;
	float posZ;
	float rotX;  // pitch
	float rotY;  // yaw
	float rotZ;  // roll

	// Timestamp: momento en el que se capturó este keyframe (en segundos desde el inicio de la captura)
	float timestamp;

	// Interpolation increments (calculated at runtime)
	float posXInc;
	float posYInc;
	float posZInc;
	float rotXInc;
	float rotYInc;
	float rotZInc;
};

// Forward declarations for internal classes
class Keyframe_Parser;
class Keyframe_Pretty_Printer;

// Main keyframe animation system class
class Keyframe_System {
public:
	// Constructor and destructor
	Keyframe_System(int maxKeyframes = 100);
	~Keyframe_System();

	// Mode control
	void toggleRecordingMode();
	void togglePlaybackMode();
	void resetAnimation();

	// Recording mode operations
	void handleRecordingInput(bool* keys, float mouseX, float mouseY, float deltaTime, 
		glm::vec3& modelPosition, float& modelRotX, float& modelRotY, float& modelRotZ);
	void saveCurrentKeyframe();

	// Playback operations
	void updatePlayback(float deltaTime);
	bool isPlaying() const;
	bool isRecording() const;

	// File operations
	bool loadKeyframesFromFile(const std::string& filename);
	bool saveKeyframesToFile(const std::string& filename);

	// Model integration
	void applyTransformationToModel(glm::vec3& position, float& rotX, float& rotY, float& rotZ);

	// Keyframe management
	int getKeyframeCount() const;
	void clearKeyframes();

	// Integración con CameraPositionTracker
	void setCameraPositionTracker(CameraPositionTracker* tracker);

private:
	// Keyframe storage
	Keyframe keyframes[MAX_FRAMES];
	int frameCount;
	int maxFrames;
	int currentPlayIndex;
	int currentInterpolationStep;
	int maxInterpolationSteps;
	int framesPerSegment[MAX_FRAMES];  // Número de frames para cada segmento

	// State flags
	bool recordingMode;
	bool playbackMode;
	bool canSaveFrame;
	bool framesCalculated;  // Flag para indicar si los frames intermedios ya fueron calculados

	// Timing variables
	float animationTimer;
	float frameRate;  // 12 FPS
	float recordingStartTime;  // Tiempo cuando se inició la captura
	float totalRecordingTime;  // Duración total de la captura

	// Current transformation state (for playback)
	glm::vec3 currentPosition;
	float currentRotX;
	float currentRotY;
	float currentRotZ;

	// Referencia al CameraPositionTracker para deshabilitarlo durante grabación
	CameraPositionTracker* cameraTracker;

	// Helper methods
	void calculateInterpolation(int fromIndex, int toIndex);
	void printRecordingMenu();
	void calculateAllFrames();  // Calcular todos los frames intermedios para playback
};
