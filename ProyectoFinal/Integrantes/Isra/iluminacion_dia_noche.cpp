#include "iluminacion_dia_noche.h"
#include <cmath>
#include <glfw3.h>  // Para glfwGetTime()

CicloDiaNoche::CicloDiaNoche()
    : tiempoCiclo(0.0f), duracionDia(10.0f), duracionCicloCompleto(20.0f), radioOrbita(300.0f)
    , pausado(false), ultimoTiempoActualizacion(0.0)
{
    posicionLuzDia = glm::vec3(0.0f, 0.0f, 0.0f);
    posicionLuzNoche = glm::vec3(0.0f, 0.0f, 0.0f);
}

void CicloDiaNoche::Inicializar(float duracionDia, float radioOrbita)
{
    this->duracionDia = duracionDia;
    this->duracionCicloCompleto = duracionDia * 2.0f; // El ciclo completo dura el doble
    this->radioOrbita = radioOrbita;
    this->tiempoCiclo = 0.0f;
    this->ultimoTiempoActualizacion = 0.0; // Se inicializara en la primera actualizacion
    
    // Inicializar luz de dia en mediodia (X=0, Y=300, Z=0)
    posicionLuzDia = glm::vec3(0.0f, radioOrbita, 0.0f);
    
    // Inicializar luz de noche en posicion opuesta (X=0, Y=-300, Z=0)
    posicionLuzNoche = glm::vec3(0.0f, -radioOrbita, 0.0f);
    
    // Crear las luces direccionales con valores por defecto
    luzDia = DirectionalLight(
        1.0f, 1.0f, 1.0f,  // color blanco
        0.5f, 0.8f,        // ambient, diffuse
        0.0f, -1.0f, 0.0f  // direccion inicial (apunta hacia abajo)
    );
    
    luzNoche = DirectionalLight(
        0.7f, 0.8f, 1.0f,  // color azul-blanco suave
        0.15f, 0.24f,      // ambient y diffuse al 30% de la luz de dia (0.5*0.3=0.15, 0.8*0.3=0.24)
        0.0f, 1.0f, 0.0f   // direccion inicial (apunta hacia arriba)
    );
    
    ActualizarLuces();
}

void CicloDiaNoche::Actualizar(float tiempoDelta)
{
    // Si esta pausado, no actualizar el tiempo
    if (pausado)
    {
        // Resetear el tiempo de actualizacion para evitar saltos al reanudar
        ultimoTiempoActualizacion = 0.0;
        return;
    }
    
    // Usar tiempo real de GLFW en lugar del deltaTime del main
    // Esto hace que el ciclo sea independiente del deltaTime "inflado" del main
    double tiempoActual = glfwGetTime();
    
    // Primera vez que se llama o despues de pausar
    if (ultimoTiempoActualizacion == 0.0)
    {
        ultimoTiempoActualizacion = tiempoActual;
        return;
    }
    
    // Calcular deltaTime real
    double deltaTimeReal = tiempoActual - ultimoTiempoActualizacion;
    ultimoTiempoActualizacion = tiempoActual;
    
    // Acumular tiempo del ciclo con tiempo real
    tiempoCiclo += (float)deltaTimeReal;
    
    // Reiniciar ciclo cuando se completa
    if (tiempoCiclo >= duracionCicloCompleto)
    {
        tiempoCiclo -= duracionCicloCompleto;
    }
    
    // Calcular progreso del ciclo (0.0 a 1.0)
    float progreso = tiempoCiclo / duracionCicloCompleto;
    
    // Calcular angulo en radianes (0 a 2*PI)
    // Comenzamos en PI/2 (mediodia, arriba) y avanzamos en sentido horario
    float angulo = (3.14159265f / 2.0f) - (progreso * 2.0f * 3.14159265f);
    
    // Calcular posicion orbital de la luz de DIA en el plano XY
    posicionLuzDia.x = radioOrbita * cos(angulo);
    posicionLuzDia.y = radioOrbita * sin(angulo);
    posicionLuzDia.z = 0.0f;
    
    // Calcular posicion orbital de la luz de NOCHE (opuesta a la de dia)
    // Cuando la luz de dia esta arriba (Y > 0), la luz de noche esta abajo (Y < 0)
    posicionLuzNoche.x = -posicionLuzDia.x;  // Opuesta en X
    posicionLuzNoche.y = -posicionLuzDia.y;  // Opuesta en Y
    posicionLuzNoche.z = 0.0f;
    
    // Actualizar ambas luces
    ActualizarLuces();
}

