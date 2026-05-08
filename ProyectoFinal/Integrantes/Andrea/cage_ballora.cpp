#include "cage_ballora.h"

CageBallora::CageBallora()
{
	position = glm::vec3(-68.72f, -3.0f, 6.00f);
	scale = glm::vec3(4.0f, 4.0f, 4.0f);
	rotationY = 0.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

CageBallora::~CageBallora()
{
}

void CageBallora::Initialize()
{
	if (!initialized)
	{
		balloraModel.LoadModel("Models/CageBallora.obj");
		initialized = true;
	}
}

void CageBallora::Render(GLuint uniformModel, GLuint uniformColor,
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

	balloraModel.RenderModel();
}
