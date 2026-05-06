#include "big_raven.h"

BigRaven::BigRaven()
{
	// Posicion especificada por el usuario
	position = glm::vec3(10.0f, 8.0f, -140.0f);
	scale = glm::vec3(100.0f, 100.0f, 100.0f);  // Cuervo grande
	rotationY = -103.0f; // Sin rotacion inicial
	initialized = false;

	// Material con especularidad moderada para plumas
	material = Material(0.5f, 32);
}

BigRaven::~BigRaven()
{
}

void BigRaven::Initialize()
{
	if (!initialized)
	{
		ravenModel.LoadModel("Integrantes/Isra/modelos/big_raven/raven.obj");
		initialized = true;
	}
}

void BigRaven::Render(GLuint uniformModel, GLuint uniformColor,
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
	ravenModel.RenderModel();
}
