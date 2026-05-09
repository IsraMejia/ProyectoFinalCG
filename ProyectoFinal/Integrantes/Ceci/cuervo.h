#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Cuervo
{
public:
	Cuervo();
	~Cuervo();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	Model cuerpoModel;
	Model ala1Model;
	Model ala2Model;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
