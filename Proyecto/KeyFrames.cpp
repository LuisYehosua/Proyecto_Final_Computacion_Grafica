// 320255409 | 422130448
// 13/05/2026
// Proyecto Final 

#include <iostream>
#include <cmath>
#include <fstream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Robot keyframe functions
void saveFrameRobot();
void resetElementsRobot();
void interpolationRobot();
void AnimationRobot();
void saveToFileRobot(const char* filename);
void loadFromFileRobot(const char* filename);

// Perro keyframe functions 
void saveFramePerro();
void resetElementsPerro();
void interpolationPerro();
void AnimationPerro();
void saveToFilePerro(const char* filename);
void loadFromFilePerro(const char* filename);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool    keys[1024];
bool    firstMouse = true;

// Light

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = false;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f)
};

float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,

    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,

     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,

    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f
};

glm::vec3 Light1 = glm::vec3(0);
float rotBall = 0.0;

// Variables de postura del robot (relativas al cuerpo base)
float rBrazoD = 0.0f;
float rBrazoI = 0.0f;
float rPiernaD = 0.0f;
float rPiernaI = 0.0f;
float robotPosX = 5.0f;
float robotPosY = 0.0f;
float robotPosZ = 0.0f;
float robotRotY = 0.0f;

#define MAX_FRAMES_ROBOT 250
int iMaxStepsRobot = 190;
int iCurrStepsRobot = 0;

typedef struct {
    // Valores del keyframe
    float brazoDeg;
    float brazoIeg;
    float piernaDeg;
    float piernaIeg;
    // Incrementos calculados por interpolación
    float brazoDInc;
    float brazoIInc;
    float piernaDInc;
    float piernaIInc;
} FRAME_ROBOT;

FRAME_ROBOT KeyFrameRobot[MAX_FRAMES_ROBOT];
int  frameIndexRobot = 0;
bool playRobot = false;
int  playIndexRobot = 0;

void saveFrameRobot()
{
    if (frameIndexRobot >= MAX_FRAMES_ROBOT) { printf("Máximo de keyframes alcanzado\n"); return; }
    printf("Robot - guardando keyframe %d\n", frameIndexRobot);
    KeyFrameRobot[frameIndexRobot].brazoDeg = rBrazoD;
    KeyFrameRobot[frameIndexRobot].brazoIeg = rBrazoI;
    KeyFrameRobot[frameIndexRobot].piernaDeg = rPiernaD;
    KeyFrameRobot[frameIndexRobot].piernaIeg = rPiernaI;
    frameIndexRobot++;
}

void resetElementsRobot()
{
    rBrazoD = KeyFrameRobot[0].brazoDeg;
    rBrazoI = KeyFrameRobot[0].brazoIeg;
    rPiernaD = KeyFrameRobot[0].piernaDeg;
    rPiernaI = KeyFrameRobot[0].piernaIeg;
}

void interpolationRobot()
{
    int i = playIndexRobot;
    KeyFrameRobot[i].brazoDInc = (KeyFrameRobot[i + 1].brazoDeg - KeyFrameRobot[i].brazoDeg) / iMaxStepsRobot;
    KeyFrameRobot[i].brazoIInc = (KeyFrameRobot[i + 1].brazoIeg - KeyFrameRobot[i].brazoIeg) / iMaxStepsRobot;
    KeyFrameRobot[i].piernaDInc = (KeyFrameRobot[i + 1].piernaDeg - KeyFrameRobot[i].piernaDeg) / iMaxStepsRobot;
    KeyFrameRobot[i].piernaIInc = (KeyFrameRobot[i + 1].piernaIeg - KeyFrameRobot[i].piernaIeg) / iMaxStepsRobot;
}

void AnimationRobot()
{
    if (!playRobot) return;

    if (iCurrStepsRobot >= iMaxStepsRobot)
    {
        playIndexRobot++;
        if (playIndexRobot > frameIndexRobot - 2)
        {
            printf("Robot - animación terminada\n");
            playIndexRobot = 0;
            playRobot = false;
        }
        else
        {
            iCurrStepsRobot = 0;
            interpolationRobot();
        }
    }
    else
    {
        rBrazoD += KeyFrameRobot[playIndexRobot].brazoDInc;
        rBrazoI += KeyFrameRobot[playIndexRobot].brazoIInc;
        rPiernaD += KeyFrameRobot[playIndexRobot].piernaDInc;
        rPiernaI += KeyFrameRobot[playIndexRobot].piernaIInc;
        iCurrStepsRobot++;
    }
}

