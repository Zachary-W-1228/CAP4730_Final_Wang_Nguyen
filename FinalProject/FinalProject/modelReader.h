#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>

using namespace std;

class modelReader {
public:
	modelReader(const char* modelPath);
	vector<float> returnVertices();
	vector<int> returnIndices();
	int returnVerticesNum();
private:
	void readFile(const char* modelPath);
	vector<float> vertices;
	vector<int> Indices;
	int numVertices;
};