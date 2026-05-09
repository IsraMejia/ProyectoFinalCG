#include "manometro.h"

Manometro::Manometro()
{
	position     = glm::vec3(18.13f, -3.0f, -14.41f);
	scale        = glm::vec3(10.0f, 10.0f, 10.0f);
	rotationY    = 0.0f;
	flechaAngulo = 0.0f;
	material     = Material(0.3f, 4);
	initialized  = false;
}

Manometro::~Manometro()
{
}

void Manometro::Initialize()
{
	if (!initialized)
	{
		manometroModel.LoadModel("Models/manometro.obj");
		flechaModel.LoadModel("Models/manometro_flecha.obj");
		initialized = true;
	}
}

void Manometro::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized)
	{
		Initialize();
	}

	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// --- Manómetro base ---
	glm::mat4 modelBase(1.0f);
	modelBase = glm::translate(modelBase, position);
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	manometroModel.RenderModel();

	// --- Flecha (jerarquía sobre el manómetro) ---
	// El offset Y se aplica ANTES del scale para que no se multiplique por 10
	glm::mat4 modelFlecha(1.0f);
	modelFlecha = glm::translate(modelFlecha, position + glm::vec3(0.0f, 19.23f, 0.0f));
	modelFlecha = glm::rotate(modelFlecha, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelFlecha = glm::scale(modelFlecha, scale);
	modelFlecha = glm::rotate(modelFlecha, flechaAngulo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFlecha));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	flechaModel.RenderModel();
}
