#include "application.h"

#include <IL/il.h>

Application* Application::s_instance = nullptr;

Application::Application()
{
}

Application::Application(const std::string& title)
{
	std::cout << "INFO: Initializing application" << std::endl;
	s_instance = this;

	m_window = std::make_unique<Window>(1280, 960, title);

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	m_window->setVsync(true);

	m_running = true;
}

Application::~Application()
{
	
}

void Application::run()
{
	while (m_running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_window->onUpdate();
	}
}

void Application::stop()
{
	m_running = false;
}
