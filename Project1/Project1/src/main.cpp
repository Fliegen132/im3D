#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include "graphics/shader.h"
#include "inputs/keyboard.h"

#include "inputs/mouse.h"
#include "inputs/camera.h"
#include "inputs/screen.h"
#include "graphics/texture.h"
#include "graphics/Models/cube.hpp"
#include "graphics/Models/lamp.hpp"

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

Screen screen;

Camera cameras[2]
{
	Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	Camera(glm::vec3(0.0f, 0.0f, 5.0f)),
};
int activeCam = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-0.4f, 0, -0.5), glm::vec3(0.25f));

class WindowCreator
{
public:
	WindowCreator()
	{
		if (!glfwInit())
		{
			std::cout << "glfwInit failed" << std::endl;
			return;
		}

		if (!screen.init())
		{
			glfwTerminate();
			return;
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Glad failed" << std::endl;
			glfwTerminate();
		}

		screen.setParameters();
		
		Shader shader("assets/object.vs", "assets/object.fs");
		Shader lampShader("assets/object.vs", "assets/lamp.fs");

		Cube cube (glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.75f), Material::bronze);
		cube.init();

		lamp.init();
		while (!screen.shouldClose())
		{
			float currentTime = glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;

			processInput(deltaTime);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			screen.update();

			shader.activate();
			shader.set3Float("light.position", lamp.pos);
			shader.set3Float("viewPos", cameras[activeCam].cameraPos);
			shader.set3Float("light.ambient", lamp.ambient);
			shader.set3Float("light.diffuse", lamp.diffuse);
			shader.set3Float("light.specular", lamp.specular);
			//create transform for screen

			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			view = cameras[activeCam].getViewMatrix();

			projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);

			cube.render(shader);

			lampShader.activate();
			lampShader.setMat4("view", view);
			lampShader.setMat4("projection", projection);
			lamp.render(lampShader);
			screen.newFrame();
		}

		cube.cleanup();
		lamp.cleanup();
		glfwTerminate();
	}
private:
	void processInput(double dt)
	{
		if (Keyboard::keyCodeDown(GLFW_KEY_ESCAPE))
		{
			screen.setShouldClose(true);
		}
		if (Keyboard::key(GLFW_KEY_W))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::FORWARD, dt);
		}
		if (Keyboard::key(GLFW_KEY_S))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::BACKWARD, dt);

		}
		if (Keyboard::key(GLFW_KEY_A))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::LEFT, dt);

		}
		if (Keyboard::key(GLFW_KEY_D))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::RIGHT, dt);
		}
		if (Keyboard::key(GLFW_KEY_SPACE))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::UP, dt);
		}
		if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
		{
			cameras[activeCam].updateCameraPosition(CameraDirection::DOWN, dt);
		}
		if (Keyboard::keyCodeDown(GLFW_KEY_E))
		{
			if (activeCam >= 1)
				return;
			activeCam++;
		}
		if (Keyboard::keyCodeDown(GLFW_KEY_Q))
		{
			if (activeCam <= 0)
				return;
			activeCam--;
		}

		double dx = Mouse::getDX() / 3, dy = Mouse::getDY() / 3;

		if (dx != 0 || dy != 0)
		{
			cameras[activeCam].updateCameraDirection(dx, dy);
		}

		double scrollDy = Mouse::getScrollDY();

		if (scrollDy != 0)
		{
			cameras[activeCam].updateCameraZoom(scrollDy);
		}
#pragma region lamp
		if (Keyboard::key(GLFW_KEY_LEFT))
		{
			lamp.pos.x -= 0.001f;
		}
		if (Keyboard::key(GLFW_KEY_RIGHT))
		{
			lamp.pos.x += 0.001f;
		}
		if (Keyboard::key(GLFW_KEY_UP))
		{
			lamp.pos.z -= 0.001f;
		}
		if (Keyboard::key(GLFW_KEY_DOWN))
		{
			lamp.pos.z += 0.001f;
		}
#pragma endregion
	}
};

int main()
{
	std::cout << "Start program" << std::endl;
	WindowCreator cr;
	return 0;
}
