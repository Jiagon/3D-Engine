#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include <vector>
#include <sstream>
#include <fstream>

#include <iostream>

class Model
{
private:
	GLuint vertBuf;
	GLuint vertArr;
	unsigned int vertCount;

public:
	Model();
	~Model();

	bool buffer(std::string objFile);
	void render();
};


struct Vertex {
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 norm;
};

struct VertInd {
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};
