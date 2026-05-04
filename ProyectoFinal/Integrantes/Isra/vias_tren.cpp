#include "vias_tren.h"
#include <cmath>

ViasTren::ViasTren()
{
	outerRailMesh = nullptr;
	innerRailMesh = nullptr;
	tieMesh = nullptr;
	initialized = false;

	// Dimensiones de la seccion transversal del riel
	railWidth = 0.003f;   // ancho del riel
	railHeight = 0.002f;  // alto del riel

	// Dimensiones de los ovalos - reducido 6%
	outerRadiusX = 0.07941f;  // 94% de 0.08448
	outerRadiusZ = 0.05956f;  // 94% de 0.06336

	// Separacion entre rieles
	float separation = 0.006f;

	innerRadiusX = outerRadiusX - separation;
	innerRadiusZ = outerRadiusZ - separation;

	// Numero de segmentos para suavidad
	numSegments = 120;

	// Dimensiones de las tablas de madera - 30% mas pequeñas
	tieWidth = 0.0007f;    // ancho de la tabla (70% de 0.001)
	tieHeight = 0.00105f;  // alto de la tabla (70% de 0.0015)
	tieLength = (separation + railWidth + 0.002f) * 0.7f; // largo reducido 30%

	// Material metalico plateado para los rieles
	railMaterial = Material(1.0f, 64); // alta especularidad para metal plateado

	// Material cafe para las tablas de madera
	tieMaterial = Material(0.2f, 4); // baja especularidad para madera
}

ViasTren::~ViasTren()
{
	if (outerRailMesh)
	{
		delete outerRailMesh;
		outerRailMesh = nullptr;
	}
	if (innerRailMesh)
	{
		delete innerRailMesh;
		innerRailMesh = nullptr;
	}
	if (tieMesh)
	{
		delete tieMesh;
		tieMesh = nullptr;
	}
}

void ViasTren::CreateRailRing(float radiusX, float radiusZ, float railWidth, float railHeight)
{
	const float PI = 3.14159265f;
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	float halfWidth = railWidth / 2.0f;
	float halfHeight = railHeight / 2.0f;

	// Generar vertices del anillo
	// Cada segmento tiene 8 vertices (seccion rectangular con 4 vertices, duplicados para interior/exterior)
	for (int i = 0; i <= numSegments; i++)
	{
		float angle = (2.0f * PI * i) / numSegments;
		float nextAngle = (2.0f * PI * (i + 1)) / numSegments;

		// Posicion central en el ovalo
		float x = radiusX * cos(angle);
		float z = radiusZ * sin(angle);

		// Calcular tangente para orientar la seccion rectangular
		float dx = radiusX * cos(nextAngle) - x;
		float dz = radiusZ * sin(nextAngle) - z;
		float tangentAngle = atan2(dx, dz);

		// Vectores perpendiculares para la seccion transversal
		float perpX = -sin(tangentAngle);
		float perpZ = cos(tangentAngle);

		// 4 vertices de la seccion rectangular en este punto del anillo
		// Esquina inferior izquierda
		float x1 = x + perpX * halfWidth;
		float z1 = z + perpZ * halfWidth;
		float y1 = -halfHeight;

		// Esquina inferior derecha
		float x2 = x - perpX * halfWidth;
		float z2 = z - perpZ * halfWidth;
		float y2 = -halfHeight;

		// Esquina superior derecha
		float x3 = x - perpX * halfWidth;
		float z3 = z - perpZ * halfWidth;
		float y3 = halfHeight;

		// Esquina superior izquierda
		float x4 = x + perpX * halfWidth;
		float z4 = z + perpZ * halfWidth;
		float y4 = halfHeight;

		// Normal apuntando hacia afuera (simplificada)
		float nx = x / sqrt(x*x + z*z);
		float nz = z / sqrt(x*x + z*z);

		// Agregar vertices (posicion + UV + normal)
		// Cara superior
		vertices.insert(vertices.end(), {
			x4, y4, z4,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			x3, y3, z3,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		});

		// Cara inferior
		vertices.insert(vertices.end(), {
			x1, y1, z1,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			x2, y2, z2,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		});

		// Cara exterior
		vertices.insert(vertices.end(), {
			x4, y4, z4,  0.0f, 1.0f,  perpX, 0.0f, perpZ,
			x1, y1, z1,  0.0f, 0.0f,  perpX, 0.0f, perpZ,
		});

		// Cara interior
		vertices.insert(vertices.end(), {
			x3, y3, z3,  1.0f, 1.0f,  -perpX, 0.0f, -perpZ,
			x2, y2, z2,  1.0f, 0.0f,  -perpX, 0.0f, -perpZ,
		});
	}

	// Generar indices para conectar los segmentos
	for (int i = 0; i < numSegments; i++)
	{
		int base = i * 8;
		int nextBase = (i + 1) * 8;

		// Cara superior (2 triangulos)
		indices.push_back(base + 0);
		indices.push_back(nextBase + 0);
		indices.push_back(nextBase + 1);

		indices.push_back(base + 0);
		indices.push_back(nextBase + 1);
		indices.push_back(base + 1);

		// Cara inferior
		indices.push_back(base + 2);
		indices.push_back(nextBase + 2);
		indices.push_back(nextBase + 3);

		indices.push_back(base + 2);
		indices.push_back(nextBase + 3);
		indices.push_back(base + 3);

		// Cara exterior
		indices.push_back(base + 4);
		indices.push_back(nextBase + 4);
		indices.push_back(nextBase + 5);

		indices.push_back(base + 4);
		indices.push_back(nextBase + 5);
		indices.push_back(base + 5);

		// Cara interior
		indices.push_back(base + 6);
		indices.push_back(nextBase + 6);
		indices.push_back(nextBase + 7);

		indices.push_back(base + 6);
		indices.push_back(nextBase + 7);
		indices.push_back(base + 7);
	}

	// Crear el mesh
	Mesh* mesh = new Mesh();
	mesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());

	// Asignar al mesh correspondiente
	if (radiusX == outerRadiusX)
		outerRailMesh = mesh;
	else
		innerRailMesh = mesh;
}

