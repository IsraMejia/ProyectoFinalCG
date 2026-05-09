#include "personaje_principal.h"
#include <cmath>
#include <algorithm>  // Para std::max y std::min
#include <glfw3.h>    // Para glfwGetTime()

MasterChief::MasterChief()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);  // Centro de la isla
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationY = 0.0f;
	material = Material(0.3f, 4);
	
	// Inicializar angulos de animacion en 0 (pose neutral)
	anguloBrazoIzq = 0.0f;
	anguloBrazoDer = 0.0f;
	anguloAnteBrazoIzq = 0.0f;
	anguloAnteBrazoDer = 0.0f;
	anguloPiernaIzq = 0.0f;
	anguloPiernaDer = 0.0f;
	anguloPantorrillaIzq = 0.0f;
	anguloPantorrillaDer = 0.0f;
	
	// Inicializar sistema de tiempo de animacion
	ultimoTiempoAnimacion = 0.0;
	tiempoAnimacion = 0.0f;
	velocidadAnimacion = 2.0f;  // 2 ciclos por segundo (ajustable)
	
	// Parametros de animacion (ajustables para diferentes estilos de caminata)
	amplitudBrazo = 15.0f;      // Brazos se mueven +/- 15 grados
	amplitudPierna = 25.0f;     // Piernas se mueven +/- 25 grados
	flexionCodo = 8.0f;         // Codos se flexionan 8 grados adicionales
	flexionRodilla = 20.0f;     // Rodillas se flexionan 20 grados adicionales
	velocidadRetorno = 5.0f;    // Velocidad de retorno a pose neutral (grados por segundo)
}

MasterChief::~MasterChief()
{
}

void MasterChief::Initialize()
{
	// Cargar todos los modelos del Jefe Maestro
	torso.LoadModel("Models/chief_torso.obj");
	casco.LoadModel("Models/chief_casco.obj");
	brazo_izq.LoadModel("Models/chief_brazo_izq.obj");
	brazo_der.LoadModel("Models/chief_brazo_der.obj");
	antebrazo_izq.LoadModel("Models/chief_antebrazo_izq.obj");
	antebrazo_der.LoadModel("Models/chief_antebrazo_der.obj");
	pierna_izq.LoadModel("Models/chief_pierna_izq.obj");
	pierna_der.LoadModel("Models/chief_pierna_der.obj");
	pantorrilla_izq.LoadModel("Models/chief_pantorrilla_izq.obj");
	pantorrilla_der.LoadModel("Models/chief_pantorrilla_der.obj");
}

void MasterChief::Update(bool isWalkingKeyPressed, float deltaTime)
{
	UpdateWalkingAnimation(isWalkingKeyPressed, deltaTime);
}

void MasterChief::Move(const glm::vec3& direction, float deltaTime, float speed)
{
	// Mover solo en el plano XZ (sin cambio en Y)
	glm::vec3 movement = direction * speed * deltaTime;
	movement.y = 0.0f;  // Forzar Y a 0 para mantener altura constante
	
	position += movement;
}

void MasterChief::UpdateWalkingAnimation(bool isWalking, float deltaTime)
{
	// Usar tiempo real de GLFW (independiente del deltaTime del main)
	double tiempoActual = glfwGetTime();
	
	// Primera vez que se llama
	if (ultimoTiempoAnimacion == 0.0)
	{
		ultimoTiempoAnimacion = tiempoActual;
		return;
	}
	
	// Calcular deltaTime real
	double deltaTimeReal = tiempoActual - ultimoTiempoAnimacion;
	ultimoTiempoAnimacion = tiempoActual;
	
	if (isWalking)
	{
		// Acumular tiempo de animacion
		tiempoAnimacion += (float)deltaTimeReal * velocidadAnimacion;
		
		// Mantener el tiempo en el rango [0, 2*PI] para evitar overflow
		const float DOS_PI = 2.0f * 3.14159265f;
		if (tiempoAnimacion >= DOS_PI)
		{
			tiempoAnimacion -= DOS_PI;
		}
		
		// Calcular angulos usando funciones sinusoidales
		// Los brazos y piernas opuestas se mueven en fase contraria
		float faseBrazoIzq = std::sin(tiempoAnimacion);
		float faseBrazoDer = -faseBrazoIzq;  // Opuesto
		float fasePiernaIzq = -faseBrazoIzq; // Opuesto al brazo izquierdo
		float fasePiernaDer = faseBrazoIzq;  // Opuesto a pierna izquierda
		
		// BRAZOS - Movimiento de pendulo desde el hombro (adelante/atras)
		anguloBrazoIzq = amplitudBrazo * faseBrazoIzq;
		anguloBrazoDer = amplitudBrazo * faseBrazoDer;
		
		// ANTEBRAZOS - Flexion adicional del codo cuando el brazo va hacia adelante
		if (faseBrazoIzq > 0.0f)
		{
			// Brazo izquierdo hacia adelante - flexionar codo
			anguloAnteBrazoIzq = flexionCodo * faseBrazoIzq;
		}
		else
		{
			// Brazo izquierdo hacia atras - sin flexion extra
			anguloAnteBrazoIzq = 0.0f;
		}
		
		if (faseBrazoDer > 0.0f)
		{
			// Brazo derecho hacia adelante - flexionar codo
			anguloAnteBrazoDer = flexionCodo * faseBrazoDer;
		}
		else
		{
			// Brazo derecho hacia atras - sin flexion extra
			anguloAnteBrazoDer = 0.0f;
		}
		
		// PIERNAS - Rotacion en X (adelante/atras)
		anguloPiernaIzq = amplitudPierna * fasePiernaIzq;
		anguloPiernaDer = amplitudPierna * fasePiernaDer;
		
		// PANTORRILLAS - Flexion adicional cuando la pierna va hacia adelante
		anguloPantorrillaIzq = (fasePiernaIzq > 0.0f) ? flexionRodilla * fasePiernaIzq : 0.0f;
		anguloPantorrillaDer = (fasePiernaDer > 0.0f) ? flexionRodilla * fasePiernaDer : 0.0f;
	}
	else
	{
		// No esta caminando - retornar suavemente a pose neutral
		float factorRetorno = velocidadRetorno * (float)deltaTimeReal;
		
		// Interpolar hacia 0 usando lerp manual
		auto lerpToZero = [factorRetorno](float& angulo) {
			if (std::abs(angulo) < 0.1f)
			{
				angulo = 0.0f;  // Snap a 0 si esta muy cerca
			}
			else
			{
				// Mover hacia 0
				if (angulo > 0.0f)
					angulo = std::max(0.0f, angulo - factorRetorno);
				else
					angulo = std::min(0.0f, angulo + factorRetorno);
			}
		};
		
		lerpToZero(anguloBrazoIzq);
		lerpToZero(anguloBrazoDer);
		lerpToZero(anguloAnteBrazoIzq);
		lerpToZero(anguloAnteBrazoDer);
		lerpToZero(anguloPiernaIzq);
		lerpToZero(anguloPiernaDer);
		lerpToZero(anguloPantorrillaIzq);
		lerpToZero(anguloPantorrillaDer);
	}
}

