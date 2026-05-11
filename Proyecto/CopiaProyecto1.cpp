//320255409 | 422130448
//13/05/2026
//Proyecto Final
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

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

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
unsigned int loadCubemap(std::vector<std::string> faces);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera (Y = 1.7 para altura de ojos)
Camera  camera(glm::vec3(0.0f, 1.7f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

// LAMPARAS DEL TECHO
glm::vec3 lampPositions[] = {
	glm::vec3(17.849f, 18.173f,   4.872f),
	glm::vec3(17.488f, 18.173f,  15.971f),
	glm::vec3(17.686f, 18.173f,  -7.700f),
	glm::vec3(18.457f, 18.219f, -17.999f),
	glm::vec3(4.4002f, 18.205f, -18.156f),
	glm::vec3(4.5392f, 18.198f,  -7.913f),
	glm::vec3(4.6731f, 18.213f,   4.883f),
	glm::vec3(4.7584f, 18.188f,  15.869f)
};
const int NUM_LAMPS = sizeof(lampPositions) / sizeof(lampPositions[0]);

bool lampsOn = false;


// Constantes del walking simulator
const float CAMERA_HEIGHT = 6.7f;
const float BOB_SPEED = 9.0f;
const float BOB_AMOUNT = 0.08f;
const float WALK_SPEED_MULT = 0.5f;
const float PITCH_LIMIT = 75.0f;

// Variables del parpadeo
float blinkTimer = 0.0f;
float nextBlinkTime = 3.0f;
bool isBlinking = false;
float blinkProgress = 0.0f;
const float BLINK_DURATION = 0.20f;


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


// Vertices del cubo del Skybox
float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
};


// Vertices del quad para el parpadeo
float quadVertices[] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	 1.0f,  1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
	-1.0f, -1.0f
};


glm::vec3 Light1 = glm::vec3(0);
float rotBall = 0.0f;
bool AnimBall = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	srand((unsigned int)time(NULL));

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final | 320255409 | 422130448", nullptr, nullptr);

	if (nullptr == window)
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


	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag");
	Shader blinkShader("Shader/blink.vs", "Shader/blink.frag");

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

	// VAO/VBO de los cubitos
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

	// VAO/VBO del Skybox
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// VAO/VBO del quad de parpadeo
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);


	// Cargar skybox
	std::vector<std::string> facesDia = {
		"Models/Skybox/dia/right.jpg",
		"Models/Skybox/dia/left.jpg",
		"Models/Skybox/dia/top.jpg",
		"Models/Skybox/dia/bottom.jpg",
		"Models/Skybox/dia/back.jpg",
		"Models/Skybox/dia/front.jpg"
	};
	unsigned int cubemapDia = loadCubemap(facesDia);

	skyboxShader.Use();
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);


	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();


		// Logica del parpadeo
		blinkTimer += deltaTime;
		float blinkAlpha = 0.0f;

		if (isBlinking)
		{
			blinkProgress += deltaTime / BLINK_DURATION;
			if (blinkProgress >= 1.0f)
			{
				isBlinking = false;
				blinkProgress = 0.0f;
				blinkTimer = 0.0f;
				nextBlinkTime = 3.0f + (rand() % 30) / 10.0f;
			}
			else
			{
				blinkAlpha = sin(blinkProgress * 3.14159f);
			}
		}
		else if (blinkTimer >= nextBlinkTime)
		{
			isBlinking = true;
			blinkProgress = 0.0f;
		}


		glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f);

		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		// Point light 1 (SPACE)
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// Lamparas rojas
		glm::vec3 lampColor = lampsOn ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f);
		for (int i = 0; i < NUM_LAMPS; i++)
		{
			std::string idx = std::to_string(i + 1);
			std::string base = "pointLights[" + idx + "].";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "position").c_str()),
				lampPositions[i].x, lampPositions[i].y, lampPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "ambient").c_str()),
				lampColor.x * 0.1f, lampColor.y * 0.1f, lampColor.z * 0.1f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "diffuse").c_str()),
				lampColor.x, lampColor.y, lampColor.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "specular").c_str()),
				lampColor.x, lampColor.y, lampColor.z);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "linear").c_str()), 0.022f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "quadratic").c_str()), 0.0019f);
		}

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Facultad.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(18.1f, 0.0f, -20.0f));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.8f, 4.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Stand4.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(3.0f, 4.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Stand1.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 3.0f));
		model = glm::scale(model, glm::vec3(5.0f, 4.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Stand3.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, 0.0f, -0.231f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Humanoide.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.231f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dog.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ingeniera.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(8.5f, 0.0f, 4.0f));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pareja.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(9.5f, 0.0f, -6.3f));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mujer.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, -16.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Hombre.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(17.0f, 5.3f, 16.5f));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Multitud.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDisable(GL_BLEND);
		glBindVertexArray(0);


		// Lamp shader
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		if (lampsOn)
		{
			glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), 1.0f, 0.0f, 0.0f);
			glBindVertexArray(VAO);
			for (int i = 0; i < NUM_LAMPS; i++)
			{
				model = glm::mat4(1);
				model = glm::translate(model, lampPositions[i]);
				model = glm::scale(model, glm::vec3(0.6f, 0.2f, 1.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(0);
		}


		// Skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapDia);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		// Quad de parpadeo encima de todo
		if (blinkAlpha > 0.01f)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			blinkShader.Use();
			glUniform1f(glGetUniformLocation(blinkShader.Program, "alpha"), blinkAlpha);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}


		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}


// Funcion para cargar un cubemap
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Fallo al cargar textura del cubemap: " << faces[i] << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void DoMovement()
{
	static float bobTime = 0.0f;

	bool isWalking = keys[GLFW_KEY_W] || keys[GLFW_KEY_S] ||
		keys[GLFW_KEY_A] || keys[GLFW_KEY_D] ||
		keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] ||
		keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT];

	// Velocidad realista
	float walkDelta = deltaTime * WALK_SPEED_MULT;

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, walkDelta);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, walkDelta);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, walkDelta);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, walkDelta);

	if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
	if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
	if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
	if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
	if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;

	// Cabeceo al caminar
	if (isWalking)
	{
		bobTime += deltaTime * BOB_SPEED;
		float bob = sin(bobTime) * BOB_AMOUNT;
		camera.SetPositionY(CAMERA_HEIGHT + bob);
	}
	else
	{
		bobTime = 0.0f;
		camera.SetPositionY(CAMERA_HEIGHT);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		else Light1 = glm::vec3(0);
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		lampsOn = !lampsOn;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);

	// Limitar mirada vertical
	if (camera.GetPitch() > PITCH_LIMIT)
	{
		camera.SetPitch(PITCH_LIMIT);
	}
	if (camera.GetPitch() < -PITCH_LIMIT)
	{
		camera.SetPitch(-PITCH_LIMIT);
	}
}