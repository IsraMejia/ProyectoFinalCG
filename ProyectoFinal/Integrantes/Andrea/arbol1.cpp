#include "arbol1.h"

Arbol1::Arbol1()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.9f, 0.9f, 0.9f);
	rotationY = 0.0f;
	material = Material(0.3f, 4);
}

Arbol1::Arbol1(glm::vec3 pos, float rotation, glm::vec3 escala)
{
	position = pos;
	scale = escala;
	rotationY = rotation;
	material = Material(0.3f, 4);
}

Arbol1::~Arbol1()
{
}

void Arbol1::Initialize()
{
	modelo.LoadModel("Models/Arbol1.obj");
}

void Arbol1::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	// Reducir tamaño al 40% en todos los ejes manteniendo la base en el suelo
	glm::vec3 adjustedScale = scale * 0.4f;  // 40% del tamaño original en X, Y, Z
	
	// Calcular ajuste de posición Y para mantener la base en el suelo
	// Cuando reducimos la escala Y, el centro del modelo baja
	// Necesitamos compensar subiendo la posición
	float heightReduction = scale.y - adjustedScale.y;  // Cuánto se redujo la altura
	float yOffset = heightReduction * 0.5f;  // La mitad de la reducción (porque el pivote está en el centro)
	
	glm::vec3 adjustedPosition = position;
	adjustedPosition.y += yOffset;

	glm::mat4 model(1.0);
	model = glm::translate(model, adjustedPosition);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, adjustedScale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	modelo.RenderModel();
}
