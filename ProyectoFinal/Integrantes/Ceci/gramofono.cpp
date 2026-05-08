#include "gramofono.h"

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

	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// --- Base del gramófono ---
	glm::mat4 modelBase(1.0f);
	modelBase = glm::translate(modelBase, position);
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	baseModel.RenderModel();

	// --- Boca/bocina (jerárquica sobre la base, desplazada en Y) ---
	// El offset se aplica ANTES del scale para que no se multiplique por 5
	glm::mat4 modelBoca(1.0f);
	modelBoca = glm::translate(modelBoca, position + glm::vec3(0.0f, 6.0f, 0.0f));
	modelBoca = glm::rotate(modelBoca, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBoca = glm::scale(modelBoca, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBoca));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	bocaModel.RenderModel();
}
