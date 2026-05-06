#include "elite.h"

EliteHalo::EliteHalo()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY = 0.0f;
	initialized = false;
}

EliteHalo::~EliteHalo()
{
}

void EliteHalo::Initialize()
{
	if (initialized) return;

	// Cargar modelos jerarquicos del Elite
	eliteCuerpo.LoadModel("Integrantes/Isra/modelos/eliteHalo/elite_cuerpo.obj");
	eliteRostro.LoadModel("Integrantes/Isra/modelos/eliteHalo/elite_rostro.obj");
	eliteBrazoDer.LoadModel("Integrantes/Isra/modelos/eliteHalo/elite_brazo_der.obj");
	eliteBrazoIzq.LoadModel("Integrantes/Isra/modelos/eliteHalo/elite_brazo_izq.obj");

	// Material con brillo moderado
	material = Material(0.5f, 32);

	// Posicion entre estacion de tren y torre forerunner
	// Estacion esta en (0, 0, -50), Torre en (120, 0, 80)
	// Elite en punto medio desplazado
	position = glm::vec3(40.0f, 0.0f, 105.0f);
	scale = glm::vec3(3.3f, 3.3f, 3.3f);
	rotationY = 45.0f;  

	initialized = true;
}

void EliteHalo::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	// Transformacion base del Elite
	glm::mat4 modelBase = glm::mat4(1.0);
	modelBase = glm::translate(modelBase, position);
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// 1. Cuerpo (base de la jerarquia)
	glm::mat4 modelCuerpo = modelBase;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpo));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	eliteCuerpo.RenderModel();

	// 2. Rostro (hijo del cuerpo)
	glm::mat4 modelRostro = modelCuerpo;
	// El rostro ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRostro));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	eliteRostro.RenderModel();

	// 3. Brazo derecho (hijo del cuerpo)
	glm::mat4 modelBrazoDer = modelCuerpo;
	// El brazo ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	eliteBrazoDer.RenderModel();

	// 4. Brazo izquierdo (hijo del cuerpo)
	glm::mat4 modelBrazoIzq = modelCuerpo;
	// El brazo ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	eliteBrazoIzq.RenderModel();
}
