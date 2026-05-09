#include "kalen.h"

Kalen::Kalen()
{
	position       = glm::vec3(0.0f, 0.0f, 0.0f);
	scale          = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY      = 0.0f;
	brazoDerAngulo = 0.0f;
	brazoIzqAngulo = 0.0f;
	initialized    = false;
}

Kalen::~Kalen()
{
}

void Kalen::Initialize()
{
	if (initialized) return;

	cuerpoModel.LoadModel("Models/kalen_cuerpo.obj");
	cabezaModel.LoadModel("Models/kalen_cabeza.obj");
	brazoDerModel.LoadModel("Models/kalen_brazo_der.obj");
	brazoIzqModel.LoadModel("Models/kalen_brazo_izq.obj");

	material = Material(0.3f, 4);

	position  = glm::vec3(79.48f, -0.92f, -17.31f);
	scale     = glm::vec3(2.0f, 2.0f, 2.0f);
	rotationY = 90.0f;

	initialized = true;
}

void Kalen::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));

	// Cuerpo base
	model = glm::mat4(1.0);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
	modelaux = model;

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cuerpoModel.RenderModel();

	// Cabeza
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cabezaModel.RenderModel();

	// Brazo derecho
	model = modelaux;
	model = glm::rotate(model, brazoDerAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brazoDerModel.RenderModel();

	// Brazo izquierdo
	model = modelaux;
	model = glm::rotate(model, brazoIzqAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brazoIzqModel.RenderModel();
}
