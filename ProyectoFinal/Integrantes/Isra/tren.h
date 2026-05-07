#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Tren
{
public:
	Tren();
	~Tren();

	// Inicializa y carga el modelo
	void Initialize();

	// Renderiza el tren (unica llamada necesaria en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model trenModel;
	Model vagonModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
	
	// Offset relativo del vagon respecto al tren (en espacio local del tren)
	glm::vec3 vagonOffset;
	
	// Rotacion adicional del vagon en Y para seguir la curva de las vias
	float vagonRotationY;
};
