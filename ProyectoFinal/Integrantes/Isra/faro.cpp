#include "faro.h"

Faro::Faro()
{
	// Posicion: Atras (sur) de la estacion del tren y la torre Forerunner
	// Estacion: (50, 0.81, 70), Torre Forerunner: (80, -5, 95)
	// Faro: entre ambos en X, pero mas al sur (Z mayor que 95)
	position = glm::vec3(20.0f, 25.0f, 140.0f);
	scale = glm::vec3(20.0f, 20.0f, 20.0f);
	rotationY = 0.0f; // Sin rotacion inicial
	initialized = false;

	// Material con especularidad alta para simular superficie pintada
	material = Material(0.8f, 32);

	// Posicion de la luz en la parte superior del faro
	lightPosition = glm::vec3(19.85f, 45.83f, 140.16f);

	// ============ CONFIGURACION DE LA LUZ DEL FARO ============
	// Rotacion de la luz
	lightRotationY = 0.0f;
	lightRotationSpeed = 0.60f;  // AJUSTABLE: Velocidad de rotacion en grados/segundo (30 = lento, 90 = rapido)

	// Direccion inicial de la luz (apuntando hacia el norte, -Z)
	glm::vec3 lightDirection(0.0f, -0.3f, -1.0f);  // AJUSTABLE: Direccion del haz (Y negativo = hacia abajo, Z negativo = hacia norte)
	lightDirection = glm::normalize(lightDirection);

	// SpotLight del faro con haz direccional rotatorio
	faroLight = SpotLight(
		// Color RGB (amarillo)
		1.0f, 1.0f, 0.0f,           // AJUSTABLE: Color amarillo (R=1.0, G=1.0, B=0.0)
		
		// Intensidades
		0.3f,                        // AJUSTABLE: Ambient (0.0-1.0) - luz ambiental base
		2.0f,                        // AJUSTABLE: Diffuse (0.0-5.0) - intensidad del haz principal
		
		// Posicion de la luz
		lightPosition.x, lightPosition.y, lightPosition.z,
		
		// Direccion del haz
		lightDirection.x, lightDirection.y, lightDirection.z,
		
		// Atenuacion (controla el alcance de la luz)
		0.1f,                        // AJUSTABLE: Constant (0.1-1.0) - atenuacion constante
		0.01f,                       // AJUSTABLE: Linear (0.001-0.1) - atenuacion lineal
		0.001f,                      // AJUSTABLE: Exponent (0.0001-0.01) - atenuacion exponencial
		
		// Angulo del cono de luz
		28.0f                        // AJUSTABLE: Edge (5.0-45.0) - angulo del cono en grados (menor = haz mas estrecho)
	);
}

Faro::~Faro()
{
}

void Faro::Initialize()
{
	if (!initialized)
	{
		faroModel.LoadModel("Models/faro.obj");
		initialized = true;
	}
}

void Faro::Update(float deltaTime)
{
	// Actualizar rotacion de la luz
	lightRotationY += lightRotationSpeed * deltaTime;
	
	// Mantener el angulo entre 0 y 360 grados
	if (lightRotationY >= 360.0f)
	{
		lightRotationY -= 360.0f;
	}

	// Calcular nueva direccion del haz basada en la rotacion
	float radians = glm::radians(lightRotationY);
	
	// Direccion base (apuntando hacia norte con inclinacion hacia abajo)
	glm::vec3 baseDirection(0.0f, -0.3f, -1.0f);  // Debe coincidir con la direccion inicial
	
	// Rotar la direccion alrededor del eje Y
	glm::vec3 newDirection;
	newDirection.x = baseDirection.x * cos(radians) - baseDirection.z * sin(radians);
	newDirection.y = baseDirection.y;  // Mantener la inclinacion vertical
	newDirection.z = baseDirection.x * sin(radians) + baseDirection.z * cos(radians);
	
	newDirection = glm::normalize(newDirection);

	// Actualizar la direccion de la luz en el SpotLight usando SetFlash
	faroLight.SetFlash(lightPosition, newDirection);
}

void Faro::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	// Auto-inicializar si no se ha hecho
	if (!initialized)
	{
		Initialize();
	}

	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	// Transformaciones: translate -> rotate -> scale
	model = glm::translate(model, position);
	model = glm::rotate(model, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);

	// Enviar uniforms
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Renderizar
	faroModel.RenderModel();
}
