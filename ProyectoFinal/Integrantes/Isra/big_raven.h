#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class BigRaven
{
public:
	BigRaven();
	~BigRaven();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza el cuervo gigante (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model ravenModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
