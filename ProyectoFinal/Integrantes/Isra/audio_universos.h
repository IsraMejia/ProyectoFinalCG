/*
 * Sistema de Audio Posicional 2D - Header
 * Universos de Edgar Allan Poe
 * 
 * Autor: Isra
 * Fecha: Mayo 2026
 */

#ifndef AUDIO_UNIVERSOS_H
#define AUDIO_UNIVERSOS_H

#include <glm.hpp>

// ============================================================
// FUNCIONES PÚBLICAS
// ============================================================

/**
 * Inicializa el sistema de audio y carga todas las pistas MP3
 * Debe llamarse UNA VEZ al inicio del programa, después de inicializar GLFW/OpenGL
 * 
 * @return true si se inicializó correctamente, false en caso de error
 * 
 * Ejemplo de uso:
 *   if (!initAudioSystem()) {
 *       printf("Error al inicializar el sistema de audio\n");
 *   }
 */
bool initAudioSystem();

/**
 * Actualiza los volúmenes de todas las pistas basándose en la posición de la cámara
 * Debe llamarse EN CADA FRAME del loop principal
 * 
 * @param cameraPos Posición 3D de la cámara (solo se usan X y Z, Y se ignora)
 * 
 * Ejemplo de uso en el loop principal:
 *   while (!mainWindow.getShouldClose()) {
 *       // ... input y lógica ...
 *       updateAudio(camera.getCameraPosition());
 *       // ... renderizado ...
 *   }
 */
void updateAudio(glm::vec3 cameraPos);

/**
 * Limpia y libera todos los recursos del sistema de audio
 * Debe llamarse UNA VEZ al cerrar el programa, antes de terminar GLFW/OpenGL
 * 
 * Ejemplo de uso:
 *   cleanupAudioSystem();
 *   glfwTerminate();
 */
void cleanupAudioSystem();

#endif // AUDIO_UNIVERSOS_H
