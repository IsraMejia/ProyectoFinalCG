#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>

class CamaraInteres
{
public:
	CamaraInteres();
	~CamaraInteres();

	// Activa o desactiva la camara
	void SetActiva(bool activa);
	bool IsActiva() const;

	// Maneja la tecla C para activar/desactivar
	void HandleInput(bool* keys);

	// Actualiza la posicion de la camara (llamar cada frame)
	void Update(float deltaTime);

	// Retorna la matriz view calculada
	glm::mat4 GetViewMatrix() const;

	// Retorna la posicion actual de la camara (para eyePosition)
	glm::vec3 GetPosition() const;

private:
	// Centro de interes: escenario de Miku
	glm::vec3 target;

	// Estado de la camara
	bool activa;
	bool teclaCPresionada; // para evitar toggle rapido

	// Parametros de orbita
	float angulo;          // angulo actual de orbita (radianes)
	float radio;           // distancia al target
	float altura;          // altura sobre el target
	float velocidadOrbita; // rad/seg
	float velocidadZoom;   // unidades/seg
	float radioMin;
	float radioMax;

	// Fase de la animacion: 0=orbita lejana, 1=acercamiento, 2=orbita cercana, 3=alejamiento
	int fase;
	float tiempoFase;
};
