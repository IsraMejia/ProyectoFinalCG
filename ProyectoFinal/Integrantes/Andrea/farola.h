#pragma once

#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Farola
{
public:
	Farola();
	Farola(glm::vec3 pos, float rotation = 0.0f, bool conCamara = false, glm::vec3 escala = glm::vec3(1.0f));
	~Farola();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const float toRadians);

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetLightPosition() const { 
		// La luz está en la parte superior del farol
		// Ajustamos la altura según la escala del farol
		return glm::vec3(position.x, position.y + 8.0f * scale.y, position.z); 
	}
	bool TieneCamara() const { return tieneCamara; }

private:
	// Modelos compartidos (estáticos) para optimizar memoria
	static Model* farolaModel;
	static Model* camaraModel;
	static bool modelsLoaded;

	// Propiedades de la instancia
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool tieneCamara;

	// Rotación de la cámara (oscilante)
	float cameraRotationY;
	float cameraRotationSpeed;
	bool rotatingRight; // true = girando a la derecha, false = girando a la izquierda
};
