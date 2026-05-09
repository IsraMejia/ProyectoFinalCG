#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Miku
{
public:
	Miku();
	~Miku();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	void SetBrazo1Angulo(float angulo) { brazo1Angulo = angulo; }
	void SetBrazo2Angulo(float angulo) { brazo2Angulo = angulo; }

private:
	Model baseModel;
	Model cabezaModel;
	Model brazo1Model;
	Model brazo2Model;
	Model pierna1Model;
	Model pierna2Model;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	float brazo1Angulo;
	float brazo2Angulo;
	bool initialized;
};
