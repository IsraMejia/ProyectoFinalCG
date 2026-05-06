#include "maquina_vapor.h"

MaquinaVapor::MaquinaVapor()
{
	// Posicion frente a la estacion de tren, cerca del lado este de las vias, paralela a las vias
	// Estacion esta en (50, 0.81, 70) con rotacion 205 grados
	// Colocamos la maquina mas al este y un poco al norte, paralela a las vias
	position = glm::vec3(107.0f, 0.0f, 50.0f);
	scale = glm::vec3(135.0f, 135.0f, 135.0f);
	rotationY = 310.0f; // Paralela a las vias (las vias van norte-sur)
	initialized = false;

	// Material con especularidad moderada para metal
	material = Material(0.7f, 32);
}

MaquinaVapor::~MaquinaVapor()
{
}

void MaquinaVapor::Initialize()
{
	if (!initialized)
	{
		maquinaModel.LoadModel("Integrantes/Isra/modelos/maquinaVapor/maquina_vapor.obj");
		initialized = true;
	}
}

void MaquinaVapor::Render(GLuint uniformModel, GLuint uniformColor,
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
	maquinaModel.RenderModel();
}
