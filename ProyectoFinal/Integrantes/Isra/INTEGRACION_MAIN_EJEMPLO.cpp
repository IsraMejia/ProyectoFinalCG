/*
 * EJEMPLO DE INTEGRACION DEL SISTEMA DE KEYFRAMES CON CAMERA POSITION TRACKER
 * 
 * Este archivo muestra cómo integrar el sistema de keyframes con el tracker de cámara
 * para evitar conflictos en la salida de consola.
 * 
 * IMPORTANTE: Este es un archivo de EJEMPLO. Copia las secciones relevantes a tu main.cpp
 */

// ============================================================================
// INCLUDES NECESARIOS (agregar al inicio de tu main.cpp)
// ============================================================================
#include "Integrantes/Isra/animacion_KF.h"
#include "Integrantes/Isra/animKFcamara.h"
#include "Integrantes/Isra/halo_pelican.h"
#include "Integrantes/Isra/camara_position.h"

// ============================================================================
// VARIABLES GLOBALES (agregar a tus variables globales)
// ============================================================================

// Sistema de keyframes
Keyframe_System animSystem(100);
Animation_Camera animCamera;

// Modelo Halo Pelican
HaloPelican pelican;

// Camera Position Tracker (ya debes tenerlo)
CameraPositionTracker cameraTracker;

// ============================================================================
// CALLBACK PARA SINCRONIZAR CAMERA TRACKER CON KEYFRAME SYSTEM
// ============================================================================

// Esta función se llama automáticamente cuando cambia el modo de grabación
void onRecordingModeChanged(bool isRecording)
{
	// Deshabilitar el camera tracker cuando se activa el modo de grabación
	// Rehabilitarlo cuando se desactiva
	cameraTracker.SetEnabled(!isRecording);
	
	if (isRecording)
	{
		std::cout << "[INFO] Camera Position Tracker DESHABILITADO (modo grabación activo)" << std::endl;
	}
	else
	{
		std::cout << "[INFO] Camera Position Tracker HABILITADO (modo grabación inactivo)" << std::endl;
	}
}

// ============================================================================
// INICIALIZACION (agregar en tu función de inicialización)
// ============================================================================

void inicializarSistemas()
{
	// Inicializar modelo Pelican
	pelican.Initialize();
	
	// Inicializar camera tracker (como ya lo tienes)
	cameraTracker.Initialize(
		glm::vec3(0.0f, 0.0f, 0.0f),  // posición inicial
		glm::vec3(0.0f, 1.0f, 0.0f),  // up vector
		-60.0f,  // yaw
		0.0f,    // pitch
		0.5f,    // move speed
		0.5f     // turn speed
	);
	
	// IMPORTANTE: Registrar el callback para sincronizar con keyframe system
	animSystem.setRecordingModeCallback(onRecordingModeChanged);
	
	// Intentar cargar keyframes desde archivo
	animSystem.loadKeyframesFromFile("Integrantes/Isra/pelican_halo_code.kf");
	
	std::cout << "\n[INFO] Sistema de keyframes inicializado" << std::endl;
	std::cout << "[INFO] Presiona K para activar modo de grabación de keyframes" << std::endl;
	std::cout << "[INFO] Presiona SPACE para reproducir animación" << std::endl;
}

// ============================================================================
// MANEJO DE INPUT EN EL LOOP PRINCIPAL
// ============================================================================

void manejarInputKeyframes(bool* keys, float mouseX, float mouseY, float deltaTime)
{
	// Variables estáticas para detectar presión única de teclas
	static bool kKeyPressed = false;
	static bool spaceKeyPressed = false;
	static bool zeroKeyPressed = false;
	
	// Tecla K: Toggle modo de grabación
	if (keys[GLFW_KEY_K] && !kKeyPressed)
	{
		kKeyPressed = true;
		
		bool wasRecording = animSystem.isRecording();
		animSystem.toggleRecordingMode();
		
		// Si salimos del modo de grabación, guardar keyframes
		if (wasRecording && !animSystem.isRecording())
		{
			animSystem.saveKeyframesToFile("Integrantes/Isra/pelican_halo_runtime.kf");
		}
	}
	else if (!keys[GLFW_KEY_K])
	{
		kKeyPressed = false;
	}
	
	// Tecla SPACE: Toggle modo de reproducción
	if (keys[GLFW_KEY_SPACE] && !spaceKeyPressed)
	{
		spaceKeyPressed = true;
		animSystem.togglePlaybackMode();
	}
	else if (!keys[GLFW_KEY_SPACE])
	{
		spaceKeyPressed = false;
	}
	
	// Tecla 0: Reset animación
	if (keys[GLFW_KEY_0] && !zeroKeyPressed)
	{
		zeroKeyPressed = true;
		animSystem.resetAnimation();
	}
	else if (!keys[GLFW_KEY_0])
	{
		zeroKeyPressed = false;
	}
	
	// Manejo de input en modo grabación (solo si está activo)
	if (animSystem.isRecording())
	{
		animSystem.handleRecordingInput(keys, mouseX, mouseY, deltaTime);
	}
}

