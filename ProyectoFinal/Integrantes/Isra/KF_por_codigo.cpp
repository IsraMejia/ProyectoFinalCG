#include "KF_por_codigo.h"
#include <cmath>
#include <iostream>

Train_Keyframe_System::Train_Keyframe_System()
{
	keyframeCount = 0;
	isPlaying = false;
	animationTimer = 0.0f;
	currentKeyframeIndex = 0;
	currentInterpolationStep = 0;
	
	currentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	currentRotationY = 0.0f;

	// Generar keyframes automaticamente
	GenerateOvalKeyframes();
	
	// Calcular interpolaciones
	CalculateInterpolations();
	
	// Iniciar automaticamente
	Play();
	
	std::cout << "\n========================================" << std::endl;
	std::cout << "SISTEMA DE ANIMACION DEL TREN" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "Keyframes generados: " << keyframeCount << std::endl;
	std::cout << "Duracion: " << ANIMATION_DURATION << " segundos" << std::endl;
	std::cout << "Frame rate: " << FRAME_RATE << " FPS" << std::endl;
	std::cout << "Total frames: " << TOTAL_FRAMES << std::endl;
	std::cout << "Animacion iniciada automaticamente" << std::endl;
	std::cout << "========================================\n" << std::endl;
}

Train_Keyframe_System::~Train_Keyframe_System()
{
}

void Train_Keyframe_System::GenerateOvalKeyframes()
{
	const float PI = 3.14159265f;
	
	// Generar 200 keyframes distribuidos uniformemente alrededor del ovalo
	// Invertimos la direccion del recorrido (de 2*PI a 0 en lugar de 0 a 2*PI)
	for (int i = 0; i < MAX_KEYFRAMES; i++)
	{
		// Angulo en el ovalo (2*PI a 0 para invertir direccion)
		float angle = (2.0f * PI * (MAX_KEYFRAMES - i)) / MAX_KEYFRAMES;
		
		// Calcular posicion en el centro de las vias (espacio local de la isla)
		float localX = CENTER_RADIUS_X * cos(angle);
		float localZ = CENTER_RADIUS_Z * sin(angle) + VIAS_OFFSET_Z;
		float localY = VIAS_Y;
		
		// Transformar al espacio mundial (aplicar escala y traslacion de la isla)
		float worldX = localX * ISLAND_SCALE;
		float worldY = (localY * ISLAND_SCALE) + ISLAND_TRANSLATE_Y;
		float worldZ = localZ * ISLAND_SCALE;
		
		// Calcular rotacion basada en la tangente del ovalo
		float rotationY = CalculateTangentRotation(angle);
		
		// Calcular timestamp (distribuido uniformemente en 60 segundos)
		float timestamp = (ANIMATION_DURATION * i) / MAX_KEYFRAMES;
		
		// Guardar keyframe
		keyframes[i].posX = worldX;
		keyframes[i].posY = worldY;
		keyframes[i].posZ = worldZ;
		keyframes[i].rotY = rotationY;
		keyframes[i].timestamp = timestamp;
		
		keyframeCount++;
	}
	
	std::cout << "Generados " << keyframeCount << " keyframes proceduralmente" << std::endl;
}

float Train_Keyframe_System::CalculateTangentRotation(float angle)
{
	const float PI = 3.14159265f;
	
	// Calcular un punto mas lejano para obtener una tangente mas suave
	// Usar 5 keyframes adelante en lugar de solo 1 para suavizar la rotacion
	//float nextAngle = angle + (2.0f * PI * 2500000000000000000000000000000000.0f / MAX_KEYFRAMES);
    //float nextAngle = angle  ;
	float nextAngle = angle - 0.0000002f  ;

	// Posiciones actual y siguiente
	float x1 = CENTER_RADIUS_X * cos(angle);
	float z1 = CENTER_RADIUS_Z * sin(angle);
	float x2 = CENTER_RADIUS_X * cos(nextAngle);
	float z2 = CENTER_RADIUS_Z * sin(nextAngle);
	
	// Vector tangente (direccion del movimiento)
	float dx = x2 - x1;
	float dz = z2 - z1;
	
	// Calcular angulo de rotacion en grados
	// atan2(dx, dz) calcula el angulo correcto para que el tren apunte en la direccion del movimiento
	// En OpenGL, la rotacion Y gira alrededor del eje Y (vertical)
	// atan2(dx, dz) da el angulo desde el eje Z positivo hacia el eje X positivo
	float rotationRadians = atan2(dx, dz);
	float rotationDegrees = rotationRadians * (180.0f / PI);
	
	// Invertir el angulo para que coincida con la direccion invertida del recorrido
	rotationDegrees = -rotationDegrees;
	
	return rotationDegrees;
}

