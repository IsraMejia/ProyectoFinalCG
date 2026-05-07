#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class HaloPelican
{
public:
	HaloPelican();
	~HaloPelican();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza el Pelican de Halo (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	// Public transformation variables for external animation system
	glm::vec3 position;
	float rotationX;  // pitch
	float rotationY;  // yaw
	float rotationZ;  // roll

private:
	Model pelicanModel;
	Material material;
	glm::vec3 scale;
	bool initialized;
};