void CicloDiaNoche::ActualizarLuces()
{
    glm::vec3 centro(0.0f, 0.0f, 0.0f);
    
    // ===== LUZ DE DIA =====
    glm::vec3 direccionDia = glm::normalize(centro - posicionLuzDia);
    
    if (posicionLuzDia.y > 0.0f)
    {
        // Dia - luz encendida con intensidad completa
        luzDia = DirectionalLight(
            1.0f, 1.0f, 1.0f,  // color blanco
            0.5f, 0.8f,        // ambient moderada, diffuse fuerte
            direccionDia.x, direccionDia.y, direccionDia.z
        );
    }
    else
    {
        // Noche - luz de dia apagada
        luzDia = DirectionalLight(
            1.0f, 1.0f, 1.0f,  // color blanco
            0.0f, 0.0f,        // sin ambient ni diffuse
            direccionDia.x, direccionDia.y, direccionDia.z
        );
    }
    
    // ===== LUZ DE NOCHE =====
    // Esta luz SIEMPRE esta encendida, pero solo es visible cuando esta arriba (Y > 0)
    glm::vec3 direccionNoche = glm::normalize(centro - posicionLuzNoche);
    
    if (posicionLuzNoche.y > 0.0f)
    {
        // La luz de noche esta arriba - encendida con intensidad suave (30% de la luz de dia)
        luzNoche = DirectionalLight(
            0.7f, 0.8f, 1.0f,  // color azul-blanco suave
            0.15f, 0.24f,      // ambient y diffuse al 30% de la luz de dia (0.5*0.3=0.15, 0.8*0.3=0.24)
            direccionNoche.x, direccionNoche.y, direccionNoche.z
        );
    }
    else
    {
        // La luz de noche esta abajo - apagada
        luzNoche = DirectionalLight(
            0.7f, 0.8f, 1.0f,  // color azul-blanco
            0.0f, 0.0f,        // sin ambient ni diffuse
            direccionNoche.x, direccionNoche.y, direccionNoche.z
        );
    }
    
    // Combinar ambas luces en una sola para el shader
    CombinarLuces();
}

void CicloDiaNoche::CombinarLuces()
{
    // Determinar cual luz es la dominante
    glm::vec3 direccionFinal;
    float ambientFinal, diffuseFinal;
    glm::vec3 colorFinal;
    
    if (posicionLuzDia.y > 0.0f)
    {
        // Es de dia - la luz de dia es dominante
        // Usamos la direccion de la luz de dia
        glm::vec3 centro(0.0f, 0.0f, 0.0f);
        direccionFinal = glm::normalize(centro - posicionLuzDia);
        
        // Color blanco de dia
        colorFinal = glm::vec3(1.0f, 1.0f, 1.0f);
        ambientFinal = 0.5f;
        diffuseFinal = 0.8f;
    }
    else
    {
        // Es de noche - la luz de noche es dominante
        // Usamos la direccion de la luz de noche
        glm::vec3 centro(0.0f, 0.0f, 0.0f);
        direccionFinal = glm::normalize(centro - posicionLuzNoche);
        
        // Color azul-blanco de noche con intensidad suave (30% de la luz de dia)
        colorFinal = glm::vec3(0.7f, 0.8f, 1.0f);
        ambientFinal = 0.15f;  // 30% de la luz de dia
        diffuseFinal = 0.24f;  // 30% de la luz de dia
    }
    
    // Crear la luz combinada
    luzCombinada = DirectionalLight(
        colorFinal.x, colorFinal.y, colorFinal.z,
        ambientFinal, diffuseFinal,
        direccionFinal.x, direccionFinal.y, direccionFinal.z
    );
}

DirectionalLight* CicloDiaNoche::ObtenerLuzDireccional()
{
    // Retorna la luz combinada (dia + noche)
    return &luzCombinada;
}

DirectionalLight* CicloDiaNoche::ObtenerLuzDia()
{
    return &luzDia;
}

DirectionalLight* CicloDiaNoche::ObtenerLuzNoche()
{
    return &luzNoche;
}

float CicloDiaNoche::ObtenerProgresoCiclo() const
{
    return tiempoCiclo / duracionCicloCompleto;
}

bool CicloDiaNoche::EsDeDia() const
{
    return posicionLuzDia.y > 0.0f;
}

float CicloDiaNoche::ObtenerDuracionDia() const
{
    return duracionDia;
}

float CicloDiaNoche::ObtenerDuracionCicloCompleto() const
{
    return duracionCicloCompleto;
}

void CicloDiaNoche::PausarReanudar()
{
    pausado = !pausado;
}

bool CicloDiaNoche::EstaPausado() const
{
    return pausado;
}
