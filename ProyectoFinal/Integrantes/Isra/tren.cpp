#include "tren.h"
#include <cmath>

Tren::Tren()
{
	// Posicion base original del usuario (muy cercana a las vias reales)
	// Solo hacemos ajustes minimos para alinear con la geometria de las vias
	
	// Geometria de las vias (desde vias_tren.cpp)
	float outerRadiusX = 0.08604f;
	float outerRadiusZ = 0.06463f;
	float tieLength = 0.006f * 0.3f; // 0.0018f
	float innerRadiusX = outerRadiusX - tieLength; // 0.08424f
	float innerRadiusZ = outerRadiusZ - tieLength; // 0.06283f
	
	// Centro entre los dos rieles (promedio)
	float centerRadiusX = (outerRadiusX + innerRadiusX) / 2.0f; // 0.08514f
	float centerRadiusZ = (outerRadiusZ + innerRadiusZ) / 2.0f; // 0.06373f
	
	// Offset de las vias en Z
	float viasOffsetZ = -0.010f;
	
	// Altura de las vias
	float islandHeight = 0.0194f + 0.001f - 0.0008f; // 0.0196f
	float railHeight = 0.00063f * 0.3f * 4.7f; // con escalas aplicadas
	float viasY = islandHeight + railHeight * 0.5f;
	
	// Usar la posicion original con ajustes minimos
	// La posicion original (25.86, -1.28, 65.00) esta en coordenadas de mundo
	// Las vias estan centradas en (0, viasY, viasOffsetZ) en coordenadas locales de la isla
	// Mantenemos la posicion original que ya esta bien ubicada
	position = glm::vec3(25.86f, -3.28f, 65.00f);
	
	// Escala original (dimensiones ideales)
	scale = glm::vec3(1.5f, 1.5f, 1.5f);
	
	// Rotacion para alinear con las vias (ajuste fino)
	// La posicion original sugiere que el tren esta orientado correctamente
	rotationY = 10.0f;
	
	initialized = false;

	// Material con especularidad moderada para superficie metalica del tren
	material = Material(0.6f, 32);
}

Tren::~Tren()
{
}

void Tren::Initialize()
{
	if (!initialized)
	{
		trenModel.LoadModel("Integrantes/Isra/modelos/tren/tren.obj");
		initialized = true;
	}
}

void Tren::Render(GLuint uniformModel, GLuint uniformColor,
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
	trenModel.RenderModel();
}
