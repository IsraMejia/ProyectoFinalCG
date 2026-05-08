#pragma once

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

// Sistema de animacion por keyframes generados proceduralmente
// Diseñado para animar el tren siguiendo las vias ovaladas

class Train_Keyframe_System
{
public:
	// Constructor: genera automaticamente los keyframes del recorrido
	Train_Keyframe_System();
	~Train_Keyframe_System();

	// Actualizar la animacion (llamar cada frame)
	void Update(float deltaTime);

	// Obtener la transformacion actual para aplicar al tren
	void GetCurrentTransform(glm::vec3& outPosition, float& outRotationY);

	// Iniciar/pausar la animacion
	void Play();
	void Pause();
	void Reset();

	// Estado de la animacion
	bool IsPlaying() const { return isPlaying; }

private:
	// Estructura de un keyframe
	struct Keyframe
	{
		float posX;
		float posY;
		float posZ;
		float rotY;
		float timestamp;  // Tiempo en segundos desde el inicio
		
		// Incrementos para interpolacion
		float posXInc;
		float posYInc;
		float posZInc;
		float rotYInc;
	};

	// Generar todos los keyframes del recorrido oval
	void GenerateOvalKeyframes();

	// Calcular interpolaciones entre keyframes
	void CalculateInterpolations();

	// Calcular rotacion basada en la tangente del ovalo
	float CalculateTangentRotation(float angle);

	// Parametros de las vias (copiados de vias_tren.cpp)
	static constexpr float OUTER_RADIUS_X = 0.08604f;
	static constexpr float OUTER_RADIUS_Z = 0.06463f;
	static constexpr float TIE_LENGTH = 0.006f * 0.3f;  // 0.0018f
	static constexpr float INNER_RADIUS_X = OUTER_RADIUS_X - TIE_LENGTH;
	static constexpr float INNER_RADIUS_Z = OUTER_RADIUS_Z - TIE_LENGTH;
	
	// Centro entre los dos rieles
	static constexpr float CENTER_RADIUS_X = (OUTER_RADIUS_X + INNER_RADIUS_X) / 2.0f;
	static constexpr float CENTER_RADIUS_Z = (OUTER_RADIUS_Z + INNER_RADIUS_Z) / 2.0f;
	
	// Offset de las vias en el espacio de la isla
	static constexpr float VIAS_OFFSET_Z = -0.010f;
	static constexpr float ISLAND_HEIGHT = 0.0194f + 0.001f - 0.0008f;
	static constexpr float RAIL_HEIGHT = 0.00063f * 0.3f * 4.7f;
	static constexpr float VIAS_Y = ISLAND_HEIGHT + RAIL_HEIGHT * 0.5f;
	
	// Transformacion de la isla (escala y traslacion)
	static constexpr float ISLAND_SCALE = 1250.0f;
	static constexpr float ISLAND_TRANSLATE_Y = -28.0f;

	// Parametros de animacion
	static constexpr int MAX_KEYFRAMES = 200;
	static constexpr float ANIMATION_DURATION = 60.0f;  // 60 segundos
	static constexpr float FRAME_RATE = 12.0f;  // 12 FPS
	static constexpr int TOTAL_FRAMES = (int)(ANIMATION_DURATION * FRAME_RATE);  // 720 frames

	// Array de keyframes
	Keyframe keyframes[MAX_KEYFRAMES];
	int keyframeCount;

	// Estado de reproduccion
	bool isPlaying;
	float animationTimer;
	int currentKeyframeIndex;
	int currentInterpolationStep;
	int framesPerSegment[MAX_KEYFRAMES];  // Frames entre cada par de keyframes

	// Transformacion actual (interpolada)
	glm::vec3 currentPosition;
	float currentRotationY;
};
