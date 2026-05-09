#include "farolas.h"

FarolasManager::FarolasManager()
    : farola1(nullptr)
    , farola2(nullptr)
    , farola3(nullptr)
    , farola4(nullptr)
    , farola5(nullptr)
{
    // Definir posiciones de las farolas
    farolaPositions = {
        { glm::vec3(-69.55f, -3.0f, 19.05f), true },   // Farola 1 - Con cámara
        { glm::vec3(10.94f, -3.0f, 81.13f), false },   // Farola 2 - Sin cámara
        { glm::vec3(-26.55f, -3.0f, -66.53f), true },  // Farola 3 - Con cámara
        { glm::vec3(93.23f, -3.0f, 44.67f), false },   // Farola 4 - Sin cámara
        { glm::vec3(22.57f, -3.0f, -0.40f), true }     // Farola 5 - Con cámara
    };

    // Crear instancias de farolas con sus posiciones y configuraciones
    farola1 = new Farola(farolaPositions[0].position, 0.0f, farolaPositions[0].hasCamera);
    farola2 = new Farola(farolaPositions[1].position, 0.0f, farolaPositions[1].hasCamera);
    farola3 = new Farola(farolaPositions[2].position, 0.0f, farolaPositions[2].hasCamera);
    farola4 = new Farola(farolaPositions[3].position, 0.0f, farolaPositions[3].hasCamera);
    farola5 = new Farola(farolaPositions[4].position, 0.0f, farolaPositions[4].hasCamera);
}

FarolasManager::~FarolasManager()
{
    if (farola1) delete farola1;
    if (farola2) delete farola2;
    if (farola3) delete farola3;
    if (farola4) delete farola4;
    if (farola5) delete farola5;
}

void FarolasManager::Initialize()
{
    // Inicializar la primera farola (carga los modelos compartidos)
    farola1->Initialize();
    // Las demás farolas comparten los mismos modelos estáticos ya cargados
}

void FarolasManager::Render(GLuint uniformModel, GLuint uniformColor, 
                             GLuint uniformSpecularIntensity, GLuint uniformShininess, 
                             float toRadians)
{
    // Renderizar todas las farolas con cámaras rotando jerárquicamente
    farola1->Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
    farola2->Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
    farola3->Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
    farola4->Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
    farola5->Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
}

void FarolasManager::SetupSpotLights(SpotLight* spotLights, unsigned int& spotLightCount, bool farolesEncendidos)
{
    // Determinar intensidad según estado de faroles
    GLfloat farolIntensity = farolesEncendidos ? 1.5f : 0.0f;

    // Configurar spotlight para cada farola
    for (size_t i = 0; i < farolaPositions.size() && i < MAX_SPOT_LIGHTS; i++)
    {
        // Posición del spotlight (5 unidades arriba de la base de la farola)
        glm::vec3 lightPos = farolaPositions[i].position;
        lightPos.y = 5.0f;  // Altura de la luz

        spotLights[spotLightCount] = SpotLight(
            1.0f, 0.9f, 0.7f,      // color amarillo cálido (luz de farol)
            0.0f, farolIntensity,   // ambient bajo, diffuse según estado
            lightPos.x, lightPos.y, lightPos.z,  // posición
            0.0f, -1.0f, 0.0f,      // dirección hacia abajo
            1.0f, 0.01f, 0.001f,    // atenuación (constante, lineal, cuadrática)
            15.0f);                 // ángulo del cono (edge)
        
        spotLightCount++;
    }
}
