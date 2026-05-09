#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Kalen
{
public:
	Kalen();
	~Kalen();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	void SetBrazoDerAngulo(float angulo) { brazoDerAngulo = angulo; }
	void SetBrazoIzqAngulo(float angulo) { brazoIzqAngulo = angulo; }

private:
	Model cuerpoModel;
	Model cabezaModel;
	Model brazoDerModel;
	Model brazoIzqModel;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	float brazoDerAngulo;
	float brazoIzqAngulo;
	bool initialized;
};
