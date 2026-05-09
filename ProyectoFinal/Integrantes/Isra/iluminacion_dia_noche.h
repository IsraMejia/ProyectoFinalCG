#pragma once

#include "../../dependencias/DirectionalLight.h"
#include <glm.hpp>

class CicloDiaNoche
{
public:
    CicloDiaNoche();
    
    // Inicializar el sistema con parametros del ciclo
    // duracionDia: tiempo en segundos que dura el recorrido de dia (semicirculo superior)
    // El ciclo completo (dia + noche) durara el doble de este valor
    void Inicializar(float duracionDia = 10.0f, float radioOrbita = 300.0f);
    
    // Actualizar la posicion orbital de la luz
    void Actualizar(float tiempoDelta);
    
    // Obtener la luz direccional actualizada
    DirectionalLight* ObtenerLuzDireccional();
    
    // Obtener la luz de dia (blanca, fuerte)
    DirectionalLight* ObtenerLuzDia();
    
    // Obtener la luz de noche (azul-blanca, suave)
    DirectionalLight* ObtenerLuzNoche();
    
    // Obtener el progreso actual del ciclo (0.0 a 1.0)
    float ObtenerProgresoCiclo() const;
    
    // Verificar si es de dia (Y > 0)
    bool EsDeDia() const;
    
    // Obtener duracion del dia en segundos
    float ObtenerDuracionDia() const;
    
    // Obtener duracion del ciclo completo en segundos
    float ObtenerDuracionCicloCompleto() const;
    
    // Control de reproduccion - Pausar/Reanudar con tecla Z
    void PausarReanudar();
    bool EstaPausado() const;
    
private:
    DirectionalLight luzDia;       // Luz principal del dia (blanca, fuerte)
    DirectionalLight luzNoche;     // Luz secundaria de noche (azul-blanca, suave)
    DirectionalLight luzCombinada; // Luz combinada que se envia al shader
    
    float tiempoCiclo;           // Tiempo acumulado en el ciclo actual
    float duracionDia;           // Duracion del recorrido de dia en segundos
    float duracionCicloCompleto; // Duracion total del ciclo (dia + noche) en segundos
    float radioOrbita;           // Radio de la orbita circular
    
    glm::vec3 posicionLuzDia;    // Posicion orbital de la luz de dia
    glm::vec3 posicionLuzNoche;  // Posicion orbital de la luz de noche (opuesta)
    
    // Control de reproduccion
    bool pausado;                // Estado de pausa (tecla Z)
    
    // Manejo de tiempo real (independiente del deltaTime del main)
    double ultimoTiempoActualizacion;  // Ultimo tiempo de actualizacion (glfwGetTime)
    
    void ActualizarLuces();
    void CombinarLuces();
};
