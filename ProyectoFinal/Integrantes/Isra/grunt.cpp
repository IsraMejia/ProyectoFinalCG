#include "grunt.h"

GruntHalo::GruntHalo()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY = 0.0f;
	initialized = false;
}

GruntHalo::~GruntHalo()
{
}

void GruntHalo::Initialize()
{
	if (initialized) return;

	// Cargar modelos jerarquicos del Grunt
	gruntCuerpo.LoadModel("Integrantes/Isra/modelos/grunt/grunt_cuerpo.obj");
	gruntRostro.LoadModel("Integrantes/Isra/modelos/grunt/grunt_rostro.obj");
	gruntBrazoDer.LoadModel("Integrantes/Isra/modelos/grunt/grunt_brazo_der.obj");
	gruntBrazoIzq.LoadModel("Integrantes/Isra/modelos/grunt/grunt_brazo_izq.obj");

	// Material con brillo moderado
	material = Material(0.5f, 32);

	// Posicion al lado del Elite
	// Elite esta en (40, 0, 105), colocamos el Grunt a su lado
	position = glm::vec3(40.0f, -1.20f, 100.0f);
	scale = glm::vec3(2.5f, 2.5f, 2.5f);  // Grunt es mas pequeno que Elite
	rotationY = 225.0f;  

	initialized = true;
}

void GruntHalo::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) return;

	// Transformacion base del Grunt
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
	gruntCuerpo.RenderModel();

	// 2. Rostro (hijo del cuerpo)
	glm::mat4 modelRostro = modelCuerpo;
	// El rostro ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRostro));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	gruntRostro.RenderModel();

	// 3. Brazo derecho (hijo del cuerpo)
	glm::mat4 modelBrazoDer = modelCuerpo;
	// El brazo ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	gruntBrazoDer.RenderModel();

	// 4. Brazo izquierdo (hijo del cuerpo)
	glm::mat4 modelBrazoIzq = modelCuerpo;
	// El brazo ya esta posicionado correctamente en el modelo
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	gruntBrazoIzq.RenderModel();
}
