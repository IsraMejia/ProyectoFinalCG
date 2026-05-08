#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class Gramofono
{
public:
	Gramofono();
	Gramofono(glm::vec3 pos, float rotY);
	~Gramofono();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	// Modelos compartidos entre todas las instancias
	static Model baseModel;
	static Model bocaModel;
	static bool modelsLoaded;

	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;
};
