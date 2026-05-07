#include "halo_pelican.h"

HaloPelican::HaloPelican()
{
	// Posicion especificada por el usuario
	position = glm::vec3(-58.58f, 1.15f, 35.59f);
	scale = glm::vec3(3.0f, 3.0f, 3.0f);  // Escala base del Pelican
	rotationX = 0.0f;  // pitch - sin rotacion inicial
	rotationY = 80.0f; // yaw - rotacion inicial
	rotationZ = 0.0f;  // roll - sin rotacion inicial
	initialized = false;

	// Material con especularidad alta para superficie metalica
	material = Material(0.8f, 64);
}

HaloPelican::~HaloPelican()
{
}

void HaloPelican::Initialize()
{
	if (!initialized)
	{
		pelicanModel.LoadModel("Integrantes/Isra/modelos/pelican_halo/pelican.obj");
		initialized = true;
	}
}

void HaloPelican::Render(GLuint uniformModel, GLuint uniformColor,
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

	// Transformaciones: translate -> rotate (X, Y, Z) -> scale
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));  // pitch
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));  // yaw
	model = glm::rotate(model, rotationZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));  // roll
	model = glm::scale(model, scale);

	// Enviar uniforms
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Renderizar
	pelicanModel.RenderModel();
}
