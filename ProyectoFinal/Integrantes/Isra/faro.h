#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"
#include "../../dependencias/SpotLight.h"

class Faro
{
public:
	Faro();
	~Faro();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza el faro y actualiza la rotacion de la luz
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	// Actualiza la rotacion de la luz del faro
	void Update(float deltaTime);

	// Obtiene la luz del faro para configurarla en el shader
	SpotLight* GetLight() { return &faroLight; }

private:
	Model faroModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;

	// Luz spotlight del faro (haz direccional rotatorio)
	SpotLight faroLight;
	glm::vec3 lightPosition;
	float lightRotationY;        // Angulo de rotacion actual de la luz
	float lightRotationSpeed;    // Velocidad de rotacion en grados/segundo
};
