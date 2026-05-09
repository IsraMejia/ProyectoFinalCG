
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

//para iluminación
#include "dependencias/CommonValues.h"
#include "dependencias/DirectionalLight.h"
#include "dependencias/PointLight.h"
#include "dependencias/SpotLight.h"
#include "dependencias/Material.h"

// Solo modulos esenciales
#include "Integrantes/Isra/camara_position.h"
#include "Integrantes/Isra/iluminacion_dia_noche.h"
#include "Integrantes/Isra/personaje_principal.h"

// MODULOS ACTIVADOS
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
#include "Integrantes/Isra/KF_por_codigo.h"
#include "Integrantes/Isra/keyframes_main.h"
#include "Integrantes/Andrea/escenario.h"
#include "Integrantes/Andrea/cage_freddy.h"
#include "Integrantes/Andrea/cage_ballora.h"
#include "Integrantes/Andrea/speakers.h"
#include "Integrantes/Andrea/sala_monitores.h"
#include "Integrantes/Andrea/funtime_foxy.h"
#include "Integrantes/Andrea/farola.h"
#include "Integrantes/Andrea/farolas.h"
#include "Integrantes/Andrea/arbol1.h"
#include "Integrantes/Andrea/entrada.h"
#include "Integrantes/Ceci/escenario_M.h"
#include "Integrantes/Ceci/caliope.h"
#include "Integrantes/Ceci/manometro.h"
#include "Integrantes/Ceci/gramofono.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// ============ ELEMENTOS ACTIVOS ============
CameraPositionTracker cameraTracker;

// Modelo del oceano (reemplaza el piso)
Model oceanModel;

// Modelo de la isla
Model islandModel;

// Jefe Maestro (modulo Isra) - Personaje principal con modelado jerarquico
MasterChief masterChief;

// Ciclo Dia/Noche (modulo Isra) - Sistema de ciclo dia/noche
CicloDiaNoche cicloDiaNoche;

// Configuracion del ciclo dia/noche
// Este valor controla la duracion del DIA (semicirculo superior)
// El ciclo completo (dia + noche) durara el DOBLE de este valor
// Ejemplo: duracionDia = 10.0f -> Dia: 10s, Noche: 10s, Total: 20s
const float DURACION_DIA_SEGUNDOS = 50.0f; 

// Control de tecla Z para pausar/reanudar ciclo dia/noche
bool teclaZ_Presionada = false;

Skybox skybox;

// ============ ELEMENTOS ACTIVADOS ============
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

// Escenario (modulo Andrea)
Escenario escenario;

// CageFreddy (modulo Andrea)
CageFreddy cageFreddy;

// CageBallora (modulo Andrea)
CageBallora cageBallora;

// Speakers (modulo Andrea)
Speakers speakers;
Speakers speakers2(glm::vec3(-76.58f, -3.0f, -8.39f));

// SalaMonitores (modulo Andrea)
SalaMonitores salaMonitores;

// FuntimeFoxy (modulo Andrea)
FuntimeFoxy funtimeFoxy;

// Farolas Manager (modulo Andrea) - Maneja todas las farolas del mapa
FarolasManager farolasManager;

