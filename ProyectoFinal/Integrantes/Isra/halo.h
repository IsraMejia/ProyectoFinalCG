#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class HaloForerunner
{
public:
	HaloForerunner();
	~HaloForerunner();

	// Inicializa y carga todo lo necesario
	void Initialize();

	// Renderiza el modelo (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor, 
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model forerunnerModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
