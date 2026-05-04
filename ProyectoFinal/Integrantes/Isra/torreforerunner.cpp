#include "torreforerunner.h"

HaloForerunner::HaloForerunner()
{
	// Posicion en el cuadrante sur-este de la isla (X+, Z+)
	position = glm::vec3(80.0f, -5.0f, 95.0f);
	scale = glm::vec3(2.50f, 2.50f, 2.50f);
	rotationY = 110.0f; // grados
	initialized = false;

	// Material propio del forerunner
	material = Material(0.3f, 4);
}

HaloForerunner::~HaloForerunner()
{
}

void HaloForerunner::Initialize()
{
	if (!initialized)
	{
		forerunnerModel.LoadModel("Models/forerunner.obj");
		initialized = true;
	}
}

void HaloForerunner::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	// Auto-inicializar si no se ha hecho
	if (!initialized)
	{
		Initialize();
	}

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	// Transformaciones: translate -> rotate -> scale
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	// Enviar uniforms
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Renderizar
	forerunnerModel.RenderModel();
}
