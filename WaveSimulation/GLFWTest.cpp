#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main()
{
	if (!glfwInit())
	{
		throw new std::runtime_error("glfw init error");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Test window", NULL, NULL);

	if (!window)
	{
		throw new std::runtime_error("glfw window init failed");
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	while (!glfwWindowShouldClose(window))
	{

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
