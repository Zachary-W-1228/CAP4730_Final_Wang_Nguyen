#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "modelReader.h"

using namespace glm;
using namespace std;

modelReader::modelReader(const char* modelPath) {
    readFile(modelPath);
}

vector<int> modelReader::returnIndices() {
    return Indices;
}

vector<float> modelReader::returnVertices() {
    return vertices;
}

int modelReader::returnVerticesNum() {
    return numVertices;
}

void modelReader::readFile(const char* modelPath) {
    ifstream myfile;
    myfile.open(modelPath, ios::in);
    vector<float> Coords;

    if (!myfile.is_open())
    {
        cout << "read fail." << endl;
    }
    string line;
    while (!myfile.eof())
    {

        //Get Coords
        getline(myfile, line);
        if (line[0] == 'v' && line[1] == ' ')
        {
            line = line.substr(line.find(' ') + 1);
            float x = stof(line.substr(0, line.find(' ')));
            line = line.substr(line.find(' ') + 1);
            float y = stof(line.substr(0, line.find(' ')));
            line = line.substr(line.find(' ') + 1);
            float z = stof(line.substr(0, line.find(' ')));
            Coords.push_back(x);
            Coords.push_back(y);
            Coords.push_back(z);
        }

        //Get Verticies
        if (line[0] == 'f') {
            line = line.substr(line.find(' ') + 1);
            vector<int> face;
            while (true) {
                if (line.find_first_of("1234567890") == string::npos)
                    break;
                int vertNum = 0;
                vertNum = stoi(line.substr(0, line.find_first_not_of("1234567890")));
                face.push_back(vertNum);
                if (line.find(' ') == string::npos)
                    break;
                line = line.substr(line.find(' ') + 1);
            }
            for (int i = 1; i < face.size() - 1; i++) {
                //point1
                Indices.push_back(face.at(0) - 1);
                //point2
                Indices.push_back(face.at(i) - 1);
                //point3
                Indices.push_back(face.at(i + 1) - 1);
            }
        }
    }
    myfile.close();

    numVertices = Coords.size() / 3;
    for (int i = 0; i < numVertices; i++) {
        vertices.push_back(Coords.at(i * 3));
        vertices.push_back(Coords.at(i * 3 + 1));
        vertices.push_back(Coords.at(i * 3 + 2));
        vertices.push_back(0.0);
        vertices.push_back(0.0);
        vertices.push_back(0.0);
    }

    //Get Normals
    for (int i = 0; i < Indices.size(); i++) {
        if (i % 3 == 0) {
            unsigned int pointA = Indices.at(i);
            unsigned int pointB = Indices.at(i + 1);
            unsigned int pointC = Indices.at(i + 2);

            vec3 Apos = vec3(vertices.at(pointA * 6), vertices.at(pointA * 6 + 1), vertices.at(pointA * 6 + 2));
            vec3 Bpos = vec3(vertices.at(pointB * 6), vertices.at(pointB * 6 + 1), vertices.at(pointB * 6 + 2));
            vec3 Cpos = vec3(vertices.at(pointC * 6), vertices.at(pointC * 6 + 1), vertices.at(pointC * 6 + 2));

            vec3 normal = normalize(cross((Bpos - Apos), (Cpos - Apos)));
            //get Anormal
            vec3 normalA = vec3(vertices.at(pointA * 6 + 3), vertices.at(pointA * 6 + 4), vertices.at(pointA * 6 + 5)) + normal;
            vertices.at(pointA * 6 + 3) = normalA.x;
            vertices.at(pointA * 6 + 4) = normalA.y;
            vertices.at(pointA * 6 + 5) = normalA.z;
            //get Bnormal
            vec3 normalB = vec3(vertices.at(pointB * 6 + 3), vertices.at(pointB * 6 + 4), vertices.at(pointB * 6 + 5)) + normal;
            vertices.at(pointB * 6 + 3) = normalB.x;
            vertices.at(pointB * 6 + 4) = normalB.y;
            vertices.at(pointB * 6 + 5) = normalB.z;
            //get Cnormal
            vec3 normalC = vec3(vertices.at(pointC * 6 + 3), vertices.at(pointC * 6 + 4), vertices.at(pointC * 6 + 5)) + normal;
            vertices.at(pointC * 6 + 3) = normalC.x;
            vertices.at(pointC * 6 + 4) = normalC.y;
            vertices.at(pointC * 6 + 5) = normalC.z;
        }
    }

    for (int i = 3; i < vertices.size(); i += 6) {
        vec3 Normal = normalize(vec3(vertices.at(i), vertices.at(i + 1), vertices.at(i + 2)));
        vertices.at(i) = Normal.x;
        vertices.at(i + 1) = Normal.y;
        vertices.at(i + 2) = Normal.z;
    }

}