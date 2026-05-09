#include "keyframes_main.h"
#include <iostream>

KeyframeManager::KeyframeManager()
    : pelicanAnimSystem(nullptr)
    , trenAnimSystem(nullptr)
    , haloPelican(nullptr)
    , tren(nullptr)
    , kKeyPressed(false)
    , spaceKeyPressed(false)
    , zeroKeyPressed(false)
    , key1Pressed(false)
    , key2Pressed(false)
    , recordingKey1Pressed(false)
    , recordingKey2Pressed(false)
    , waitingForRecordingModelSelection(false)
    , activeModel(ACTIVE_NONE)
{
}

KeyframeManager::~KeyframeManager()
{
    if (pelicanAnimSystem) delete pelicanAnimSystem;
    if (trenAnimSystem) delete trenAnimSystem;
}

void KeyframeManager::Initialize(CameraPositionTracker* cameraTracker, HaloPelican* pelican, Tren* tren)
{
    this->haloPelican = pelican;
    this->tren = tren;

    // Crear sistemas de animación
    pelicanAnimSystem = new Keyframe_System(100);
    trenAnimSystem = new Keyframe_System(100);

    // Configurar tracker de cámara
    pelicanAnimSystem->setCameraPositionTracker(cameraTracker);
    trenAnimSystem->setCameraPositionTracker(cameraTracker);

    // Intentar cargar archivos de keyframes si existen
    std::cout << "\n========================================" << std::endl;
    std::cout << "CARGANDO ANIMACIONES EXISTENTES..." << std::endl;
    std::cout << "========================================" << std::endl;

    bool pelicanLoaded = false;
    bool trainLoaded = false;

    // Cargar animación del Pelican
    if (pelicanAnimSystem->loadKeyframesFromFile("Integrantes/Isra/pelican_halo_runtime.kf"))
    {
        pelicanLoaded = true;
        std::cout << "[OK] Pelican: Animacion cargada" << std::endl;
    }
    else
    {
        std::cout << "[--] Pelican: Sin animacion guardada" << std::endl;
    }

    // Cargar animación del Tren
    if (trenAnimSystem->loadKeyframesFromFile("Integrantes/Isra/tren_runtime.kf"))
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
            pelicanAnimSystem->togglePlaybackMode();
        }

        if (trainLoaded)
        {
            trenAnimSystem->togglePlaybackMode();
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
}