void ViasTren::CreateTieMesh()
{
	// Crear un prisma rectangular para las tablas de madera
	float hw = tieLength / 2.0f;  // mitad del largo (cruza entre rieles)
	float hh = tieHeight / 2.0f;  // mitad del alto
	float hl = tieWidth / 2.0f;   // mitad del ancho

	GLfloat vertices[] = {
		// Posicion (x,y,z), UV (u,v), Normal (nx,ny,nz)
		// Cara frontal (Z+)
		-hw, -hh,  hl,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 hw, -hh,  hl,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 hw,  hh,  hl,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
		-hw,  hh,  hl,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

		// Cara trasera (Z-)
		-hw, -hh, -hl,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
		-hw,  hh, -hl,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
		 hw,  hh, -hl,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
		 hw, -hh, -hl,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,

		// Cara superior (Y+)
		-hw,  hh,  hl,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		 hw,  hh,  hl,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		 hw,  hh, -hl,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
		-hw,  hh, -hl,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,

		// Cara inferior (Y-)
		-hw, -hh,  hl,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		-hw, -hh, -hl,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
		 hw, -hh, -hl,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
		 hw, -hh,  hl,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,

		// Cara derecha (X+)
		 hw, -hh,  hl,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 hw, -hh, -hl,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		 hw,  hh, -hl,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
		 hw,  hh,  hl,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,

		// Cara izquierda (X-)
		-hw, -hh,  hl,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
		-hw,  hh,  hl,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
		-hw,  hh, -hl,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
		-hw, -hh, -hl,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f
	};

	unsigned int indices[] = {
		// Frontal
		0, 1, 2,  0, 2, 3,
		// Trasera
		4, 5, 6,  4, 6, 7,
		// Superior
		8, 9, 10,  8, 10, 11,
		// Inferior
		12, 13, 14,  12, 14, 15,
		// Derecha
		16, 17, 18,  16, 18, 19,
		// Izquierda
		20, 21, 22,  20, 22, 23
	};

	tieMesh = new Mesh();
	tieMesh->CreateMesh(vertices, indices, 192, 36);
}

void ViasTren::Initialize()
{
	if (!initialized)
	{
		// Crear anillo exterior
		CreateRailRing(outerRadiusX, outerRadiusZ, railWidth, railHeight);

		// Crear anillo interior
		CreateRailRing(innerRadiusX, innerRadiusZ, railWidth, railHeight);

		// Crear mesh de las tablas
		CreateTieMesh();

		initialized = true;
	}
}

void ViasTren::Render(GLuint uniformModel, GLuint uniformColor,
	GLuint uniformSpecularIntensity, GLuint uniformShininess,
	const glm::mat4& islandTransform, const float toRadians)
{
	// Auto-inicializar si no se ha hecho
	if (!initialized)
	{
		Initialize();
	}

	// Altura de la isla en espacio local - 1 unidad mas abajo
	float islandHeight = 0.0194f + 0.001f - 0.0008f;

	// Offset para mover las vias: izquierda (X-) y arriba (Z-)
	float offsetX = -0.005f;  // mover a la izquierda
	float offsetZ = -0.015f;  // mover hacia arriba (Z negativo)

	const float PI = 3.14159265f;

	// Renderizar anillo exterior (plateado)
	glm::mat4 model = islandTransform;
	model = glm::translate(model, glm::vec3(offsetX, islandHeight, offsetZ));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	railMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	outerRailMesh->RenderMesh();

	// Renderizar anillo interior (plateado)
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	railMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	innerRailMesh->RenderMesh();

	// Renderizar tablas de madera cada 5 grados (360/5 = 72 tablas)
	int numTies = 72;
	for (int i = 0; i < numTies; i++)
	{
		float angle = (2.0f * PI * i) / numTies;
		float nextAngle = (2.0f * PI * (i + 1)) / numTies;

		// Calcular posicion en el punto medio entre los dos rieles
		float avgRadiusX = (outerRadiusX + innerRadiusX) / 2.0f;
		float avgRadiusZ = (outerRadiusZ + innerRadiusZ) / 2.0f;

		float x = avgRadiusX * cos(angle);
		float z = avgRadiusZ * sin(angle);

		// Calcular tangente para orientar la tabla perpendicular a la via
		float dx = avgRadiusX * cos(nextAngle) - x;
		float dz = avgRadiusZ * sin(nextAngle) - z;
		float tangentAngle = atan2(dx, dz);

		// Posicionar tabla en el punto mas alto (encima de los rieles)
		float tieY = islandHeight + railHeight / 2.0f;

		model = islandTransform;
		model = glm::translate(model, glm::vec3(x + offsetX, tieY, z + offsetZ));
		model = glm::rotate(model, tangentAngle, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tieMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tieMesh->RenderMesh();
	}
}
