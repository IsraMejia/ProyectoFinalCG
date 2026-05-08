#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Mesh.h"
#include "../../dependencias/Texture.h"
#include "../../dependencias/Material.h"

class Escenario_M
{
public:
	Escenario_M();
	~Escenario_M();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

private:
	void RenderPieza(GLuint uniformModel, GLuint uniformColor,
		const glm::mat4& base,
		glm::vec3 offset, glm::vec3 escala,
		const glm::vec3& color);

	Mesh* cubo;
	Texture textura;
	Material material;
	glm::vec3 position;   // posición base del escenario en el mapa
	float rotationY;
	bool initialized;
};
