#include "faro.h"

Faro::Faro()
{
	// Posicion: Atras (sur) de la estacion del tren y la torre Forerunner
	// Estacion: (50, 0.81, 70), Torre Forerunner: (80, -5, 95)
	// Faro: entre ambos en X, pero mas al sur (Z mayor que 95)
	position = glm::vec3(20.0f, 25.0f, 140.0f);
	scale = glm::vec3(20.0f, 20.0f, 20.0f);
	rotationY = 0.0f; // Sin rotacion inicial
	initialized = false;

	// Material con especularidad alta para simular superficie pintada
	material = Material(0.8f, 32);
}

Faro::~Faro()
{
}

void Faro::Initialize()
{
	if (!initialized)
	{
		faroModel.LoadModel("Integrantes/Isra/modelos/faro/faro.obj");
		initialized = true;
	}
}

void Faro::Render(GLuint uniformModel, GLuint uniformColor,
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
	faroModel.RenderModel();
}
