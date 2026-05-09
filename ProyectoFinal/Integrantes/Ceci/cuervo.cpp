#include "cuervo.h"
#include <glfw3.h>
#include <cmath>

Cuervo::Cuervo()
{
	position    = glm::vec3(0.0f, 0.0f, 0.0f);
	scale       = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY   = 0.0f;
	initialized = false;
}

Cuervo::~Cuervo()
{
}

void Cuervo::Initialize()
{
	if (initialized) return;

	cuerpoModel.LoadModel("Models/cuervo_cuerpo.obj");
	ala1Model.LoadModel("Models/cuervo_ala1.obj");
	ala2Model.LoadModel("Models/cuervo_ala2.obj");

	material = Material(0.3f, 4);

	// Enfrente del gramofono 1 (66.62, -3.0, 35.20)
	// El gramofono mira hacia adelante, el cuervo queda frente a él
	position  = glm::vec3(66.62f, 0.0f, 30.0f);
	scale     = glm::vec3(15.0f, 15.0f, 15.0f);
	rotationY = 0.0f; // mira hacia el gramofono

	initialized = true;
}

void Cuervo::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	float t = (float)glfwGetTime();

	// abs(sin()) para movimiento de rebote
	// Frecuencia moderada para simular aleteo rítmico
	float aleteo = 25.0f * abs(sin(t * 4.0f));

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

	// Ala 1 — rota hacia arriba con el aleteo
	model = modelaux;
	model = glm::rotate(model, aleteo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ala1Model.RenderModel();

	// Ala 2 — rota en sentido contrario (simetría)
	model = modelaux;
	model = glm::rotate(model, -aleteo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ala2Model.RenderModel();
}
