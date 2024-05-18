#include "window.h"

#include "application/application.h"
#include "utils/GlDebug.h"

#include <stdexcept>
#include <glad/glad.h>


static void glfw_error_callback(int error, const char* description)
{
	throw std::runtime_error(description);
}

Window::Window(uint32_t width, uint32_t height, const std::string& title)
	: m_width(width), m_height(height), m_title(title)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW!");

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window!");
	}

	initContext();

	if (m_isVsync)
	{
		glfwSwapInterval(1);
	}

	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* w)
		{
			Application& app = Application::getInstance();
			app.stop();
		});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height)
		{
			Application& app = Application::getInstance();
			Window& window = app.getWindow();
			window.resize(width, height);
		});
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::initContext()
{
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD!");
	};

	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << std::endl;

	GLint v;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &v);
	std::cout << "Compute WG count X: " << v << std::endl;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &v);
	std::cout << "Compute WG count Y: " << v << std::endl;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &v);
	std::cout << "Compute WG count Z: " << v << std::endl;
	std::cout << std::endl;

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &v);
	std::cout << "Compute WG size X: " << v << std::endl;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &v);
	std::cout << "Compute WG size Y: " << v << std::endl;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &v);
	std::cout << "Compute WG size Z: " << v << std::endl;
	std::cout << std::endl;

	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &v);
	std::cout << "Compute WG invocations: " << v << std::endl;
	std::cout << std::endl;

	glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &v);
	std::cout << "Compute shader max image units: " << v << std::endl;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &v);
	std::cout << "Max image units: " << v << std::endl;
	std::cout << std::endl;

#ifdef _DEBUG
#endif
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)GL_ERROR_CALLBACK, 0);
}

void Window::setVsync(bool value)
{
	m_isVsync = value;
	if (m_isVsync)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
}

void Window::resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glViewport(0, 0, m_width, m_height);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}