void KeyframeManager::HandleInput(bool* keys, float xChange, float yChange, float deltaTime)
{
    // Cambiar a animación del Pelican con tecla 1
    if (keys[GLFW_KEY_1] && !key1Pressed && 
        !pelicanAnimSystem->isRecording() && !trenAnimSystem->isRecording())
    {
        key1Pressed = true;
        if (pelicanAnimSystem->getKeyframeCount() > 0)
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
    else if (!keys[GLFW_KEY_1])
    {
        key1Pressed = false;
    }

    // Cambiar a animación del Tren con tecla 2
    if (keys[GLFW_KEY_2] && !key2Pressed && 
        !pelicanAnimSystem->isRecording() && !trenAnimSystem->isRecording())
    {
        key2Pressed = true;
        if (trenAnimSystem->getKeyframeCount() > 0)
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
    else if (!keys[GLFW_KEY_2])
    {
        key2Pressed = false;
    }

    // Manejar diferentes modos
    HandleRecordingToggle(keys);
    HandleModelSelection(keys);
    HandlePlaybackToggle(keys);
    HandleAnimationReset(keys);
    HandleRecordingInput(keys, xChange, yChange, deltaTime);
}

void KeyframeManager::HandleRecordingToggle(bool* keys)
{
    if (keys[GLFW_KEY_K] && !kKeyPressed)
    {
        kKeyPressed = true;

        // Si algún sistema está grabando, salir del modo grabación
        if (pelicanAnimSystem->isRecording())
        {
            pelicanAnimSystem->toggleRecordingMode();

            if (!pelicanAnimSystem->isRecording() && pelicanAnimSystem->getKeyframeCount() > 0)
            {
                pelicanAnimSystem->saveKeyframesToFile("Integrantes/Isra/pelican_halo_runtime.kf");
            }
            waitingForRecordingModelSelection = false;
        }
        else if (trenAnimSystem->isRecording())
        {
            trenAnimSystem->toggleRecordingMode();

            if (!trenAnimSystem->isRecording() && trenAnimSystem->getKeyframeCount() > 0)
            {
                trenAnimSystem->saveKeyframesToFile("Integrantes/Isra/tren_runtime.kf");
            }
            waitingForRecordingModelSelection = false;
        }
        else if (waitingForRecordingModelSelection)
        {
            waitingForRecordingModelSelection = false;
            std::cout << "\n>>> SELECCION CANCELADA <<<\n" << std::endl;
        }
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
    else if (!keys[GLFW_KEY_K])
    {
        kKeyPressed = false;
    }
}

void KeyframeManager::HandleModelSelection(bool* keys)
{
    if (!waitingForRecordingModelSelection) return;

    // Opción 1: Pelican
    if (keys[GLFW_KEY_1] && !recordingKey1Pressed)
    {
        recordingKey1Pressed = true;
        waitingForRecordingModelSelection = false;

        std::cout << "\n>>> PELICAN DE HALO SELECCIONADO <<<\n" << std::endl;

        activeModel = ACTIVE_PELICAN;
        pelicanAnimSystem->toggleRecordingMode();
    }
    else if (!keys[GLFW_KEY_1])
    {
        recordingKey1Pressed = false;
    }

    // Opción 2: Tren
    if (keys[GLFW_KEY_2] && !recordingKey2Pressed)
    {
        recordingKey2Pressed = true;
        waitingForRecordingModelSelection = false;

        std::cout << "\n>>> TREN SELECCIONADO <<<\n" << std::endl;

        activeModel = ACTIVE_TRAIN;
        trenAnimSystem->toggleRecordingMode();
    }
    else if (!keys[GLFW_KEY_2])
    {
        recordingKey2Pressed = false;
    }
}

void KeyframeManager::HandlePlaybackToggle(bool* keys)
{
    if (keys[GLFW_KEY_SPACE] && !spaceKeyPressed)
    {
        spaceKeyPressed = true;

        bool pelicanToggled = false;
        bool trenToggled = false;

        if (pelicanAnimSystem->getKeyframeCount() >= 2)
        {
            pelicanAnimSystem->togglePlaybackMode();
            pelicanToggled = true;
        }

        if (trenAnimSystem->getKeyframeCount() >= 2)
        {
            trenAnimSystem->togglePlaybackMode();
            trenToggled = true;
        }

        // Mostrar mensaje informativo
        if (pelicanToggled && trenToggled)
        {
            if (pelicanAnimSystem->isPlaying() || trenAnimSystem->isPlaying())
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
            std::cout << "\n>>> " << (pelicanAnimSystem->isPlaying() ? "Reproduciendo" : "Pausado") 
                      << " animacion del Pelican <<<\n" << std::endl;
        }
        else if (trenToggled)
        {
            std::cout << "\n>>> " << (trenAnimSystem->isPlaying() ? "Reproduciendo" : "Pausado") 
                      << " animacion del Tren <<<\n" << std::endl;
        }
        else
        {
            std::cout << "\n>>> ERROR: No hay animaciones cargadas (necesitas al menos 2 keyframes) <<<\n" << std::endl;
        }
    }
    else if (!keys[GLFW_KEY_SPACE])
    {
        spaceKeyPressed = false;
    }
}

void KeyframeManager::HandleAnimationReset(bool* keys)
{
    if (keys[GLFW_KEY_0] && !zeroKeyPressed)
    {
        zeroKeyPressed = true;

        bool pelicanReset = false;
        bool trenReset = false;

        if (pelicanAnimSystem->getKeyframeCount() > 0)
        {
            pelicanAnimSystem->resetAnimation();
            pelicanReset = true;
        }

        if (trenAnimSystem->getKeyframeCount() > 0)
        {
            trenAnimSystem->resetAnimation();
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
    else if (!keys[GLFW_KEY_0])
    {
        zeroKeyPressed = false;
    }
}

void KeyframeManager::HandleRecordingInput(bool* keys, float xChange, float yChange, float deltaTime)
{
    // Auto-completar selección interna
    if (pelicanAnimSystem->isWaitingForModelSelection())
    {
        bool autoKeys[1024] = {false};
        autoKeys[GLFW_KEY_1] = true;
        pelicanAnimSystem->handleModelSelection(autoKeys);
    }
    else if (trenAnimSystem->isWaitingForModelSelection())
    {
        bool autoKeys[1024] = {false};
        autoKeys[GLFW_KEY_2] = true;
        trenAnimSystem->handleModelSelection(autoKeys);
    }

    // Manejar input de grabación
    if (pelicanAnimSystem->isRecording() && !pelicanAnimSystem->isWaitingForModelSelection())
    {
        pelicanAnimSystem->handleRecordingInput(keys, xChange, yChange, deltaTime,
            haloPelican->position, haloPelican->rotationX, haloPelican->rotationY, haloPelican->rotationZ);
    }

    if (trenAnimSystem->isRecording() && !trenAnimSystem->isWaitingForModelSelection())
    {
        trenAnimSystem->handleRecordingInput(keys, xChange, yChange, deltaTime,
            tren->position, tren->rotationX, tren->rotationY, tren->rotationZ);
    }
}

void KeyframeManager::Update(float deltaTime)
{
    UpdatePlayback(deltaTime);
}

void KeyframeManager::UpdatePlayback(float deltaTime)
{
    // Actualizar playback
    pelicanAnimSystem->updatePlayback(deltaTime);
    trenAnimSystem->updatePlayback(deltaTime);

    // Aplicar transformaciones
    pelicanAnimSystem->applyTransformationToModel(haloPelican->position, haloPelican->rotationX,
        haloPelican->rotationY, haloPelican->rotationZ);

    trenAnimSystem->applyTransformationToModel(tren->position, tren->rotationX,
        tren->rotationY, tren->rotationZ);
}

void KeyframeManager::GetCameraTransform(glm::mat4& viewMatrix, glm::vec3& eyePosition, 
                                          glm::mat4 defaultView, glm::vec3 defaultPos)
{
    if (pelicanAnimSystem->isRecording())
    {
        // Cámara fija para Pelican
        glm::vec3 fixedCameraPos(-198.46f, 37.29f, -94.11f);
        glm::vec3 fixedCameraFront(0.93f, -0.07f, 0.36f);
        glm::vec3 fixedCameraUp(0.0f, 1.0f, 0.0f);

        glm::vec3 fixedCameraTarget = fixedCameraPos + fixedCameraFront;
        viewMatrix = glm::lookAt(fixedCameraPos, fixedCameraTarget, fixedCameraUp);
        eyePosition = fixedCameraPos;
    }
    else if (trenAnimSystem->isRecording())
    {
        // Cámara aérea para Tren
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
        viewMatrix = defaultView;
        eyePosition = defaultPos;
    }
}