void saveToFileRobot(const char* filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) { printf("No se pudo crear el archivo\n"); return; }
    file << frameIndexRobot << "\n";
    for (int i = 0; i < frameIndexRobot; i++)
    {
        file << KeyFrameRobot[i].brazoDeg << " "
            << KeyFrameRobot[i].brazoIeg << " "
            << KeyFrameRobot[i].piernaDeg << " "
            << KeyFrameRobot[i].piernaIeg << "\n";
    }
    file.close();
    printf("Animación robot guardada en %s\n", filename);
}

void loadFromFileRobot(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) { printf("No se pudo abrir %s\n", filename); return; }
    file >> frameIndexRobot;
    for (int i = 0; i < frameIndexRobot; i++)
    {
        file >> KeyFrameRobot[i].brazoDeg
            >> KeyFrameRobot[i].brazoIeg
            >> KeyFrameRobot[i].piernaDeg
            >> KeyFrameRobot[i].piernaIeg;
    }
    file.close();
    printf("Animación robot cargada desde %s (%d keyframes)\n", filename, frameIndexRobot);
}

//PERRO KEYFRAME SYSTEM 

float pHead = 0.0f;
float pTail = 0.0f;
float pFLeftLeg = 0.0f;
float pFRightLeg = 0.0f;
float pBLeftLeg = 0.0f;
float pBRightLeg = 0.0f;
float pBodyRotZ = 0.0f;
float pRotDog = 0.0f;
float pPosX = 0.0f;
float pPosY = 0.0f;
float pPosZ = 0.0f;

#define MAX_FRAMES_PERRO 250
int iMaxStepsPerro = 190;
int iCurrStepsPerro = 0;

typedef struct {
    float posX, posY, posZ;
    float rotDog, bodyRotZ;
    float head, tail;
    float FLeftLeg, FRightLeg, BLeftLeg, BRightLeg;
    // Incrementos
    float incX, incY, incZ;
    float rotDogInc, bodyRotZInc;
    float headInc, tailInc;
    float FLeftLegInc, FRightLegInc, BLeftLegInc, BRightLegInc;
} FRAME_PERRO;

FRAME_PERRO KeyFramePerro[MAX_FRAMES_PERRO];
int  frameIndexPerro = 0;
bool playPerro = false;
int  playIndexPerro = 0;

void saveFramePerro()
{
    if (frameIndexPerro >= MAX_FRAMES_PERRO) { printf("Máximo de keyframes perro alcanzado\n"); return; }
    printf("Perro - guardando keyframe %d\n", frameIndexPerro);
    KeyFramePerro[frameIndexPerro].posX = pPosX;
    KeyFramePerro[frameIndexPerro].posY = pPosY;
    KeyFramePerro[frameIndexPerro].posZ = pPosZ;
    KeyFramePerro[frameIndexPerro].rotDog = pRotDog;
    KeyFramePerro[frameIndexPerro].bodyRotZ = pBodyRotZ;
    KeyFramePerro[frameIndexPerro].head = pHead;
    KeyFramePerro[frameIndexPerro].tail = pTail;
    KeyFramePerro[frameIndexPerro].FLeftLeg = pFLeftLeg;
    KeyFramePerro[frameIndexPerro].FRightLeg = pFRightLeg;
    KeyFramePerro[frameIndexPerro].BLeftLeg = pBLeftLeg;
    KeyFramePerro[frameIndexPerro].BRightLeg = pBRightLeg;
    frameIndexPerro++;
}

void resetElementsPerro()
{
    pPosX = KeyFramePerro[0].posX;
    pPosY = KeyFramePerro[0].posY;
    pPosZ = KeyFramePerro[0].posZ;
    pRotDog = KeyFramePerro[0].rotDog;
    pBodyRotZ = KeyFramePerro[0].bodyRotZ;
    pHead = KeyFramePerro[0].head;
    pTail = KeyFramePerro[0].tail;
    pFLeftLeg = KeyFramePerro[0].FLeftLeg;
    pFRightLeg = KeyFramePerro[0].FRightLeg;
    pBLeftLeg = KeyFramePerro[0].BLeftLeg;
    pBRightLeg = KeyFramePerro[0].BRightLeg;
}

