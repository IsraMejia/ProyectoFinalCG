#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class EliteHalo
{
public:
	EliteHalo();
	~EliteHalo();

	// Inicializa y carga todos los modelos jerarquicos
	void Initialize();

	// Renderiza el Elite completo (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	// Modelos jerarquicos del Elite
	Model eliteCuerpo;
	Model eliteRostro;
	Model eliteBrazoDer;
	Model eliteBrazoIzq;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
