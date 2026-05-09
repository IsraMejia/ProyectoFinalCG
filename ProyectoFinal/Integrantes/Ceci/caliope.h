#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Mesh.h"
#include "../../dependencias/Texture.h"
#include "../../dependencias/Material.h"

class Caliope
{
public:
	Caliope();
	~Caliope();

	void Initialize();
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		GLuint uniformTextureOffset,
		const float toRadians, float deltaTime);

private:
	Model caliopeModel;
	Model tubosModel;
	Mesh* notasMesh;      // quad en cruz para las notas musicales
	Texture notasTexture; // musica.png
	Material material;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationY;
	bool initialized;

	// Animación
	float tiempoAnim;     // acumulador de tiempo
	float vibracionTubos; // escala oscilatoria de los tubos
	float notasOffsetV;   // offset UV vertical para las notas (suben)
};
