#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class MasterChief
{
public:
	MasterChief();
	~MasterChief();

	void Initialize();
	void Update(bool isWalkingKeyPressed, float deltaTime);
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	glm::vec3 GetPosition() const { return position; }

private:
	// Modelos de cada parte del cuerpo
	Model torso;
	Model casco;
	Model brazo_izq;
	Model brazo_der;
	Model antebrazo_izq;
	Model antebrazo_der;
	Model pierna_izq;
	Model pierna_der;
	Model pantorrilla_izq;
	Model pantorrilla_der;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;

	// Sistema de animacion de caminata
	void UpdateWalkingAnimation(bool isWalking, float deltaTime);
	
	// Angulos de rotacion para animacion (en grados)
	float anguloBrazoIzq;
	float anguloBrazoDer;
	float anguloAnteBrazoIzq;
	float anguloAnteBrazoDer;
	float anguloPiernaIzq;
	float anguloPiernaDer;
	float anguloPantorrillaIzq;
	float anguloPantorrillaDer;
	
	// Control de tiempo de animacion (independiente del deltaTime del main)
	double ultimoTiempoAnimacion;
	float tiempoAnimacion;
	float velocidadAnimacion;  // Velocidad del ciclo de caminata
	
	// Parametros de animacion
	float amplitudBrazo;       // Angulo maximo de swing de brazos
	float amplitudPierna;      // Angulo maximo de swing de piernas
	float flexionCodo;         // Angulo de flexion del codo
	float flexionRodilla;      // Angulo de flexion de la rodilla
	float velocidadRetorno;    // Velocidad de retorno a pose neutral
};
