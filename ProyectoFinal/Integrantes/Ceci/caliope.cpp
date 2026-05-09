#include "caliope.h"

Caliope::Caliope()
{
	position  = glm::vec3(50.24f, -3.0f, -21.17f);
	scale     = glm::vec3(5.0f, 5.0f, 5.0f);
	rotationY = 0.0f;
	material  = Material(0.3f, 4);
	initialized = false;
}

Caliope::~Caliope()
{
}

void Caliope::Initialize()
{
	if (!initialized)
	{
		caliopeModel.LoadModel("Models/caliope.obj");
		initialized = true;
	}
}

void Caliope::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized)
	{
		Initialize();
	}

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	caliopeModel.RenderModel();
}
