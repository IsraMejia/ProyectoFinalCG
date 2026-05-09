#include "manometro.h"
#include <cmath>
#include <glfw3.h>

Manometro::Manometro()
{
	position     = glm::vec3(0.0f, 0.0f, 0.0f);
	scale        = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY    = 0.0f;
	flechaAngulo = 0.0f;
	tiempoAnim   = 0.0f;
	material     = Material(0.3f, 4);
	initialized  = false;
}

Manometro::~Manometro()
{
}

void Manometro::Initialize()
{
	if (initialized) return;

	manometroModel.LoadModel("Models/manometro.obj");
	flechaModel.LoadModel("Models/manometro_flecha.obj");

	material  = Material(0.5f, 32);
	position  = glm::vec3(49.97f, -3.0f, -23.55f);
	scale     = glm::vec3(10.0f, 10.0f, 10.0f);
	rotationY = 90.0f;

	initialized = true;
}

void Manometro::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	GLuint uniformTextureOffset,
	const float toRadians, float deltaTime)
{
	if (!initialized) return;

	float t = (float)glfwGetTime();
	// abs(sin()) da efecto de rebote: llega al extremo y rebota de vuelta
	flechaAngulo = 45.0f * abs(sin(t * 2.0f)) - 22.5f;

	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Resetear offset de textura
	glm::vec2 toffset(0.0f, 0.0f);
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

	// --- Manómetro base ---
	glm::mat4 modelBase(1.0f);
	modelBase = glm::translate(modelBase, position);
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	manometroModel.RenderModel();

	// --- Flecha oscilatoria ---
	// Pivote real de la flecha según el .obj: (1.482, -0.109, -0.347)
	// La flecha apunta en Y, oscila en Z para moverse de lado a lado
	glm::vec3 pivote(1.482f, -0.109f, -0.347f);

	glm::mat4 modelFlecha(1.0f);
	modelFlecha = glm::translate(modelFlecha, position + glm::vec3(0.0f, 19.23f, 0.0f));
	modelFlecha = glm::rotate(modelFlecha, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelFlecha = glm::scale(modelFlecha, scale);
	// Trasladar al pivote, rotar, regresar
	modelFlecha = glm::translate(modelFlecha,  pivote);
	modelFlecha = glm::rotate(modelFlecha, flechaAngulo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	modelFlecha = glm::translate(modelFlecha, -pivote);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFlecha));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	flechaModel.RenderModel();
}
