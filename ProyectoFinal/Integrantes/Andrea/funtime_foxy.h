#pragma once

#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class FuntimeFoxy
{
public:
	FuntimeFoxy();
	~FuntimeFoxy();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model foxyModel;
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	float rotationSpeed;
	bool rotatingRight; // true = girando a la derecha, false = girando a la izquierda
	bool initialized;
};