void MasterChief::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const float toRadians)
{
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);

	// Matriz base del personaje - centrado en (0,0) con altura ajustada
	glm::mat4 modelBase(1.0);
	modelBase = glm::translate(modelBase, position);
	modelBase = glm::rotate(modelBase, rotationY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	modelBase = glm::scale(modelBase, scale);

	// ========== TORSO (raíz de la jerarquía) ==========
	glm::mat4 modelTorso = modelBase;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorso));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	torso.RenderModel();

	// ========== CASCO (hijo del torso) ==========
	glm::mat4 modelCasco = modelTorso;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCasco));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	casco.RenderModel();

	// ========== BRAZO IZQUIERDO (hijo del torso) ==========
	// Posicion inicial: ajustar posicion para enlazar con el torso
	glm::mat4 modelBrazoIzq = modelTorso;
	// Trasladar para posicionar el hombro correctamente (X: hacia centro, Y: altura del hombro)
	modelBrazoIzq = glm::translate(modelBrazoIzq, glm::vec3(-0.27f, 0.2f, 0.0f));
	// Rotar -35 grados en Z para bajar el brazo izquierdo (menos que 45 para mas separacion)
	modelBrazoIzq = glm::rotate(modelBrazoIzq, -19.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	// Aplicar movimiento de pendulo (eje X) para la animacion de caminata
	modelBrazoIzq = glm::rotate(modelBrazoIzq, anguloBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	brazo_izq.RenderModel();

	// ========== ANTEBRAZO IZQUIERDO (hijo del brazo izquierdo) ==========
	glm::mat4 modelAnteBrazoIzq = modelBrazoIzq;
	// El antebrazo hereda el movimiento del brazo
	// Solo aplicar flexion adicional del codo
	modelAnteBrazoIzq = glm::rotate(modelAnteBrazoIzq, -anguloAnteBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAnteBrazoIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	antebrazo_izq.RenderModel();

	// ========== BRAZO DERECHO (hijo del torso) ==========
	// Posicion inicial: ajustar posicion para enlazar con el torso
	glm::mat4 modelBrazoDer = modelTorso;
	// Trasladar para posicionar el hombro correctamente (X: hacia centro, Y: altura del hombro)
	modelBrazoDer = glm::translate(modelBrazoDer, glm::vec3(0.27f, 0.2f, 0.0f));
	// Rotar +35 grados en Z para bajar el brazo derecho (menos que 45 para mas separacion)
	modelBrazoDer = glm::rotate(modelBrazoDer, 19.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	// Aplicar movimiento de pendulo (eje X) para la animacion de caminata
	modelBrazoDer = glm::rotate(modelBrazoDer, anguloBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	brazo_der.RenderModel();

	// ========== ANTEBRAZO DERECHO (hijo del brazo derecho) ==========
	glm::mat4 modelAnteBrazoDer = modelBrazoDer;
	// El antebrazo hereda el movimiento del brazo
	// Solo aplicar flexion adicional del codo
	modelAnteBrazoDer = glm::rotate(modelAnteBrazoDer, -anguloAnteBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAnteBrazoDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	antebrazo_der.RenderModel();

	// ========== PIERNA IZQUIERDA (hijo del torso) ==========
	glm::mat4 modelPiernaIzq = modelTorso;
	modelPiernaIzq = glm::rotate(modelPiernaIzq, anguloPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	pierna_izq.RenderModel();

	// ========== PANTORRILLA IZQUIERDA (hijo de pierna izquierda) ==========
	glm::mat4 modelPantorrillaIzq = modelPiernaIzq;
	modelPantorrillaIzq = glm::rotate(modelPantorrillaIzq, -anguloPantorrillaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPantorrillaIzq));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	pantorrilla_izq.RenderModel();

	// ========== PIERNA DERECHA (hijo del torso) ==========
	glm::mat4 modelPiernaDer = modelTorso;
	modelPiernaDer = glm::rotate(modelPiernaDer, anguloPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	pierna_der.RenderModel();

	// ========== PANTORRILLA DERECHA (hijo de pierna derecha) ==========
	glm::mat4 modelPantorrillaDer = modelPiernaDer;
	modelPantorrillaDer = glm::rotate(modelPantorrillaDer, -anguloPantorrillaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPantorrillaDer));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	pantorrilla_der.RenderModel();
}
