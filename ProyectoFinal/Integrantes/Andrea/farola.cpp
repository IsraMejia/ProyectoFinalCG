#include "farola.h"

// Implementación de Farola con jerarquía (farola + cámara opcional)
// Las cámaras rotan con patrón oscilante

// Inicializar miembros estáticos
Model* Farola::farolaModel = nullptr;
Model* Farola::camaraModel = nullptr;
bool Farola::modelsLoaded = false;

Farola::Farola()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY = 0.0f;
	tieneCamara = false;
	cameraRotationY = 0.0f;
	cameraRotationSpeed = 0.1f;
	rotatingRight = true;
	material = Material(0.3f, 4);
}

Farola::Farola(glm::vec3 pos, float rotation, bool conCamara, glm::vec3 escala)
{
	position = pos;
	scale = glm::vec3(2.0f, 2.0f, 2.0f); 
	rotationY = rotation;
	tieneCamara = conCamara;
	cameraRotationY = 0.0f;
	cameraRotationSpeed = 0.1f;
	rotatingRight = true;
	material = Material(0.3f, 4);
}

Farola::~Farola()
{
}

void Farola::Initialize()
{
	if (!modelsLoaded)
	{
		if (farolaModel == nullptr)
		{
			farolaModel = new Model();
		}
		if (camaraModel == nullptr)
		{
			camaraModel = new Model();
		}
		
		farolaModel->LoadModel("Integrantes/Andrea/modelos/FarolYCam/FarolOptimizado.obj");
		camaraModel->LoadModel("Integrantes/Andrea/modelos/FarolYCam/SecCamaraOptimizada.obj");
		modelsLoaded = true;
	}
}

void Farola::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	// Inicialización lazy - solo carga los modelos una vez para todas las farolas
	if (!modelsLoaded)
	{
		Initialize();
		if (!modelsLoaded || farolaModel == nullptr)
			return; // No renderizar si falló la inicialización
	}

	glm::vec3 color(1.0f, 1.0f, 1.0f);

	// ===== RENDERIZAR FAROLA (BASE) =====
	glm::mat4 modelFarola(1.0);
	modelFarola = glm::translate(modelFarola, position);
	modelFarola = glm::rotate(modelFarola, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelFarola = glm::scale(modelFarola, scale);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFarola));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	farolaModel->RenderModel();

	// ===== RENDERIZAR CÁMARA (SI TIENE) - JERARQUÍA =====
	if (tieneCamara && camaraModel != nullptr)
	{
		if (rotatingRight)
		{
			cameraRotationY += cameraRotationSpeed;
			if (cameraRotationY >= 140.0f)
			{
				cameraRotationY = 140.0f;
				rotatingRight = false;
			}
		}
		else
		{
			cameraRotationY -= cameraRotationSpeed;
			if (cameraRotationY <= -140.0f)
			{
				cameraRotationY = -140.0f;
				rotatingRight = true;
			}
		}

		// Transformación jerárquica: cámara relativa a la farola
		glm::mat4 modelCamara = modelFarola; // Hereda la transformación de la farola
		modelCamara = glm::translate(modelCamara, glm::vec3(0.0f, -6.0f, 0.0f)); // Altura de la cámara (relativa a farola)
		modelCamara = glm::rotate(modelCamara, cameraRotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // rotación cámara 
		modelCamara = glm::scale(modelCamara, glm::vec3(2.0f, 2.0f, 2.0f)); // escala cámara

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCamara));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		material.UseMaterial(uniformSpecularIntensity, uniformShininess);

		camaraModel->RenderModel();
	}
}
