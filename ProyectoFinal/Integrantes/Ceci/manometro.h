#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Manometro
{
public:
	Manometro();
	~Manometro();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	// Permite controlar el ángulo de la flecha desde fuera (animación)
	void SetFlechaAngulo(float angulo) { flechaAngulo = angulo; }

private:
	Model manometroModel;
	Model flechaModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	float flechaAngulo;   // rotación de la flecha sobre su eje (jerarquía)
	bool initialized;
};
