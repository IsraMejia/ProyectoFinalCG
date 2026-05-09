#include "escenario_M.h"


Escenario_M::Escenario_M()
{
	position    = glm::vec3(68.70f, -3.0f, -3.70f);
	rotationY   = -90.0f;
	material  = Material(0.3f, 4);   // Material_opaco
	cubo        = nullptr;
	initialized = false;
}

Escenario_M::~Escenario_M()
{
	if (cubo)
	{
		cubo->ClearMesh();
		delete cubo;
		cubo = nullptr;
	}
}

void Escenario_M::Initialize()
{
	if (initialized) return;

	// Cubo unitario con normales por cara (igual que CrearCuboIluminado en main)
	unsigned int indices[] = {
		// Frontal
		 0,  1,  2,   2,  3,  0,
		// Derecha
		 4,  5,  6,   6,  7,  4,
		// Trasera
		 8,  9, 10,  10, 11,  8,
		// Izquierda
		12, 13, 14,  14, 15, 12,
		// Abajo
		16, 17, 18,  18, 19, 16,
		// Arriba
		20, 21, 22,  22, 23, 20
	};

	GLfloat vertices[] = {
		// x      y      z       S     T       NX     NY     NZ
		// Frontal (normal -Z, indices horario visto desde +Z)
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
		// Derecha (normal -X)
		 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		// Trasera (normal +Z)
		 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,
		// Izquierda (normal +X)
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
		// Abajo (normal +Y)
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		// Arriba (normal -Y)
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f
	};

	cubo = new Mesh();
	cubo->CreateMesh(vertices, indices, 192, 36);

	textura = Texture("Textures/miku_textura.png");
	textura.LoadTextureA();

	initialized = true;
}

// Helper: aplica offset y escala sobre la base, envía uniforms y dibuja
void Escenario_M::RenderPieza(GLuint uniformModel, GLuint uniformColor,
	const glm::mat4& base,
	glm::vec3 offset, glm::vec3 escala,
	const glm::vec3& color)
{
	glm::mat4 model = base;
	if (offset != glm::vec3(0.0f))
		model = glm::translate(model, offset);
	model = glm::scale(model, escala);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	cubo->RenderMesh();
}

void Escenario_M::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	if (!initialized) Initialize();

	textura.UseTexture();
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Colores — todos en blanco para que la textura se vea sin modificar
	const glm::vec3 colorTubos    = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 colorEscenario= glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 colorPared    = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 colorGradas   = glm::vec3(1.0f, 1.0f, 1.0f);

	// Base del escenario: translate -> rotate -> escala global
	glm::mat4 base = glm::mat4(1.0f);
	base = glm::translate(base, position);
	base = glm::rotate(base, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	base = glm::scale(base, glm::vec3(2.5f, 1.0f, 2.5f)); // más ancho y profundo

	// 1. Piso
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f), glm::vec3(16.0f, 4.0f, 10.0f), colorEscenario);

	// 2. Pasarela
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 0.0f, 7.5f), glm::vec3(4.0f, 4.0f, 5.0f), colorEscenario);

	// 3. Postes verticales
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(-7.5f, 6.0f, -4.5f), glm::vec3(0.5f, 12.0f, 0.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3( 7.5f, 6.0f, -4.5f), glm::vec3(0.5f, 12.0f, 0.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(-7.5f, 6.0f,  4.5f), glm::vec3(0.5f, 12.0f, 0.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3( 7.5f, 6.0f,  4.5f), glm::vec3(0.5f, 12.0f, 0.5f), colorTubos);

	// 4. Tubos horizontales superiores
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 11.75f,  4.5f), glm::vec3(15.5f, 0.5f, 0.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 11.75f, -4.5f), glm::vec3(15.5f, 0.5f, 0.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(-7.5f, 11.75f, 0.0f), glm::vec3(0.5f, 0.5f, 9.5f), colorTubos);

	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3( 7.5f, 11.75f, 0.0f), glm::vec3(0.5f, 0.5f, 9.5f), colorTubos);

	// 5. Techo
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 12.25f, 0.0f), glm::vec3(16.5f, 0.5f, 10.5f), colorPared);

	// 6. Telón trasero
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 6.0f, -4.75f), glm::vec3(15.0f, 11.5f, 0.2f), colorPared);

	// 7. GRADAS (frente al escenario, jerarquicas sobre la base)
	// Escalón 1 (más cercano al escenario, más bajo)
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, 0.5f, 13.0f), glm::vec3(14.0f, 1.0f, 2.0f), colorGradas);

	// Escalón 2
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, -0.5f, 15.5f), glm::vec3(14.0f, 1.0f, 2.0f), colorGradas);

	// Escalón 3
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, -1.5f, 18.0f), glm::vec3(14.0f, 1.0f, 2.0f), colorGradas);

	// Escalón 4 (más alejado, más bajo)
	RenderPieza(uniformModel, uniformColor, base,
		glm::vec3(0.0f, -2.5f, 20.5f), glm::vec3(14.0f, 1.0f, 2.0f), colorGradas);
}
