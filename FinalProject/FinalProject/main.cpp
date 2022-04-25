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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
#include "shaderReader.h"
#include "modelReader.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PlayerState.h"
#include "Model.h"
#include "Mesh.h"

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

    Model cursorModel("data/cursor.obj");
    Model metModel("data/asteroids.obj");
    Model planeModel("data/spaceship.obj");
    Model bulletModel("data/laser.obj");

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

        cursorModel.Draw(cursorshader);

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

                bulletModel.Draw(shader);
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

                    metModel.Draw(shader);
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

                    planeModel.Draw(shader);
                }
                
            }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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


