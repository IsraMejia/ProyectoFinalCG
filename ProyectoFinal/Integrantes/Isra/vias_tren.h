#pragma once

#include <vector>
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "../../dependencias/Mesh.h"
#include "../../dependencias/Material.h"

class ViasTren
{
public:
	ViasTren();
	~ViasTren();

	// Inicializa las vias del tren
	void Initialize();

	// Renderiza las vias (una sola llamada en el loop)
	void Render(GLuint uniformModel, GLuint uniformColor,
		GLuint uniformSpecularIntensity, GLuint uniformShininess,
		const glm::mat4& islandTransform, const float toRadians);

private:
	// Crea un anillo ovalado continuo (riel completo)
	void CreateRailRing(float radiusX, float radiusZ, float railWidth, float railHeight);

	// Crea un prisma rectangular para las tablas
	void CreateTieMesh();

	// Meshes de los dos rieles (exterior e interior)
	Mesh* outerRailMesh;
	Mesh* innerRailMesh;
	Mesh* tieMesh; // tablas de madera

	// Material de los rieles y tablas
	Material railMaterial;
	Material tieMaterial;

	// Parametros de las vias
	float scaleFactorVias; // Factor de escala para ajustar todas las dimensiones (1.0 = base, 2.0 = doble)
	float scaleFactorY;    // Factor de escala solo para el eje Y (altura de rieles y maderas)
	float offsetY;         // Desplazamiento en Y para toda la via (positivo = arriba, negativo = abajo)
	float railWidth;      // ancho de la seccion transversal del riel
	float railHeight;     // alto de la seccion transversal del riel
	float outerRadiusX;   // radio exterior del ovalo en X
	float outerRadiusZ;   // radio exterior del ovalo en Z
	float innerRadiusX;   // radio interior del ovalo en X
	float innerRadiusZ;   // radio interior del ovalo en Z
	int numSegments;      // numero de segmentos para suavidad del anillo

	// Parametros de las tablas
	float tieWidth;       // ancho de la tabla
	float tieHeight;      // alto de la tabla
	float tieLength;      // largo de la tabla (distancia entre rieles)

	bool initialized;
};