#pragma once

#include <glew.h>
#include <glm.hpp>
#include <vector>
#include "farola.h"
#include "../../dependencias/SpotLight.h"
#include "../../dependencias/CommonValues.h"

// Clase que maneja todas las farolas del mapa y sus luces asociadas
class FarolasManager
{
public:
    FarolasManager();
    ~FarolasManager();

    // Inicializar todas las farolas
    void Initialize();

    // Renderizar todas las farolas
    void Render(GLuint uniformModel, GLuint uniformColor, 
                GLuint uniformSpecularIntensity, GLuint uniformShininess, 
                float toRadians);

    // Configurar los spotlights de las farolas en el shader
    void SetupSpotLights(SpotLight* spotLights, unsigned int& spotLightCount, bool farolesEncendidos);

private:
    // Instancias de las 5 farolas distribuidas por el mapa
    Farola* farola1;
    Farola* farola2;
    Farola* farola3;
    Farola* farola4;
    Farola* farola5;

    // Posiciones de las farolas para las luces
    struct FarolaPosition {
        glm::vec3 position;
        bool hasCamera;
    };
    std::vector<FarolaPosition> farolaPositions;
};
