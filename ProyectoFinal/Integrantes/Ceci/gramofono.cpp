#include "gramofono.h"
#include <glfw3.h>

// Definición de estáticos compartidos
Model Gramofono::baseModel;
Model Gramofono::bocaModel;
bool  Gramofono::modelsLoaded = false;

Gramofono::Gramofono()
{
	position    = glm::vec3(0.0f, -3.0f, 0.0f);
	scale       = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY   = 0.0f;
	material    = Material(0.3f, 4);
	initialized = false;
}

Gramofono::Gramofono(glm::vec3 pos, float rotY)
{
	position    = glm::vec3(pos.x, -3.0f, pos.z); // altura fija del escenario
	scale       = glm::vec3(3.0f, 3.0f, 3.0f);
	rotationY   = rotY;
	material    = Material(0.3f, 4);
	initialized = false;
}

Gramofono::~Gramofono()
{
}

void Gramofono::Initialize()
{
	if (!initialized)
	{
		// Solo carga los modelos una vez, todas las instancias los comparten
		if (!modelsLoaded)
		{
			baseModel.LoadModel("Models/gramofono_base.obj");
			bocaModel.LoadModel("Models/gramofono.obj");
			modelsLoaded = true;
		}
		initialized = true;
	}
}

void Gramofono::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) Initialize();

	float t = (float)glfwGetTime();

	// Vibración: pequeño rebote en Y con sin()
	float vibracion = 0.08f * sin(t * 6.0f);

	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// --- Base del gramófono con vibración ---
	glm::mat4 modelBase(1.0f);
	modelBase = glm::translate(modelBase, position + glm::vec3(0.0f, vibracion, 0.0f));
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	baseModel.RenderModel();

	// --- Boca/bocina con vibración y pulso de escala ---
	float pulso = 1.0f + 0.15f * abs(sin(t * 4.0f)); // escala entre 1.0 y 1.15
	glm::mat4 modelBoca(1.0f);
	modelBoca = glm::translate(modelBoca, position + glm::vec3(0.0f, 6.0f + vibracion, 0.0f));
	modelBoca = glm::rotate(modelBoca, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBoca = glm::scale(modelBoca, scale * pulso);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBoca));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	bocaModel.RenderModel();
}
