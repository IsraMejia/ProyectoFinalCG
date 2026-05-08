/*
Pr�ctica 7: Iluminaci�n 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "dependencias/Window.h"
#include "dependencias/Mesh.h"
#include "dependencias/Shader_light.h"
#include "dependencias/Camera.h"
#include "dependencias/Texture.h"
#include "dependencias/Skybox.h"
#include "dependencias/Model.h"

//para iluminaci�n
#include "dependencias/CommonValues.h"
#include "dependencias/DirectionalLight.h"
#include "dependencias/PointLight.h"
#include "dependencias/SpotLight.h"
#include "dependencias/Material.h"

// Modulos de integrantes
#include "Integrantes/Isra/vias_tren.h"
#include "Integrantes/Isra/estacion_tren.h"
#include "Integrantes/Isra/torreforerunner.h"
#include "Integrantes/Isra/elite.h"
#include "Integrantes/Isra/grunt.h"
#include "Integrantes/Isra/maquina_vapor.h"
#include "Integrantes/Isra/faro.h"
#include "Integrantes/Isra/gato_gigante.h"
#include "Integrantes/Isra/big_raven.h"
#include "Integrantes/Isra/halo_pelican.h"
#include "Integrantes/Isra/tren.h"
#include "Integrantes/Isra/camara_position.h"
#include "Integrantes/Isra/animacion_KF.h"
#include "Integrantes/Isra/animKFcamara.h"
#include "Integrantes/Isra/KF_por_codigo.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

CameraPositionTracker cameraTracker;

// Modelo del oceano (reemplaza el piso)
Model oceanModel;

// Modelo de la isla
Model islandModel;

// Vias del tren (modulo Isra)
ViasTren viasTren;

// Estacion de tren (modulo Isra)
EstacionTren estacionTren;

// Forerunner (modulo Isra)
HaloForerunner haloForerunner;

// Elite de Halo (modulo Isra)
EliteHalo eliteHalo;

// Grunt de Halo (modulo Isra)
GruntHalo gruntHalo;

// Maquina de vapor (modulo Isra)
MaquinaVapor maquinaVapor;

// Faro (modulo Isra)
Faro faro;

// Gato gigante (modulo Isra)
GatoGigante gatoGigante;

// Cuervo gigante (modulo Isra)
BigRaven bigRaven;

// Pelican de Halo (modulo Isra)
HaloPelican haloPelican;

// Tren (modulo Isra)
Tren tren;

// Train animation system (modulo Isra)
Train_Keyframe_System trainAnimSystem;

// Keyframe animation system (modulo Isra)
// Dos instancias separadas: una para Pelican, otra para Tren
Keyframe_System pelicanAnimSystem(100);
Keyframe_System trenAnimSystem(100);
Animation_Camera animCamera;

// Variable para rastrear qué modelo está activo para reproducción
enum ActiveModel {
	ACTIVE_NONE = 0,
	ACTIVE_PELICAN = 1,
	ACTIVE_TRAIN = 2
};
ActiveModel activeModel = ACTIVE_NONE;

Skybox skybox;

Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime  = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
SpotLight spotLights[MAX_SPOT_LIGHTS];
PointLight pointLights[MAX_POINT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	CreateShaders();


	//Posicion inicial de la camara
	cameraTracker.Initialize(
		glm::vec3(10.0f, 0.0f, -140.0f),  //Posicion Dev
		//glm::vec3(0.0f, 0.0f, 0.0f),  //Posicion CentroMapa 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		-60.0f, 0.0f, 0.3f, 0.5f
	);

	// Cargar modelo del oceano
	oceanModel.LoadModel("Models/ocean.obj");

	// Cargar modelo de la isla
	islandModel.LoadModel("Models/island.obj");

	// Inicializar vias del tren
	viasTren.Initialize();

	// Inicializar estacion de tren
	estacionTren.Initialize();

	// Inicializar forerunner
	haloForerunner.Initialize();

	// Inicializar Elite de Halo
	eliteHalo.Initialize();

	// Inicializar Grunt de Halo
	gruntHalo.Initialize();

	// Inicializar maquina de vapor
	maquinaVapor.Initialize();

	// Inicializar faro
	faro.Initialize();

	// Inicializar gato gigante
	gatoGigante.Initialize();

	// Inicializar cuervo gigante
	bigRaven.Initialize();

	// Inicializar Pelican de Halo
	haloPelican.Initialize();

	// Inicializar tren
	tren.Initialize();

	// Inicializar sistema de animacion por keyframes
	pelicanAnimSystem.setCameraPositionTracker(&cameraTracker);
	trenAnimSystem.setCameraPositionTracker(&cameraTracker);
	
	// Intentar cargar archivos de keyframes si existen
	std::cout << "\n========================================" << std::endl;
	std::cout << "CARGANDO ANIMACIONES EXISTENTES..." << std::endl;
	std::cout << "========================================" << std::endl;
	
	bool pelicanLoaded = false;
	bool trainLoaded = false;
	
	// Cargar animación del Pelican en su sistema dedicado
	if (pelicanAnimSystem.loadKeyframesFromFile("Integrantes/Isra/pelican_halo_runtime.kf"))
	{
		pelicanLoaded = true;
		std::cout << "[OK] Pelican: Animacion cargada" << std::endl;
	}
	else
	{
		std::cout << "[--] Pelican: Sin animacion guardada" << std::endl;
	}
	
	// Cargar animación del Tren en su sistema dedicado
	if (trenAnimSystem.loadKeyframesFromFile("Integrantes/Isra/tren_runtime.kf"))
	{
		trainLoaded = true;
		std::cout << "[OK] Tren: Animacion cargada" << std::endl;
	}
	else
	{
		std::cout << "[--] Tren: Sin animacion guardada" << std::endl;
	}
	
	// Auto-start animations if keyframes are loaded
	if (pelicanLoaded || trainLoaded)
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "INICIANDO ANIMACIONES AUTOMATICAMENTE..." << std::endl;
		std::cout << "========================================\n" << std::endl;
		
		if (pelicanLoaded)
		{
			pelicanAnimSystem.togglePlaybackMode();
		}
		
		if (trainLoaded)
		{
			trenAnimSystem.togglePlaybackMode();
		}
		
		std::cout << ">>> Reproduciendo animaciones del ";
		if (pelicanLoaded && trainLoaded)
		{
			std::cout << "Pelican y Tren <<<" << std::endl;
		}
		else if (pelicanLoaded)
		{
			std::cout << "Pelican <<<" << std::endl;
		}
		else
		{
			std::cout << "Tren <<<" << std::endl;
		}
		std::cout << std::endl;
	}
	
	// Establecer modelo activo predeterminado
	if (pelicanLoaded && trainLoaded)
	{
		activeModel = ACTIVE_PELICAN;
		std::cout << "Modelo activo: Pelican (presiona 1 o 2 para cambiar)" << std::endl;
	}
	else if (trainLoaded)
	{
		activeModel = ACTIVE_TRAIN;
		std::cout << "Modelo activo: Tren" << std::endl;
	}
	else if (pelicanLoaded)
	{
		activeModel = ACTIVE_PELICAN;
		std::cout << "Modelo activo: Pelican" << std::endl;
	}
	else
	{
		activeModel = ACTIVE_NONE;
		std::cout << "Ningun modelo con animacion cargada" << std::endl;
	}
	
	std::cout << "\nCONTROLES:" << std::endl;
	std::cout << "  K     : Capturar nuevos keyframes" << std::endl;
	std::cout << "  SPACE : Pausar/Reanudar animacion" << std::endl;
	std::cout << "  1     : Cargar animacion del Pelican" << std::endl;
	std::cout << "  2     : Cargar animacion del Tren" << std::endl;
	std::cout << "  0     : Resetear animacion" << std::endl;
	std::cout << "========================================\n" << std::endl;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skybox = Skybox(skyboxFaces);

	Material_opaco = Material(0.3f, 4);

	// luz direccional - simula luz solar desde arriba
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,  // color blanco
		0.5f, 0.8f,         // ambiente moderada para evitar sombras negras, difusa fuerte tipo sol
		0.0f, -1.0f, 0.0f); // apunta hacia abajo (luz viene desde arriba)

	unsigned int spotLightCount = 0;
	// linterna ligada a la camara (desactivada)
	/*
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	*/

	// Luz roja al norte (Z negativo) y azul al sur (Z positivo)
	unsigned int pointLightCount = 0;

	// Norte: luz roja en Z negativo
	pointLights[0] = PointLight(
		1.0f, 0.0f, 0.0f,    // color rojo
		0.5f, 1.0f,           // ambient, diffuse
		0.0f, 10.0f, -200.0f, // posicion norte
		0.3f, 0.02f, 0.001f); // atenuacion
	pointLightCount++;

	// Sur: luz azul en Z positivo
	pointLights[1] = PointLight(
		0.0f, 0.0f, 1.0f,    // color azul
		0.5f, 1.0f,           // ambient, diffuse
		0.0f, 10.0f, 200.0f,  // posicion sur
		0.3f, 0.02f, 0.001f); // atenuacion
	pointLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f,
		(GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		
		// Actualizar camara con tracking de posicion
		cameraTracker.Update(mainWindow.getsKeys(), mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);
		Camera* camera = cameraTracker.GetCamera();

		// Keyboard input handling for keyframe animation system
		static bool kKeyPressed = false;
		static bool spaceKeyPressed = false;
		static bool zeroKeyPressed = false;
		static bool key1Pressed = false;
		static bool key2Pressed = false;

		// Cambiar a animación del Pelican con tecla 1 (solo si no está grabando)
		if (mainWindow.getsKeys()[GLFW_KEY_1] && !key1Pressed && 
		    !pelicanAnimSystem.isRecording() && !trenAnimSystem.isRecording())
		{
			key1Pressed = true;
			if (pelicanAnimSystem.getKeyframeCount() > 0)
			{
				activeModel = ACTIVE_PELICAN;
				std::cout << "\n>>> Cambiado a animacion del PELICAN <<<" << std::endl;
				std::cout << "Presiona SPACE para reproducir\n" << std::endl;
			}
			else
			{
				std::cout << "\n>>> ERROR: No hay animacion del Pelican cargada <<<\n" << std::endl;
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_1])
		{
			key1Pressed = false;
		}

		// Cambiar a animación del Tren con tecla 2 (solo si no está grabando)
		if (mainWindow.getsKeys()[GLFW_KEY_2] && !key2Pressed && 
		    !pelicanAnimSystem.isRecording() && !trenAnimSystem.isRecording())
		{
			key2Pressed = true;
			if (trenAnimSystem.getKeyframeCount() > 0)
			{
				activeModel = ACTIVE_TRAIN;
				std::cout << "\n>>> Cambiado a animacion del TREN <<<" << std::endl;
				std::cout << "Presiona SPACE para reproducir\n" << std::endl;
			}
			else
			{
				std::cout << "\n>>> ERROR: No hay animacion del Tren cargada <<<\n" << std::endl;
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_2])
		{
			key2Pressed = false;
		}

		// Toggle recording mode with 'K' key
		static bool waitingForRecordingModelSelection = false;
		static bool recordingKey1Pressed = false;
		static bool recordingKey2Pressed = false;
		
		if (mainWindow.getsKeys()[GLFW_KEY_K] && !kKeyPressed)
		{
			kKeyPressed = true;
			
			// Si algún sistema está grabando, salir del modo grabación
			if (pelicanAnimSystem.isRecording())
			{
				pelicanAnimSystem.toggleRecordingMode();
				
				// Save keyframes when exiting recording mode
				if (!pelicanAnimSystem.isRecording() && pelicanAnimSystem.getKeyframeCount() > 0)
				{
					pelicanAnimSystem.saveKeyframesToFile("Integrantes/Isra/pelican_halo_runtime.kf");
				}
				waitingForRecordingModelSelection = false;
			}
			else if (trenAnimSystem.isRecording())
			{
				trenAnimSystem.toggleRecordingMode();
				
				// Save keyframes when exiting recording mode
				if (!trenAnimSystem.isRecording() && trenAnimSystem.getKeyframeCount() > 0)
				{
					trenAnimSystem.saveKeyframesToFile("Integrantes/Isra/tren_runtime.kf");
				}
				waitingForRecordingModelSelection = false;
			}
			// Si estamos esperando selección, cancelar
			else if (waitingForRecordingModelSelection)
			{
				waitingForRecordingModelSelection = false;
				std::cout << "\n>>> SELECCION CANCELADA <<<\n" << std::endl;
			}
			// Si no hay grabación activa, mostrar menú de selección
			else
			{
				waitingForRecordingModelSelection = true;
				std::cout << "\n========================================" << std::endl;
				std::cout << "=== SELECCION DE MODELO ===" << std::endl;
				std::cout << "========================================" << std::endl;
				std::cout << "Selecciona el modelo a animar:" << std::endl;
				std::cout << "  1 : Pelican de Halo" << std::endl;
				std::cout << "  2 : Tren" << std::endl;
				std::cout << "  K : Cancelar y salir" << std::endl;
				std::cout << "========================================\n" << std::endl;
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_K])
		{
			kKeyPressed = false;
		}
		
		// Handle model selection for recording mode (solo cuando estamos esperando selección)
		if (waitingForRecordingModelSelection)
		{
			// Opción 1: Pelican
			if (mainWindow.getsKeys()[GLFW_KEY_1] && !recordingKey1Pressed)
			{
				recordingKey1Pressed = true;
				waitingForRecordingModelSelection = false;
				
				std::cout << "\n>>> PELICAN DE HALO SELECCIONADO <<<\n" << std::endl;
				
				// Activar modo grabación del Pelican
				activeModel = ACTIVE_PELICAN;
				pelicanAnimSystem.toggleRecordingMode();
				// La selección interna se manejará automáticamente en el siguiente frame
			}
			else if (!mainWindow.getsKeys()[GLFW_KEY_1])
			{
				recordingKey1Pressed = false;
			}
			
			// Opción 2: Tren
			if (mainWindow.getsKeys()[GLFW_KEY_2] && !recordingKey2Pressed)
			{
				recordingKey2Pressed = true;
				waitingForRecordingModelSelection = false;
				
				std::cout << "\n>>> TREN SELECCIONADO <<<\n" << std::endl;
				
				// Activar modo grabación del Tren
				activeModel = ACTIVE_TRAIN;
				trenAnimSystem.toggleRecordingMode();
				// La selección interna se manejará automáticamente en el siguiente frame
			}
			else if (!mainWindow.getsKeys()[GLFW_KEY_2])
			{
				recordingKey2Pressed = false;
			}
		}

		// Toggle playback mode with SPACE key
		if (mainWindow.getsKeys()[GLFW_KEY_SPACE] && !spaceKeyPressed)
		{
			spaceKeyPressed = true;
			
			// Activar/desactivar playback de AMBOS sistemas si tienen keyframes
			bool pelicanToggled = false;
			bool trenToggled = false;
			
			if (pelicanAnimSystem.getKeyframeCount() >= 2)
			{
				pelicanAnimSystem.togglePlaybackMode();
				pelicanToggled = true;
				std::cout << "[DEBUG] Pelican: " << pelicanAnimSystem.getKeyframeCount() << " keyframes, playback=" << (pelicanAnimSystem.isPlaying() ? "ON" : "OFF") << std::endl;
			}
			
			if (trenAnimSystem.getKeyframeCount() >= 2)
			{
				trenAnimSystem.togglePlaybackMode();
				trenToggled = true;
				std::cout << "[DEBUG] Tren: " << trenAnimSystem.getKeyframeCount() << " keyframes, playback=" << (trenAnimSystem.isPlaying() ? "ON" : "OFF") << std::endl;
			}
			
			// Mostrar mensaje informativo
			if (pelicanToggled && trenToggled)
			{
				if (pelicanAnimSystem.isPlaying() || trenAnimSystem.isPlaying())
				{
					std::cout << "\n>>> Reproduciendo animaciones del Pelican y Tren <<<\n" << std::endl;
				}
				else
				{
					std::cout << "\n>>> Animaciones pausadas <<<\n" << std::endl;
				}
			}
			else if (pelicanToggled)
			{
				std::cout << "\n>>> " << (pelicanAnimSystem.isPlaying() ? "Reproduciendo" : "Pausado") << " animacion del Pelican <<<\n" << std::endl;
			}
			else if (trenToggled)
			{
				std::cout << "\n>>> " << (trenAnimSystem.isPlaying() ? "Reproduciendo" : "Pausado") << " animacion del Tren <<<\n" << std::endl;
			}
			else
			{
				std::cout << "\n>>> ERROR: No hay animaciones cargadas (necesitas al menos 2 keyframes) <<<\n" << std::endl;
				std::cout << "[DEBUG] Pelican keyframes: " << pelicanAnimSystem.getKeyframeCount() << std::endl;
				std::cout << "[DEBUG] Tren keyframes: " << trenAnimSystem.getKeyframeCount() << std::endl;
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_SPACE])
		{
			spaceKeyPressed = false;
		}

		// Reset animation with '0' key
		if (mainWindow.getsKeys()[GLFW_KEY_0] && !zeroKeyPressed)
		{
			zeroKeyPressed = true;
			
			// Resetear AMBOS sistemas si tienen keyframes
			bool pelicanReset = false;
			bool trenReset = false;
			
			if (pelicanAnimSystem.getKeyframeCount() > 0)
			{
				pelicanAnimSystem.resetAnimation();
				pelicanReset = true;
			}
			
			if (trenAnimSystem.getKeyframeCount() > 0)
			{
				trenAnimSystem.resetAnimation();
				trenReset = true;
			}
			
			if (pelicanReset && trenReset)
			{
				std::cout << "\n>>> Ambas animaciones reseteadas <<<\n" << std::endl;
			}
			else if (pelicanReset)
			{
				std::cout << "\n>>> Animacion del Pelican reseteada <<<\n" << std::endl;
			}
			else if (trenReset)
			{
				std::cout << "\n>>> Animacion del Tren reseteada <<<\n" << std::endl;
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_0])
		{
			zeroKeyPressed = false;
		}

		// Handle model selection when in recording mode
		// Auto-completar la selección interna del sistema basándose en la selección del menú principal
		if (pelicanAnimSystem.isWaitingForModelSelection())
		{
			// Simular presión de tecla 1 automáticamente
			bool autoKeys[1024] = {false};
			autoKeys[GLFW_KEY_1] = true;
			pelicanAnimSystem.handleModelSelection(autoKeys);
		}
		else if (trenAnimSystem.isWaitingForModelSelection())
		{
			// Simular presión de tecla 2 automáticamente
			bool autoKeys[1024] = {false};
			autoKeys[GLFW_KEY_2] = true;
			trenAnimSystem.handleModelSelection(autoKeys);
		}

		// Handle recording mode input (arrow keys, N, M, L, P keys)
		if (pelicanAnimSystem.isRecording() && !pelicanAnimSystem.isWaitingForModelSelection())
		{
			pelicanAnimSystem.handleRecordingInput(mainWindow.getsKeys(), 0.0f, 0.0f, deltaTime,
				haloPelican.position, haloPelican.rotationX, haloPelican.rotationY, haloPelican.rotationZ);
		}
		
		if (trenAnimSystem.isRecording() && !trenAnimSystem.isWaitingForModelSelection())
		{
			trenAnimSystem.handleRecordingInput(mainWindow.getsKeys(), 0.0f, 0.0f, deltaTime,
				tren.position, tren.rotationX, tren.rotationY, tren.rotationZ);
		}

		// Update playback animation para ambos sistemas
		pelicanAnimSystem.updatePlayback(deltaTime);
		trenAnimSystem.updatePlayback(deltaTime);
		
		// Aplicar transformación del Pelican si está en playback
		pelicanAnimSystem.applyTransformationToModel(haloPelican.position, haloPelican.rotationX, 
			haloPelican.rotationY, haloPelican.rotationZ);
		
		// Aplicar transformación del Tren si está en playback
		trenAnimSystem.applyTransformationToModel(tren.position, tren.rotationX, 
			tren.rotationY, tren.rotationZ);

		// ANIMACION PROCEDURAL DEL TREN DESHABILITADA
		// (Ahora se usa el sistema de keyframes por teclado)
		// trainAnimSystem.Update(deltaTime);
		// trainAnimSystem.GetCurrentTransform(tren.position, tren.rotationY);

		// Switch camera based on recording mode
		glm::mat4 viewMatrix;
		glm::vec3 eyePosition;
		
		if (pelicanAnimSystem.isRecording())
		{
			// Cámara fija para Pelican
			glm::vec3 fixedCameraPos(-198.46f, 37.29f, -94.11f);
			glm::vec3 fixedCameraFront(0.93f, -0.07f, 0.36f);
			glm::vec3 fixedCameraUp(0.0f, 1.0f, 0.0f);
			
			glm::vec3 fixedCameraTarget = fixedCameraPos + fixedCameraFront;
			viewMatrix = glm::lookAt(fixedCameraPos, fixedCameraTarget, fixedCameraUp);
			eyePosition = fixedCameraPos;
		}
		else if (trenAnimSystem.isRecording())
		{
			// Cámara aérea para Tren (vista desde arriba)
			glm::vec3 trainCameraPos(6.67f, 231.10f, -19.99f);
			glm::vec3 trainCameraFront(0.00f, -1.00f, -0.03f);
			glm::vec3 trainCameraUp(0.0f, 1.0f, 0.0f);
			
			glm::vec3 trainCameraTarget = trainCameraPos + trainCameraFront;
			viewMatrix = glm::lookAt(trainCameraPos, trainCameraTarget, trainCameraUp);
			eyePosition = trainCameraPos;
		}
		else
		{
			// Usar cámara principal
			viewMatrix = camera->calculateViewMatrix();
			eyePosition = camera->getCameraPosition();
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(viewMatrix, projection);

		shaderList[0].UseShader();
		uniformModel             = shaderList[0].GetModelLocation();
		uniformProjection        = shaderList[0].GetProjectionLocation();
		uniformView              = shaderList[0].GetViewLocation();
		uniformEyePosition       = shaderList[0].GetEyePositionLocation();
		uniformColor             = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess         = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView,       1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, eyePosition.x, eyePosition.y, eyePosition.z);

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);





		// Oceano - por debajo del punto 0 (superficie de la isla)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		oceanModel.RenderModel();

		// Isla centrada en la escena - superficie en Y = 0
		// La isla mide ~0.0204u de alto * escala 1250 = ~25.5u, bajada para que su tope quede en 0
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -28.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1250.0f, 1250.0f, 1250.0f));
		glm::mat4 islandTransform = model; // Guardar para jerarquia
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		islandModel.RenderModel();





		// Vias del tren (jerarquicamente sobre la isla)
		viasTren.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, islandTransform, toRadians);

		// Estacion de tren (entre vias y torre forerunner)
		estacionTren.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Forerunner en el cuadrante sur-este de la isla
		haloForerunner.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Elite de Halo entre estacion y torre forerunner
		eliteHalo.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Grunt de Halo al lado del Elite
		gruntHalo.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Maquina de vapor frente a la estacion
		maquinaVapor.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Faro
		faro.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Gato gigante en el centro de las vias
		gatoGigante.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Cuervo gigante
		bigRaven.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Pelican de Halo
		haloPelican.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Tren
		tren.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		glUseProgram(0);		mainWindow.swapBuffers();
	}

	return 0;
}
