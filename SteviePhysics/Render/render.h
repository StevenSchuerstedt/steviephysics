#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include <vector>


class renderSquare {
private:

	GLuint vao;
	glm::mat4 transform = glm::mat4(1.0f);
public:
	renderSquare(double width, glm::vec3 color);
	void render(Shader* shader);
	void setPosition(double x, double y) {
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	}
	
};

class render {
	
	
	std::vector<renderSquare*> renderSquares;
	int zoom = 6;
	void setupCamera();
public:
	GLFWwindow* window;
	int width = 768;
	int height = 768;
	Shader* standardShader = nullptr;


	int getZoomLevel() { return zoom; }
	void init();
	void enterRenderLoop(void (*appLoopFunction)());
	
	void add(renderSquare* square) {
		renderSquares.push_back(square);
	};

};

