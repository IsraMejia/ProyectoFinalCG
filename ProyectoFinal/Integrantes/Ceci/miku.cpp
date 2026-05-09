#include "miku.h"

Miku::Miku()
{
	position     = glm::vec3(0.0f, 0.0f, 0.0f);
	scale        = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY    = 0.0f;
	brazo1Angulo = 0.0f;
	brazo2Angulo = 0.0f;
	initialized  = false;
}

Miku::~Miku()
{
}

void Miku::Initialize()
{
	if (initialized) return;

	baseModel.LoadModel("Models/miku_base.obj");
	cabezaModel.LoadModel("Models/miku_cabeza.obj");
	brazo1Model.LoadModel("Models/miku_brazo1.obj");
	brazo2Model.LoadModel("Models/miku_brazo2.obj");
	pierna1Model.LoadModel("Models/miku_pierna1.obj");
	pierna2Model.LoadModel("Models/miku_pierna2.obj");

	material = Material(0.3f, 4);

	position  = glm::vec3(68.70f, -0.87f, -3.70f);
	scale     = glm::vec3(0.04f, 0.04f, 0.04f);
	rotationY = -90.0f;

	initialized = true;
}

void Miku::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));

	// Base del personaje
	model = glm::mat4(1.0);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
	modelaux = model;

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	baseModel.RenderModel();

	// Cabeza
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cabezaModel.RenderModel();

	// Brazo 1
	model = modelaux;
	model = glm::rotate(model, brazo1Angulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brazo1Model.RenderModel();

	// Brazo 2
	model = modelaux;
	model = glm::rotate(model, brazo2Angulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brazo2Model.RenderModel();

	// Pierna 1
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pierna1Model.RenderModel();

	// Pierna 2
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pierna2Model.RenderModel();
}