void interpolationPerro()
{
    int i = playIndexPerro;
    KeyFramePerro[i].incX = (KeyFramePerro[i + 1].posX - KeyFramePerro[i].posX) / iMaxStepsPerro;
    KeyFramePerro[i].incY = (KeyFramePerro[i + 1].posY - KeyFramePerro[i].posY) / iMaxStepsPerro;
    KeyFramePerro[i].incZ = (KeyFramePerro[i + 1].posZ - KeyFramePerro[i].posZ) / iMaxStepsPerro;
    KeyFramePerro[i].rotDogInc = (KeyFramePerro[i + 1].rotDog - KeyFramePerro[i].rotDog) / iMaxStepsPerro;
    KeyFramePerro[i].bodyRotZInc = (KeyFramePerro[i + 1].bodyRotZ - KeyFramePerro[i].bodyRotZ) / iMaxStepsPerro;
    KeyFramePerro[i].headInc = (KeyFramePerro[i + 1].head - KeyFramePerro[i].head) / iMaxStepsPerro;
    KeyFramePerro[i].tailInc = (KeyFramePerro[i + 1].tail - KeyFramePerro[i].tail) / iMaxStepsPerro;
    KeyFramePerro[i].FLeftLegInc = (KeyFramePerro[i + 1].FLeftLeg - KeyFramePerro[i].FLeftLeg) / iMaxStepsPerro;
    KeyFramePerro[i].FRightLegInc = (KeyFramePerro[i + 1].FRightLeg - KeyFramePerro[i].FRightLeg) / iMaxStepsPerro;
    KeyFramePerro[i].BLeftLegInc = (KeyFramePerro[i + 1].BLeftLeg - KeyFramePerro[i].BLeftLeg) / iMaxStepsPerro;
    KeyFramePerro[i].BRightLegInc = (KeyFramePerro[i + 1].BRightLeg - KeyFramePerro[i].BRightLeg) / iMaxStepsPerro;
}

void AnimationPerro()
{
    if (!playPerro) return;

    if (iCurrStepsPerro >= iMaxStepsPerro)
    {
        playIndexPerro++;
        if (playIndexPerro > frameIndexPerro - 2)
        {
            printf("Perro - animación terminada\n");
            playIndexPerro = 0;
            playPerro = false;
        }
        else
        {
            iCurrStepsPerro = 0;
            interpolationPerro();
        }
    }
    else
    {
        pPosX += KeyFramePerro[playIndexPerro].incX;
        pPosY += KeyFramePerro[playIndexPerro].incY;
        pPosZ += KeyFramePerro[playIndexPerro].incZ;
        pRotDog += KeyFramePerro[playIndexPerro].rotDogInc;
        pBodyRotZ += KeyFramePerro[playIndexPerro].bodyRotZInc;
        pHead += KeyFramePerro[playIndexPerro].headInc;
        pTail += KeyFramePerro[playIndexPerro].tailInc;
        pFLeftLeg += KeyFramePerro[playIndexPerro].FLeftLegInc;
        pFRightLeg += KeyFramePerro[playIndexPerro].FRightLegInc;
        pBLeftLeg += KeyFramePerro[playIndexPerro].BLeftLegInc;
        pBRightLeg += KeyFramePerro[playIndexPerro].BRightLegInc;
        iCurrStepsPerro++;
    }
}

void saveToFilePerro(const char* filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) { printf("No se pudo crear el archivo\n"); return; }
    file << frameIndexPerro << "\n";
    for (int i = 0; i < frameIndexPerro; i++)
    {
        file << KeyFramePerro[i].posX << " "
            << KeyFramePerro[i].posY << " "
            << KeyFramePerro[i].posZ << " "
            << KeyFramePerro[i].rotDog << " "
            << KeyFramePerro[i].bodyRotZ << " "
            << KeyFramePerro[i].head << " "
            << KeyFramePerro[i].tail << " "
            << KeyFramePerro[i].FLeftLeg << " "
            << KeyFramePerro[i].FRightLeg << " "
            << KeyFramePerro[i].BLeftLeg << " "
            << KeyFramePerro[i].BRightLeg << "\n";
    }
    file.close();
    printf("Animación perro guardada en %s\n", filename);
}

