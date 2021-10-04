#include "render.h"

void render::init() {
	glfwInit();

	window = glfwCreateWindow(width, height, "Render Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	standardShader = new Shader("Render/Shader/standard.vert", "Render/Shader/standard.frag");
	setupCamera();
}

void render::setupCamera() {
	glm::mat4 proj = glm::ortho(-1.0f * zoom, 1.0f * zoom, -(float)height / (float)width * zoom, (float)height / (float)width * zoom, 0.0f, 10.0f);
	
	standardShader->use();
	standardShader->setMat4("proj", proj);
}

void render::enterRenderLoop(void (*appLoopFunction)()) {
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//call application loop
		appLoopFunction();

		//draw all objects in scene
		for (int i = 0; i < renderSquares.size(); i++) {
			renderSquares[i]->render(standardShader);
		}


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

}




renderSquare::renderSquare(double width, glm::vec3 color) {

	GLfloat screenQuadVertices[] =
	{//	x		y
		-width, -width, color.r, color.g, color.b,
		 width, -width, color.r, color.g, color.b,
		 width,  width, color.r, color.g, color.b,
		-width,  width, color.r, color.g, color.b
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vertex Attributes
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_DYNAMIC_DRAW);
	//coordinates
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);
	//colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT) * 2));
	glEnableVertexAttribArray(1);

}

void renderSquare::render(Shader* shader) {
	shader->use();
	shader->setMat4("model", transform);
	glBindVertexArray(vao);
	glDrawArrays(GL_POLYGON, 0, 4);
}