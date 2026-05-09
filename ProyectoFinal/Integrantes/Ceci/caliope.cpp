#include "caliope.h"
#include <glfw3.h>

Caliope::Caliope()
{
	position      = glm::vec3(0.0f, 0.0f, 0.0f);
	scale         = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY     = 0.0f;
	notasMesh     = nullptr;
	tiempoAnim    = 0.0f;
	vibracionTubos= 0.0f;
	notasOffsetV  = 0.0f;
	initialized   = false;
}

Caliope::~Caliope()
{
	if (notasMesh)
	{
		notasMesh->ClearMesh();
		delete notasMesh;
		notasMesh = nullptr;
	}
}

void Caliope::Initialize()
{
	if (initialized) return;

	caliopeModel.LoadModel("Models/caliope.obj");
	tubosModel.LoadModel("Models/tubos.obj");

	// Quad en cruz para las notas (igual que vegetación/humo del código de referencia)
	unsigned int notasIndices[] = {
		0, 1, 2,  0, 2, 3,
		4, 5, 6,  4, 6, 7
	};
	GLfloat notasVertices[] = {
		// Plano frontal
		-0.5f, -0.5f,  0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		// Plano lateral (perpendicular)
		 0.0f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.0f, -0.5f,  0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	};
	notasMesh = new Mesh();
	notasMesh->CreateMesh(notasVertices, notasIndices, 64, 12);

	notasTexture = Texture("Textures/musica.png");
	notasTexture.LoadTextureA();

	material = Material(0.3f, 4);

	position  = glm::vec3(50.24f, -3.0f, -21.17f);
	scale     = glm::vec3(5.5f, 5.5f, 5.5f);
	rotationY = 0.0f;

	initialized = true;
}

void Caliope::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	GLuint uniformTextureOffset,
	const float toRadians, float deltaTime)
{
	if (!initialized) return;

	// Usar glfwGetTime() directamente para evitar acumulación incorrecta de deltaTime
	float t = (float)glfwGetTime();

	// Vibración de los tubos: oscila lentamente
	vibracionTubos = 1.0f + 0.05f * sin(t * 1.0f);

	// Offset UV de las notas: sube lentamente
	notasOffsetV = -fmod(t * 0.15f, 1.0f);

	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	glm::mat4 model(1.0f);
	glm::mat4 modelaux(1.0f);

	// Transformación base
	model = glm::mat4(1.0);
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
	modelaux = model;

	// Resetear offset antes de modelos sólidos
	glm::vec2 toffset(0.0f, 0.0f);
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

	// Cuerpo del caliope
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	caliopeModel.RenderModel();

	// Tubos con vibración en Y
	model = modelaux;
	model = glm::scale(model, glm::vec3(1.0f, vibracionTubos, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	tubosModel.RenderModel();

	// Notas musicales: salen de los tubos hacia arriba con textura animada
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	toffset = glm::vec2(0.0f, notasOffsetV);
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

	// Nota 1 - tubo izquierdo
	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.15f, 1.8f + 0.15f * sin(t * 2.0f), 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	// Nota 2 - tubo derecho, desfasada
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.15f, 1.8f + 0.15f * sin(t * 2.0f + 1.5f), 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	// Nota 3 - tubo izquierdo, más arriba
	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.2f, 2.3f + 0.15f * sin(t * 2.0f + 0.8f), 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	// Nota 4 - tubo derecho, más arriba
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.2f, 2.3f + 0.15f * sin(t * 2.0f + 2.3f), 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	// Nota 5 - centro, aún más arriba
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 2.8f + 0.15f * sin(t * 2.0f + 3.0f), 0.05f));
	model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	// Nota 6 - izquierda, nivel alto
	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.25f, 3.2f + 0.1f * sin(t * 2.5f + 1.0f), 0.0f));
	model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	notasTexture.UseTexture();
	notasMesh->RenderMesh();

	glDisable(GL_BLEND);

	// Resetear offset al terminar
	toffset = glm::vec2(0.0f, 0.0f);
	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
}
