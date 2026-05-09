#pragma once

#include "CommonValues.h"
#include <vector>
#include <string>
#include<glew.h>
#include<glm.hpp>
#include<glfw3.h>
#include <gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include "Mesh.h"
#include "Shader_light.h"
class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	Skybox(std::vector<std::string> dayFaceLocations, std::vector<std::string> nightFaceLocations);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, bool isDayTime);
	void SetDayNightMode(bool isDayTime);
		
	~Skybox();
private:
	Mesh* skyMesh;
	Shader* skyShader;
	GLuint textureIdDay;
	GLuint textureIdNight;
	GLuint textureId; // Para compatibilidad con constructor antiguo
	GLuint uniformProjection, uniformView;
	bool hasDayNightTextures;

};

