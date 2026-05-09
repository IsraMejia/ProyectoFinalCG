#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Entrada
{
public:
	Entrada();
	Entrada(glm::vec3 pos, float rotation = 0.0f, glm::vec3 escala = glm::vec3(1.0f, 1.0f, 1.0f));
	~Entrada();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	glm::vec3 GetPosition() const { return position; }

private:
	Model modelo;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
};
