#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class MaquinaVapor
{
public:
	MaquinaVapor();
	~MaquinaVapor();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza la maquina de vapor (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model maquinaModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
