#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class EstacionTren
{
public:
	EstacionTren();
	~EstacionTren();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza la estacion (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model estacionModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
