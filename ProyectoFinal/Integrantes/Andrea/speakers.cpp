#include "speakers.h"

Speakers::Speakers()
{
	position = glm::vec3(-40.70f, -3.0f, -68.36f);
	scale = glm::vec3(5.5f, 5.5f, 5.5f);
	rotationY = 80.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

Speakers::Speakers(glm::vec3 pos)
{
	position = pos;
	scale = glm::vec3(5.5f, 5.5f, 5.5f);
	rotationY = 40.0f;
	initialized = false;
	material = Material(0.3f, 4);
}

Speakers::~Speakers()
{
}

void Speakers::Initialize()
{
	if (!initialized)
	{
		speakersModel.LoadModel("Models/Speakers.obj");
		initialized = true;
	}
}

void Speakers::Render(GLuint uniformModel, GLuint uniformColor,
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

	speakersModel.RenderModel();
}
