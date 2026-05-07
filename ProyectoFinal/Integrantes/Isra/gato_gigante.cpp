#include "gato_gigante.h"

GatoGigante::GatoGigante()
{
	// Posicion: Centro de las vias del tren (centro de la isla)
	// Las vias estan centradas en (0, 0) con un offset de Z = -0.010
	position = glm::vec3(0.0f, 10.5f, -12.5f);
	scale = glm::vec3(10.0f, 10.0f, 10.0f);  // Gato gigante
	rotationY = 0.0f; // Sin rotacion inicial
	initialized = false;

	// Material con especularidad moderada para pelaje
	material = Material(0.4f, 16);
}

GatoGigante::~GatoGigante()
{
}

void GatoGigante::Initialize()
{
	if (!initialized)
	{
		gatoModel.LoadModel("Models/gato.obj");
		initialized = true;
	}
}

void GatoGigante::Render(GLuint uniformModel, GLuint uniformColor,
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
	gatoModel.RenderModel();
}
