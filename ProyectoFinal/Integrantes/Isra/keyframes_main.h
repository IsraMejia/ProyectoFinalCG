#pragma once

#include <glew.h>
#include <glfw3.h>
#include "animacion_KF.h"
#include "halo_pelican.h"
#include "tren.h"
#include "camara_position.h"

// Clase para manejar toda la lógica de animaciones por keyframes
class KeyframeManager
{
public:
    KeyframeManager();
    ~KeyframeManager();

    // Inicializar sistemas de animación
    void Initialize(CameraPositionTracker* cameraTracker, HaloPelican* pelican, Tren* tren);

    // Manejar input del teclado
    void HandleInput(bool* keys, float xChange, float yChange, float deltaTime);

    // Actualizar animaciones
    void Update(float deltaTime);

    // Obtener matrices de vista y posición de cámara según modo de grabación
    void GetCameraTransform(glm::mat4& viewMatrix, glm::vec3& eyePosition, glm::mat4 defaultView, glm::vec3 defaultPos);

private:
    // Sistemas de animación
    Keyframe_System* pelicanAnimSystem;
    Keyframe_System* trenAnimSystem;

    // Referencias a modelos
    HaloPelican* haloPelican;
    Tren* tren;

    // Estado de teclas (para detectar presiones únicas)
    bool kKeyPressed;
    bool spaceKeyPressed;
    bool zeroKeyPressed;
    bool key1Pressed;
    bool key2Pressed;
    bool recordingKey1Pressed;
    bool recordingKey2Pressed;

    // Estado de selección
    bool waitingForRecordingModelSelection;

    // Modelo activo
    enum ActiveModel {
        ACTIVE_NONE = 0,
        ACTIVE_PELICAN = 1,
        ACTIVE_TRAIN = 2
    };
    ActiveModel activeModel;

    // Métodos privados
    void HandleRecordingToggle(bool* keys);
    void HandleModelSelection(bool* keys);
    void HandlePlaybackToggle(bool* keys);
    void HandleAnimationReset(bool* keys);
    void HandleRecordingInput(bool* keys, float xChange, float yChange, float deltaTime);
    void UpdatePlayback(float deltaTime);
};