// ============================================================================
// ACTUALIZACION EN EL LOOP PRINCIPAL
// ============================================================================

void actualizarSistemasEnLoop(bool* keys, float mouseX, float mouseY, float deltaTime)
{
	// 1. Manejar input de keyframes
	manejarInputKeyframes(keys, mouseX, mouseY, deltaTime);
	
	// 2. Actualizar reproducción de keyframes
	animSystem.updatePlayback(deltaTime);
	
	// 3. Aplicar transformación al modelo
	animSystem.applyTransformationToModel(
		pelican.position,
		pelican.rotationX,
		pelican.rotationY,
		pelican.rotationZ
	);
	
	// 4. Actualizar cámara según el modo
	glm::mat4 viewMatrix;
	
	if (animSystem.isRecording())
	{
		// Modo grabación: usar cámara de animación que sigue al modelo
		animCamera.enableFollowMode(true);
		animCamera.setFollowTarget(pelican.position);
		animCamera.update(deltaTime);
		viewMatrix = animCamera.getViewMatrix();
		
		// NO actualizar camera tracker (ya está deshabilitado por el callback)
	}
	else
	{
		// Modo normal: usar camera tracker
		animCamera.enableFollowMode(false);
		cameraTracker.Update(keys, mouseX, mouseY, deltaTime);
		viewMatrix = cameraTracker.GetCamera()->calculateViewMatrix();
	}
	
	// 5. Usar viewMatrix para renderizar
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

// ============================================================================
// RENDERIZADO
// ============================================================================

void renderizarModelos(GLuint uniformModel, GLuint uniformColor,
                       GLuint uniformSpecularIntensity, GLuint uniformShininess,
                       const float toRadians)
{
	// Renderizar el Pelican
	pelican.Render(uniformModel, uniformColor, 
	               uniformSpecularIntensity, uniformShininess, 
	               toRadians);
	
	// ... renderizar otros modelos ...
}

// ============================================================================
// EJEMPLO DE LOOP PRINCIPAL COMPLETO
// ============================================================================

/*
void loopPrincipal()
{
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// Recibir eventos
		glfwPollEvents();
		
		// Obtener input de mouse
		float mouseX = mainWindow.getXChange();
		float mouseY = mainWindow.getYChange();
		
		// Actualizar sistemas
		actualizarSistemasEnLoop(mainWindow.getsKeys(), mouseX, mouseY, deltaTime);
		
		// Clear y renderizado
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Renderizar skybox, otros objetos, etc.
		// ...
		
		// Renderizar modelos
		renderizarModelos(uniformModel, uniformColor, 
		                  uniformSpecularIntensity, uniformShininess, 
		                  toRadians);
		
		// Swap buffers
		mainWindow.swapBuffers();
	}
}
*/

// ============================================================================
// RESUMEN DE CAMBIOS NECESARIOS EN TU MAIN.CPP
// ============================================================================

/*
1. AGREGAR INCLUDES:
   - #include "Integrantes/Isra/animacion_KF.h"
   - #include "Integrantes/Isra/animKFcamara.h"

2. AGREGAR VARIABLES GLOBALES:
   - Keyframe_System animSystem(100);
   - Animation_Camera animCamera;
   - HaloPelican pelican;

3. AGREGAR CALLBACK:
   - Copiar la función onRecordingModeChanged()

4. EN INICIALIZACION:
   - pelican.Initialize();
   - animSystem.setRecordingModeCallback(onRecordingModeChanged);
   - animSystem.loadKeyframesFromFile("Integrantes/Isra/pelican_halo_code.kf");

5. EN LOOP PRINCIPAL:
   - Llamar manejarInputKeyframes() antes de actualizar cámara
   - Actualizar sistema según el modo (grabación vs normal)
   - Usar la viewMatrix apropiada

6. CONTROLES:
   - K: Toggle modo grabación
   - SPACE: Toggle reproducción
   - 0: Reset animación
   - Flechas: Mover modelo (en modo grabación)
   - Mouse: Rotar modelo (en modo grabación)
   - L: Guardar keyframe (en modo grabación)
   - P: Habilitar guardar siguiente keyframe (en modo grabación)
*/
