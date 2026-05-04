#include "estacion_tren.h"

EstacionTren::EstacionTren()
{
	// Posicion entre la torre forerunner y las vias (centro-este de la isla)
	position = glm::vec3(50.0f, 0.81f, 70.0f);
	scale = glm::vec3(4.5f, 4.5f, 4.5f);
	rotationY = 205.0f; // grados - orientada hacia las vias
	initialized = false;

	// Material para la estacion (mayor especularidad para mejor iluminacion)
	material = Material(0.8f, 32);
}

EstacionTren::~EstacionTren()
{
}

void EstacionTren::Initialize()
{
	if (!initialized)
	{
		estacionModel.LoadModel("Integrantes/Isra/modelos/EstacionTren/estacion_tren.obj");
		initialized = true;
	}
}

void EstacionTren::Render(GLuint uniformModel, GLuint uniformColor,
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
	estacionModel.RenderModel();
}