void loadFromFilePerro(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) { printf("No se pudo abrir %s\n", filename); return; }
    file >> frameIndexPerro;
    for (int i = 0; i < frameIndexPerro; i++)
    {
        file >> KeyFramePerro[i].posX
            >> KeyFramePerro[i].posY
            >> KeyFramePerro[i].posZ
            >> KeyFramePerro[i].rotDog
            >> KeyFramePerro[i].bodyRotZ
            >> KeyFramePerro[i].head
            >> KeyFramePerro[i].tail
            >> KeyFramePerro[i].FLeftLeg
            >> KeyFramePerro[i].FRightLeg
            >> KeyFramePerro[i].BLeftLeg
            >> KeyFramePerro[i].BRightLeg;
    }
    file.close();
    printf("Animación perro cargada desde %s (%d keyframes)\n", filename, frameIndexPerro);
}

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main()
{
    // --- Init GLFW ---
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Proyecto Final | 320255409 | 422130448", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // --- Shaders ---
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // --- Models ---
    Model Humanoide((char*)"Models/Humanoid/HumanoRobot.obj");
    Model Multitud((char*)"Models/Multitud/MultitudPersonas1.obj");
    Model Facultad((char*)"Models/FacultadFINAL.obj");
    Model Ingeniera((char*)"Models/stands/Ingeniera.obj");
    Model Pareja((char*)"Models/stands/Stand2obj.obj");
    Model Mujer((char*)"Models/stands/Stand3.obj");
    Model Hombre((char*)"Models/stands/Stand4.obj");
    Model Dog((char*)"Models/Cuadrupedo/PerroRobot.obj");
    Model Stand1((char*)"Models/LugarStand1FINAL.obj");
    Model Stand3((char*)"Models/LugarStand3FINAL.obj");
    Model Stand4((char*)"Models/LugarStand4FINAL.obj");
    // Humanoid
    Model Cuerpo((char*)"Models/Humanoid/Cuerpo.obj");
    Model BrazoD((char*)"Models/Humanoid/BrazoD.obj");
    Model BrazoI((char*)"Models/Humanoid/BrazoI.obj");
    Model PiernaD((char*)"Models/Humanoid/PiernaD.obj");
    Model PiernaI((char*)"Models/Humanoid/PiernaI.obj");

    // --- Inicializar arreglos de keyframes ---
    for (int i = 0; i < MAX_FRAMES_ROBOT; i++) {
        KeyFrameRobot[i] = { 0,0,0,0, 0,0,0,0 };
    }
    for (int i = 0; i < MAX_FRAMES_PERRO; i++) {
        KeyFramePerro[i] = { 0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0 };
    }

    // --- VAO / VBO ---
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        AnimationRobot();
        AnimationPerro();

        glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelTemp = glm::mat4(1.0f);

        // --- Lighting shader uniforms ---
        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc,
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
            -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
            0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
            0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program,
            "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);


        // Facultad
        model = modelTemp;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Facultad.Draw(lightingShader);

        // Stand 4
        model = modelTemp;
        model = glm::translate(model, glm::vec3(18.1f, 0.0f, -20.0f));
        model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.8f, 4.0f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Stand4.Draw(lightingShader);

        // Stand 1
        model = modelTemp;
        model = glm::translate(model, glm::vec3(8.0f, 0.0f, -10.0f));
        model = glm::scale(model, glm::vec3(3.0f, 4.0f, 3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Stand1.Draw(lightingShader);

        // Stand 3
        model = modelTemp;
        model = glm::translate(model, glm::vec3(8.0f, 0.0f, 3.0f));
        model = glm::scale(model, glm::vec3(5.0f, 4.0f, 3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Stand3.Draw(lightingShader);

        //Ingeniera
        model = modelTemp;
        model = glm::translate(model, glm::vec3(22.0f, 0.0f, -1.0f));
        model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ingeniera.Draw(lightingShader);

        //Pareja
        model = modelTemp;
        model = glm::translate(model, glm::vec3(8.5f, 0.0f, 4.0f));
        model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Pareja.Draw(lightingShader);

        //Mujer
        model = modelTemp;
        model = glm::translate(model, glm::vec3(9.5f, 0.0f, -6.3f));
        model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Mujer.Draw(lightingShader);

        //Hombre
        model = modelTemp;
        model = glm::translate(model, glm::vec3(15.0f, 0.0f, -16.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Hombre.Draw(lightingShader);

        // Multitud
        model = modelTemp;
        model = glm::translate(model, glm::vec3(17.0f, 5.3f, 16.5f));
        model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.2f, 1.7f, 1.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Multitud.Draw(lightingShader);

        // Humanoide 
        glm::mat4 robotBase = glm::mat4(1.0f);

        robotBase = glm::translate(robotBase,
            glm::vec3(robotPosX, robotPosY, robotPosZ));

        robotBase = glm::rotate(robotBase,
            glm::radians(robotRotY),
            glm::vec3(0.0f, 1.0f, 0.0f));

        // Cuerpo 
        model = robotBase;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Cuerpo.Draw(lightingShader);

        // BrazoD
        model = robotBase;
        model = glm::translate(model, glm::vec3(-0.17553f, 4.45f, 0.00018f));
        model = glm::rotate(model, glm::radians(rBrazoD), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.17553f, -4.4f, -0.00018f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoD.Draw(lightingShader);

        // BrazoI
        model = robotBase;
        model = glm::translate(model, glm::vec3(0.12605f, 4.45f, -0.00295f));
        model = glm::rotate(model, glm::radians(rBrazoI), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.12605f, -4.4f, 0.00295f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoI.Draw(lightingShader);

        // PiernaD
        model = robotBase;
        model = glm::translate(model, glm::vec3(-0.10145f, 3.0f, 0.01237f));
        model = glm::rotate(model, glm::radians(rPiernaD), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.10145f, -3.0f, -0.01237f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PiernaD.Draw(lightingShader);

        // PiernaI
        model = robotBase;
        model = glm::translate(model, glm::vec3(0.02592f, 3.0f, 0.00513f));
        model = glm::rotate(model, glm::radians(rPiernaI), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.02592f, -3.0f, -0.00513f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        PiernaI.Draw(lightingShader);

        // Perro robot 
        model = modelTemp;
        model = glm::translate(model, glm::vec3(2.0f + pPosX, 1.0f + pPosY, 0.231f + pPosZ));
        model = glm::rotate(model, glm::radians(pRotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(pBodyRotZ), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dog.Draw(lightingShader);

        // --- Lamp shader ---
        lampShader.Use();
        GLint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint lampViewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint lampProjLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(lampProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    // BrazoD
    if (keys[GLFW_KEY_1]) rBrazoD += 1.0f;
    if (keys[GLFW_KEY_2]) rBrazoD -= 1.0f;
    // BrazoI
    if (keys[GLFW_KEY_3]) rBrazoI += 1.0f;
    if (keys[GLFW_KEY_4]) rBrazoI -= 1.0f;
    // PiernaD
    if (keys[GLFW_KEY_5]) rPiernaD += 1.0f;
    if (keys[GLFW_KEY_6]) rPiernaD -= 1.0f;
    // PiernaI
    if (keys[GLFW_KEY_7]) rPiernaI += 1.0f;
    if (keys[GLFW_KEY_8]) rPiernaI -= 1.0f;
    //Adelante / atrás
    if (keys[GLFW_KEY_Z]) robotPosZ -= 0.05f;
    if (keys[GLFW_KEY_X]) robotPosZ += 0.05f;
    // Izquierda / derecha
    if (keys[GLFW_KEY_C]) robotPosX -= 0.05f;
    if (keys[GLFW_KEY_V]) robotPosX += 0.05f;
    // Rotación sobre su eje
    if (keys[GLFW_KEY_B]) robotRotY += 1.0f;
    if (keys[GLFW_KEY_N]) robotRotY -= 1.0f;

    // --- Cámara ---
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])   camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])   camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])  camera.ProcessKeyboard(RIGHT, deltaTime);

    // --- Point light ---
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // ---- Robot keyframe controls ----

    // K  -> guardar keyframe del robot
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (frameIndexRobot < MAX_FRAMES_ROBOT)
            saveFrameRobot();
    }

    // L  -> play / stop animación robot
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!playRobot && frameIndexRobot > 1)
        {
            resetElementsRobot();
            interpolationRobot();
            playRobot = true;
            playIndexRobot = 0;
            iCurrStepsRobot = 0;
            printf("Robot - reproduciendo animación\n");
        }
        else
        {
            playRobot = false;
            printf("Robot - animación detenida\n");
        }
    }

    // F  -> guardar animación robot a archivo
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        saveToFileRobot("animacion_robot.txt");

    // R  -> cargar animación robot desde archivo
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        loadFromFileRobot("animacion_robot.txt");

    // -- Perro keyframe controls --

    // I  -> guardar keyframe del perro
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        if (frameIndexPerro < MAX_FRAMES_PERRO)
            saveFramePerro();
    }

    // O  -> play / stop animación perro
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        if (!playPerro && frameIndexPerro > 1)
        {
            resetElementsPerro();
            interpolationPerro();
            playPerro = true;
            playIndexPerro = 0;
            iCurrStepsPerro = 0;
            printf("Perro - reproduciendo animacion\n");
        }
        else
        {
            playPerro = false;
            printf("Perro - animacion detenida\n");
        }
    }

    // P  -> guardar animación perro a archivo
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        saveToFilePerro("animacion_perro.txt");

    // M  -> cargar animación perro desde archivo
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        loadFromFilePerro("animacion_perro.txt");

    // SPACE -> toggle luz de colores
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }
    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;
    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}