// Arbol1 (modulo Andrea)
Arbol1 arbol1(glm::vec3(30.0f, -3.0f, 40.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
Arbol1 arbol2(glm::vec3(107.20f, -2.0f, 21.70f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
Arbol1 arbol3(glm::vec3(-40.0f, -3.0f, 30.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
Arbol1 arbol4(glm::vec3(41.0f, -3.0f, 130.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));  
Arbol1 arbol5(glm::vec3(10.0f, -3.0f, -50.0f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
Arbol1 arbol6(glm::vec3(-83.00f, -2.0f, -80.15f), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));

// Entrada (modulo Andrea) - Debajo del cuervo
Entrada entrada(glm::vec3(155.87f, -160.14f, -114.92f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f)); 

// Keyframe Manager (modulo Isra) - Maneja todas las animaciones por keyframes
KeyframeManager keyframeManager;

// Escenario (modulo Ceci)
Escenario_M escenario_m;

// Caliope (modulo Ceci)
Caliope caliope;

// Manometro (modulo Ceci)
Manometro manometro;

// Gramofonos (modulo Ceci) - 4 instancias
Gramofono gramofono1(glm::vec3( 66.62f, -1.95f,  35.20f),   0.0f);
Gramofono gramofono2(glm::vec3( -2.21f, -1.58f, -83.83f),   0.0f);
Gramofono gramofono3(glm::vec3( 66.24f, -1.60f, -55.17f),   0.0f);
Gramofono gramofono4(glm::vec3(-51.90f, -0.68f,  32.54f),   0.0f);

Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime  = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Nota: La luz direccional ahora es manejada por el sistema DayNightCycle
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

	// ============ CARGAR MODELOS ACTIVOS ============
	// Cargar modelo del oceano
	oceanModel.LoadModel("Models/ocean.obj");

	// Cargar modelo de la isla
	islandModel.LoadModel("Models/island.obj");

	// Inicializar Jefe Maestro (modulo Isra)
	masterChief.Initialize();

	// ============ INICIALIZACIONES ACTIVADAS ============
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

	// Inicializar escenario (modulo Andrea)
	escenario.Initialize();

	// Inicializar CageFreddy (modulo Andrea)
	cageFreddy.Initialize();

	// Inicializar CageBallora (modulo Andrea)
	cageBallora.Initialize();

	// Inicializar Speakers (modulo Andrea)
	speakers.Initialize();
	speakers2.Initialize();

	// Inicializar SalaMonitores (modulo Andrea)
	salaMonitores.Initialize();

	// Inicializar FuntimeFoxy (modulo Andrea)
	funtimeFoxy.Initialize();

	// Inicializar Farolas Manager (modulo Andrea)
	farolasManager.Initialize();

	// Inicializar Arbol1 (modulo Andrea)
	arbol1.Initialize();
	arbol2.Initialize();
	arbol3.Initialize();
	arbol4.Initialize();
	arbol5.Initialize();
	arbol6.Initialize();

	// Inicializar Entrada (modulo Andrea)
	entrada.Initialize();

	// Inicializar sistema de animacion por keyframes (modulo Isra)
	keyframeManager.Initialize(&cameraTracker, &haloPelican, &tren);

	// Inicializar escenario (modulo Ceci)
	escenario_m.Initialize();

	// Inicializar caliope (modulo Ceci)
	caliope.Initialize();

	// Inicializar manometro (modulo Ceci)
	manometro.Initialize();

	// Inicializar gramofonos (modulo Ceci)
	gramofono1.Initialize();
	gramofono2.Initialize();
	gramofono3.Initialize();
	gramofono4.Initialize();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/CubemapL1.png");     // Right (Derecha)
	skyboxFaces.push_back("Textures/Skybox/CubemapL3.png");     // Left (Izquierda)
	skyboxFaces.push_back("Textures/Skybox/CubemapPiso.png");   // Down (Abajo)
	skyboxFaces.push_back("Textures/Skybox/CubemapTecho.png");  // Up (Arriba)
	skyboxFaces.push_back("Textures/Skybox/CubemapL4.png");     // Back (Atrás)
	skyboxFaces.push_back("Textures/Skybox/CubemapL2.png");     // Front (Frente)
	skybox = Skybox(skyboxFaces);

	Material_opaco = Material(0.3f, 4);

	// Inicializar sistema de ciclo dia/noche (modulo Isra)
	// DURACION_DIA_SEGUNDOS controla solo el recorrido de dia
	// El ciclo completo (dia + noche) durara el doble
	// Radio orbital de 300 unidades
	cicloDiaNoche.Inicializar(DURACION_DIA_SEGUNDOS, 300.0f);

	// ============ LUCES ACTIVADAS ============
	unsigned int spotLightCount = 0;
	
	// Configurar spotlights de las farolas (modulo Andrea)
	farolasManager.SetupSpotLights(spotLights, spotLightCount, true);

	// linterna ligada a la camara (desactivada)
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

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

	// Inicializar lastTime antes del loop para evitar deltaTime enorme en el primer frame
	lastTime = glfwGetTime();

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;  // Calculo original que funcionaba
		lastTime = now;

		glfwPollEvents();
		
		// ========== SISTEMA DE CAMARAS ==========
		// Actualizar camara con tracking de posicion
		// Pasar la posicion del personaje para la camara en tercera persona
		cameraTracker.Update(mainWindow.getsKeys(), mainWindow.getXChange(), mainWindow.getYChange(), 
			deltaTime, masterChief.GetPosition());
		
		// Obtener modo de camara actual
		CameraMode cameraMode = cameraTracker.GetCameraMode();
		
		// ========== MOVIMIENTO DEL PERSONAJE ==========
		// Solo mover el personaje cuando la camara en tercera persona esta activa
		if (cameraMode == CameraMode::THIRD_PERSON)
		{
			ThirdPersonCamera* tpCamera = cameraTracker.GetThirdPersonCamera();
			
			// IMPORTANTE: El yaw de la cámara ES el yaw del personaje
			// El mouse horizontal rota al personaje directamente
			float characterYaw = tpCamera->GetYaw();
			masterChief.SetRotationY(characterYaw);
			
			// Calcular el vector forward del personaje basado en su yaw
			// Debe coincidir con la fórmula de la cámara
			float yawRadians = glm::radians(characterYaw);
			glm::vec3 characterForward(-sin(yawRadians), 0.0f, -cos(yawRadians));
			glm::vec3 characterRight = glm::normalize(glm::cross(characterForward, glm::vec3(0.0f, 1.0f, 0.0f)));
			
			// Variables de movimiento
			glm::vec3 moveDirection(0.0f);
			bool isMoving = false;
			
			// Teclas de flecha para mover el personaje
			// El movimiento es relativo a la orientación del personaje
			if (mainWindow.getsKeys()[GLFW_KEY_UP])
			{
				// Avanzar en la dirección que mira el personaje
				moveDirection += characterForward;
				isMoving = true;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_DOWN])
			{
				// Retroceder
				moveDirection -= characterForward;
				isMoving = true;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_LEFT])
			{
				// Strafe izquierda
				moveDirection -= characterRight;
				isMoving = true;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_RIGHT])
			{
				// Strafe derecha
				moveDirection += characterRight;
				isMoving = true;
			}
			
			// Normalizar direccion si hay movimiento diagonal
			if (isMoving && glm::length(moveDirection) > 0.0f)
			{
				moveDirection = glm::normalize(moveDirection);
				
				// Mover el personaje (velocidad similar a la camara de desarrollo: 0.3f)
				masterChief.Move(moveDirection, deltaTime, 0.3f);
			}
			
			// Actualizar animacion del personaje (camina si se presiona alguna flecha)
			masterChief.Update(isMoving, deltaTime);
		}
		else
		{
			// En modo desarrollo, la animacion se activa con flecha arriba (comportamiento original)
			bool isWalkingKeyPressed = mainWindow.getsKeys()[GLFW_KEY_UP];
			masterChief.Update(isWalkingKeyPressed, deltaTime);
		}
		
		// Obtener transformación de cámara activa
		glm::mat4 viewMatrix = cameraTracker.GetViewMatrix();
		glm::vec3 eyePosition = cameraTracker.GetCameraPosition();

		// ============ ACTUALIZACIONES ACTIVADAS ============
		// Manejar input y actualizar animaciones por keyframes (modulo Isra)
		keyframeManager.HandleInput(mainWindow.getsKeys(), mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);
		keyframeManager.Update(deltaTime);

		// Manejar control del ciclo dia/noche con tecla Z (modulo Isra)
		if (mainWindow.getsKeys()[GLFW_KEY_Z] && !teclaZ_Presionada)
		{
			teclaZ_Presionada = true;
			cicloDiaNoche.PausarReanudar();
			if (cicloDiaNoche.EstaPausado())
			{
				printf("\n>>> CICLO DIA/NOCHE PAUSADO <<<\n");
			}
			else
			{
				printf("\n>>> CICLO DIA/NOCHE REANUDADO <<<\n");
			}
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_Z])
		{
			teclaZ_Presionada = false;
		}

		// Actualizar ciclo dia/noche (modulo Isra)
		cicloDiaNoche.Actualizar(deltaTime);

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

		// ============ LUCES ACTIVADAS ============
		// Actualizar luces de faroles según estado (tecla L)
		unsigned int currentSpotLightCount = 0;
		farolasManager.SetupSpotLights(spotLights, currentSpotLightCount, mainWindow.getFarolesEncendidos());

		// Configurar luces en el shader
		shaderList[0].SetDirectionalLight(cicloDiaNoche.ObtenerLuzDireccional());
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// ============ RENDERIZADO DE ELEMENTOS ACTIVOS ============

		// Oceano - por debajo del punto 0 (superficie de la isla)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
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

		// Jefe Maestro en el centro de la isla
		masterChief.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// ============ RENDERIZADOS ACTIVADOS ============
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

		// Cuervo gigante - Ahora sobre la entrada
		bigRaven.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Entrada (modulo Andrea) - Debajo del cuervo
		entrada.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Pelican de Halo
		haloPelican.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Tren
		tren.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
			
		// Escenario (modulo Andrea)
		escenario.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Escenario (modulo Ceci)
		escenario_m.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Caliope (modulo Ceci)
		caliope.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Manometro (modulo Ceci)
		manometro.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Gramofonos (modulo Ceci)
		gramofono1.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		gramofono2.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		gramofono3.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		gramofono4.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// CageFreddy (modulo Andrea)
		cageFreddy.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// CageBallora (modulo Andrea)
		cageBallora.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Speakers (modulo Andrea)
		speakers.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		speakers2.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// SalaMonitores (modulo Andrea)
		salaMonitores.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// FuntimeFoxy (modulo Andrea)
		funtimeFoxy.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Farolas (modulo Andrea)
		farolasManager.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);

		// Arbol1 (modulo Andrea)
		arbol1.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		arbol2.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		arbol3.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		arbol4.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		arbol5.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
		arbol6.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
			

		
		glUseProgram(0);		mainWindow.swapBuffers();
	}

	return 0;
}