void Train_Keyframe_System::CalculateInterpolations()
{
	if (keyframeCount < 2)
		return;
	
	std::cout << "\n=== CALCULANDO INTERPOLACIONES ===" << std::endl;
	
	// Calcular frames por segmento basado en timestamps
	int totalFramesCalculated = 0;
	
	for (int i = 0; i < keyframeCount - 1; i++)
	{
		// Tiempo entre este keyframe y el siguiente
		float segmentDuration = keyframes[i + 1].timestamp - keyframes[i].timestamp;
		
		// Frames en este segmento
		int framesInSegment = (int)(segmentDuration * FRAME_RATE);
		if (framesInSegment < 1) framesInSegment = 1;
		
		framesPerSegment[i] = framesInSegment;
		totalFramesCalculated += framesInSegment;
		
		// Calcular incrementos para interpolacion lineal
		keyframes[i].posXInc = (keyframes[i + 1].posX - keyframes[i].posX) / framesInSegment;
		keyframes[i].posYInc = (keyframes[i + 1].posY - keyframes[i].posY) / framesInSegment;
		keyframes[i].posZInc = (keyframes[i + 1].posZ - keyframes[i].posZ) / framesInSegment;
		keyframes[i].rotYInc = (keyframes[i + 1].rotY - keyframes[i].rotY) / framesInSegment;
	}
	
	// Loop back: ultimo keyframe al primero
	float segmentDuration = ANIMATION_DURATION - keyframes[keyframeCount - 1].timestamp;
	int framesInSegment = (int)(segmentDuration * FRAME_RATE);
	if (framesInSegment < 1) framesInSegment = 1;
	
	framesPerSegment[keyframeCount - 1] = framesInSegment;
	totalFramesCalculated += framesInSegment;
	
	keyframes[keyframeCount - 1].posXInc = (keyframes[0].posX - keyframes[keyframeCount - 1].posX) / framesInSegment;
	keyframes[keyframeCount - 1].posYInc = (keyframes[0].posY - keyframes[keyframeCount - 1].posY) / framesInSegment;
	keyframes[keyframeCount - 1].posZInc = (keyframes[0].posZ - keyframes[keyframeCount - 1].posZ) / framesInSegment;
	keyframes[keyframeCount - 1].rotYInc = (keyframes[0].rotY - keyframes[keyframeCount - 1].rotY) / framesInSegment;
	
	std::cout << "Total frames calculados: " << totalFramesCalculated << " / " << TOTAL_FRAMES << std::endl;
	std::cout << "Duracion total: " << (totalFramesCalculated / FRAME_RATE) << " segundos" << std::endl;
	std::cout << "===================================\n" << std::endl;
}

void Train_Keyframe_System::Update(float deltaTime)
{
	if (!isPlaying || keyframeCount < 2)
		return;

	// Actualizar timer con tiempo real
	animationTimer += deltaTime;

	// Verificar si es tiempo de avanzar al siguiente frame (1/12 segundo)
	float frameTime = 1.0f / FRAME_RATE;
	if (animationTimer >= frameTime)
	{
		animationTimer -= frameTime;

		// Aplicar incrementos a la transformacion actual
		currentPosition.x += keyframes[currentKeyframeIndex].posXInc;
		currentPosition.y += keyframes[currentKeyframeIndex].posYInc;
		currentPosition.z += keyframes[currentKeyframeIndex].posZInc;
		currentRotationY += keyframes[currentKeyframeIndex].rotYInc;

		// Incrementar paso de interpolacion
		currentInterpolationStep++;

		// Verificar si llegamos al final del segmento actual
		int currentSegmentFrames = framesPerSegment[currentKeyframeIndex];
		if (currentInterpolationStep >= currentSegmentFrames)
		{
			// Avanzar al siguiente keyframe
			currentKeyframeIndex++;
			currentInterpolationStep = 0;

			// Loop back al inicio
			if (currentKeyframeIndex >= keyframeCount)
			{
				currentKeyframeIndex = 0;
				// Resetear a la posicion del primer keyframe
				currentPosition.x = keyframes[0].posX;
				currentPosition.y = keyframes[0].posY;
				currentPosition.z = keyframes[0].posZ;
				currentRotationY = keyframes[0].rotY;
			}
		}
	}
}

void Train_Keyframe_System::GetCurrentTransform(glm::vec3& outPosition, float& outRotationY)
{
	outPosition = currentPosition;
	outRotationY = currentRotationY;
}

void Train_Keyframe_System::Play()
{
	if (!isPlaying)
	{
		isPlaying = true;
		
		// Si estamos en el inicio, establecer la posicion inicial
		if (currentKeyframeIndex == 0 && currentInterpolationStep == 0)
		{
			currentPosition.x = keyframes[0].posX;
			currentPosition.y = keyframes[0].posY;
			currentPosition.z = keyframes[0].posZ;
			currentRotationY = keyframes[0].rotY;
		}
		
		std::cout << "Animacion del tren: REPRODUCIENDO" << std::endl;
	}
}

void Train_Keyframe_System::Pause()
{
	if (isPlaying)
	{
		isPlaying = false;
		std::cout << "Animacion del tren: PAUSADA" << std::endl;
	}
}

void Train_Keyframe_System::Reset()
{
	currentKeyframeIndex = 0;
	currentInterpolationStep = 0;
	animationTimer = 0.0f;
	
	// Resetear a la posicion inicial
	currentPosition.x = keyframes[0].posX;
	currentPosition.y = keyframes[0].posY;
	currentPosition.z = keyframes[0].posZ;
	currentRotationY = keyframes[0].rotY;
	
	std::cout << "Animacion del tren: RESETEADA" << std::endl;
}
