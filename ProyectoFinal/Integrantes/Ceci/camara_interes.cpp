#include "camara_interes.h"
#include <cmath>

CamaraInteres::CamaraInteres()
{
	// Centro del escenario de Miku
	target = glm::vec3(68.70f, 1.0f, -3.70f);

	activa           = false;
	teclaCPresionada = false;

	// El escenario tiene rot -90 en Y, su frente apunta en +X
	// angulo=PI coloca la camara en -X (frente al escenario desde afuera)
	angulo           = 3.14159f;
	radio            = 35.0f;
	altura           = 6.0f;
	velocidadOrbita  = 0.0f;
	velocidadZoom    = 0.0f;
	radioMin         = 20.0f;
	radioMax         = 35.0f;

	fase       = 0;
	tiempoFase = 0.0f;
}

CamaraInteres::~CamaraInteres()
{
}

void CamaraInteres::SetActiva(bool a)
{
	activa = a;
	if (activa)
	{
		angulo     = 3.14159f; // frente al escenario
		radio      = radioMax;
		altura     = 6.0f;
		fase       = 0;
		tiempoFase = 0.0f;
	}
}

bool CamaraInteres::IsActiva() const
{
	return activa;
}

void CamaraInteres::HandleInput(bool* keys)
{
	if (keys[GLFW_KEY_C])
	{
		if (!teclaCPresionada)
		{
			SetActiva(!activa);
			teclaCPresionada = true;
		}
	}
	else
	{
		teclaCPresionada = false;
	}
}

void CamaraInteres::Update(float deltaTime)
{
	if (!activa) return;

	tiempoFase += deltaTime;

	// Centro angular = PI (frente al escenario)
	// Arco de barrido = 45 grados a cada lado
	const float CENTER = 3.14159f;
	const float ARC    = 0.785f;  // 45 grados en radianes

	switch (fase)
	{
	case 0: // Barrido lento de izquierda a derecha
	{
		float t      = tiempoFase / 120.0f;
		if (t > 1.0f) t = 1.0f;
		float smooth = t * t * (3.0f - 2.0f * t);
		angulo = (CENTER - ARC) + (2.0f * ARC) * smooth;
		radio  = radioMax;
		altura = 6.0f;
		if (tiempoFase >= 120.0f)
		{
			fase = 1;
			tiempoFase = 0.0f;
		}
		break;
	}

	case 1: // Regresa al centro y se acerca 
	{
		float t      = tiempoFase / 40.0f;
		if (t > 1.0f) t = 1.0f;
		float smooth = t * t * (3.0f - 2.0f * t);
		float anguloInicio = CENTER + ARC;
		angulo = anguloInicio + (CENTER - anguloInicio) * smooth;
		radio  = radioMax + (radioMin - radioMax) * smooth;
		altura = 6.0f + (3.5f - 6.0f) * smooth;
		if (tiempoFase >= 40.0f)
		{
			fase = 2;
			tiempoFase = 0.0f;
		}
		break;
	}

	case 2: // Pausa en el centro cercano
	{
		angulo = CENTER;
		radio  = radioMin;
		altura = 3.5f;
		if (tiempoFase >= 25.0f)
		{
			fase = 0;
			tiempoFase = 0.0f;
		}
		break;
	}
	}
}

glm::mat4 CamaraInteres::GetViewMatrix() const
{
	glm::vec3 pos = GetPosition();
	return glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 CamaraInteres::GetPosition() const
{
	float x = target.x + radio * cos(angulo);
	float z = target.z + radio * sin(angulo);
	float y = target.y + altura;
	return glm::vec3(x, y, z);
}
