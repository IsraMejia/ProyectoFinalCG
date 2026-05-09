#include "microfono.h"

Microfono::Microfono()
{
	position    = glm::vec3(0.0f, 0.0f, 0.0f);
	scale       = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY   = 0.0f;
	initialized = false;
}

Microfono::~Microfono()
{
}

void Microfono::Initialize()
{
	if (initialized) return;

	microfonoModel.LoadModel("Models/microfono_miku.obj");

	material = Material(0.5f, 32);

	// Frente a Miku: miku en (68.70, -0.87, -3.70), rot -90 -> frente es -X
	position  = glm::vec3(67.0f, -0.92f, -3.70f);
	scale     = glm::vec3(0.25f, 0.55f, 0.25f);
	rotationY = -90.0f;

	initialized = true;
}

void Microfono::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	model = glm::mat4(1.0);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
	microfonoModel.RenderModel();
}
