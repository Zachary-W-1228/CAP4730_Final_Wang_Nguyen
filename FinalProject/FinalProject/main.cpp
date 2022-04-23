// template based on material from learnopengl.com
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
#include <list>
#include <random>

#include "Camera.h"
#include "shaderReader.h"
#include "modelReader.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PlayerState.h"

using namespace glm;
using namespace std;


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// mouse Input setting
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;
bool buttonPress = false;

//other setting
float scaleNum = 0.1f;
float HoriRotateNum = 0.0f;
float VertRotateNum = 0.0f;

Camera camera;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Gamesetting
list <Enemy> enemies;
list <Bullet> bullets;
float enemyRate = 10.0f;
PlayerState player1(10.0f, 1.0f, 50.0f);
float EnemydeltaTime = enemyRate;
float BulletdeltaTime = player1.GetFireDelay();


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glew: load all OpenGL function pointers
    glewInit();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    shaderReader shader("Source_2.vs", "Source_2.fs");
    shaderReader cursorshader("cursor.vs", "cursor.fs");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // --------------------

    //Enemies.Meteorite
    modelReader Met_model("data/cube.obj");

    vector<float> Met_vertices = Met_model.returnVertices();
    unsigned int Met_numVertices = Met_model.returnVerticesNum();
    vector<int> Met_indices = Met_model.returnIndices();

    unsigned int Met_VBO, Met_VAO, Met_EBO;
    glGenVertexArrays(1, &Met_VAO);
    glGenBuffers(1, &Met_VBO);
    glGenBuffers(1, &Met_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(Met_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Met_VBO);
    glBufferData(GL_ARRAY_BUFFER, Met_vertices.size() * sizeof(float), &Met_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Met_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Met_indices.size() * sizeof(int), &Met_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //Enemies.Plane
    modelReader Plane_model("data/cube.obj");

    vector<float> Plane_vertices = Plane_model.returnVertices();
    unsigned int Plane_numVertices = Plane_model.returnVerticesNum();
    vector<int> Plane_indices = Plane_model.returnIndices();

    unsigned int Plane_VBO, Plane_VAO, Plane_EBO;
    glGenVertexArrays(1, &Plane_VAO);
    glGenBuffers(1, &Plane_VBO);
    glGenBuffers(1, &Plane_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(Plane_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Plane_VBO);
    glBufferData(GL_ARRAY_BUFFER, Plane_vertices.size() * sizeof(float), &Plane_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Plane_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Plane_indices.size() * sizeof(int), &Plane_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Bullet
    modelReader Bullet_model("data/cube.obj");

    vector<float> Bullet_vertices = Bullet_model.returnVertices();
    unsigned int Bullet_numVertices = Bullet_model.returnVerticesNum();
    vector<int> Bullet_indices = Bullet_model.returnIndices();

    unsigned int Bullet_VBO, Bullet_VAO, Bullet_EBO;
    glGenVertexArrays(1, &Bullet_VAO);
    glGenBuffers(1, &Bullet_VBO);
    glGenBuffers(1, &Bullet_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(Bullet_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Bullet_VBO);
    glBufferData(GL_ARRAY_BUFFER, Bullet_vertices.size() * sizeof(float), &Bullet_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Bullet_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Bullet_indices.size() * sizeof(int), &Bullet_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Cursor
    modelReader Cursor_model("data/cube.obj");

    vector<float> Cursor_vertices = Cursor_model.returnVertices();
    unsigned int Cursor_numVertices = Cursor_model.returnVerticesNum();
    vector<int> Cursor_indices = Cursor_model.returnIndices();

    unsigned int Cursor_VBO, Cursor_VAO, Cursor_EBO;
    glGenVertexArrays(1, &Cursor_VAO);
    glGenBuffers(1, &Cursor_VBO);
    glGenBuffers(1, &Cursor_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(Cursor_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Cursor_VBO);
    glBufferData(GL_ARRAY_BUFFER, Cursor_vertices.size() * sizeof(float), &Cursor_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Cursor_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Cursor_indices.size() * sizeof(int), &Cursor_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        //game logic
        // -----

        //bullet logic
        BulletdeltaTime += deltaTime;
        if(!bullets.empty())
            for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++) {
                it->UpdatePos(deltaTime);
            }
        if (buttonPress) {
            if (BulletdeltaTime >= player1.GetFireDelay()) {
                bullets.push_back(Bullet(vec3 (0.0f, 5.0f, 0.0f), player1.GetCursor(), player1.GetShotSpeed()));
                BulletdeltaTime = 0.0f;
            }
        }

        //enemies logic
        EnemydeltaTime += deltaTime;
        if(!enemies.empty())
            for (list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
                it->UpdatePos(deltaTime);
                if (distance(it->GetPos(), vec3(0.0f, 0.0f, 0.0f)) <= 3.0f) {
                    player1.GetHit();
                    it = enemies.erase(it);
                }
                else if(!bullets.empty() && it != enemies.end())
                    for (list<Bullet>::iterator Bulletit = bullets.begin(); Bulletit != bullets.end(); Bulletit++) {
                        if (distance(it->GetPos(), Bulletit->GetPos()) <= 3.0f) {
                            it->getHit();
                            if (it->getLife() == 0)
                                it == enemies.erase(it);
                            Bulletit = bullets.erase(Bulletit);
                            if (Bulletit == bullets.end())
                                break;
                        }
                    }
                if (it == enemies.end())
                    break;
            }
        if (EnemydeltaTime >= enemyRate) {
            float xPos = (-400) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (400 - (-400)));
            float yPos = (-300) + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (300 - (-300)));
            int ID = rand() % 2;
            enemies.push_back(Enemy(vec3(0.0f, 0.0f, 50.0f), vec3(0.0f, 0.0f, 0.0f), ID));
            EnemydeltaTime = 0.0f;
        }
        

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //cursor
        cursorshader.use();
        //creat view
        cursorshader.setMat4("view", camera.GetViewMatrix());
        //creat proj
        mat4 Cursor_projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cursorshader.setMat4("proj", Cursor_projection);

        // create transformations
        mat4 Cursor_transform = mat4(1.0f); // make sure to initialize matrix to identity matrix first
        Cursor_transform = translate(Cursor_transform, player1.GetCursor());

        // update transform
        cursorshader.setMat4("transform", Cursor_transform);

        glBindVertexArray(Cursor_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, Cursor_numVertices);
        glDrawElements(GL_TRIANGLES, Cursor_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind our VA no need to unbind it every time 


        //bullet
        if(!bullets.empty())
            for (list<Bullet>::iterator it = bullets.begin(); it != bullets.end(); it++) {
                //items
                shader.use();
                // set objColor
                shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
                // set LightColor
                shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                // set LightPos
                shader.setVec3("lightPos", 100.0f, 100.0f, 100.0f);
                // creat view
                shader.setMat4("view", camera.GetViewMatrix());
                //set viewPos
                shader.setVec3("viewPos", camera.GetViewPos());
                //creat proj
                mat4 Bullet_projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                shader.setMat4("proj", Bullet_projection);

                // create transformations
                mat4 Bullet_transform = mat4(1.0f); // make sure to initialize matrix to identity matrix first
                Bullet_transform = translate(Bullet_transform, it->GetPos());

                // update transform
                shader.setMat4("transform", Bullet_transform);

                glBindVertexArray(Bullet_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
                glDrawArrays(GL_TRIANGLES, 0, Bullet_numVertices);
                glDrawElements(GL_TRIANGLES, Bullet_indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0); // unbind our VA no need to unbind it every time 
            }

        
        //enemies
        if(!enemies.empty())
            for (list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
                if (it -> getID() == 0 ) {
                    //items
                    shader.use();
                    // set objColor
                    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
                    // set LightColor
                    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                    // set LightPos
                    shader.setVec3("lightPos", 100.0f, 100.0f, 100.0f);
                    // creat view
                    shader.setMat4("view", camera.GetViewMatrix());
                    //set viewPos
                    shader.setVec3("viewPos", camera.GetViewPos());
                    //creat proj
                    mat4 Enemy_projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    shader.setMat4("proj", Enemy_projection);

                    // create transformations
                    mat4 Enemy_transform = mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    Enemy_transform = translate(Enemy_transform, it ->GetPos());

                    // update transform
                    shader.setMat4("transform", Enemy_transform);

                    glBindVertexArray(Met_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
                    glDrawArrays(GL_TRIANGLES, 0, Met_numVertices);
                    glDrawElements(GL_TRIANGLES, Met_indices.size(), GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0); // unbind our VA no need to unbind it every time 
                }
                if (it->getID() == 1) {
                    //items
                    shader.use();
                    // set objColor
                    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
                    // set LightColor
                    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                    // set LightPos
                    shader.setVec3("lightPos", 100.0f, 100.0f, 100.0f);
                    // creat view
                    shader.setMat4("view", camera.GetViewMatrix());
                    //set viewPos
                    shader.setVec3("viewPos", camera.GetViewPos());
                    //creat proj
                    mat4 Enemy_projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    shader.setMat4("proj", Enemy_projection);

                    // create transformations
                    mat4 Enemy_transform = mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    Enemy_transform = translate(Enemy_transform, it->GetPos());

                    // update transform
                    shader.setMat4("transform", Enemy_transform);

                    glBindVertexArray(Plane_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
                    glDrawArrays(GL_TRIANGLES, 0, Plane_numVertices);
                    glDrawElements(GL_TRIANGLES, Plane_indices.size(), GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0); // unbind our VA no need to unbind it every time 
                }
                
            }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &Cursor_VAO);
    glDeleteBuffers(1, &Cursor_VBO);
    glDeleteBuffers(1, &Cursor_EBO);
    glDeleteVertexArrays(1, &Bullet_VAO);
    glDeleteBuffers(1, &Bullet_VBO);
    glDeleteBuffers(1, &Bullet_EBO);
    glDeleteVertexArrays(1, &Met_VAO);
    glDeleteBuffers(1, &Met_VBO);
    glDeleteBuffers(1, &Met_EBO);
    glDeleteVertexArrays(1, &Plane_VAO);
    glDeleteBuffers(1, &Plane_VBO);
    glDeleteBuffers(1, &Plane_EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

//set up Vertex data


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        buttonPress = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        buttonPress = false;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (buttonPress) {
        float sensitivity = 0.005f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        HoriRotateNum += xoffset;
        VertRotateNum += yoffset;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float sensitivity = 0.03f;
    yoffset *= sensitivity;
    scaleNum *= 1 + yoffset;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


