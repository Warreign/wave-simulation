#include "application.h"

#include <IL/il.h>
#include <algorithm>

Application* Application::s_instance = nullptr;

Application::Application(const std::string& title)
{
	std::cout << "INFO: Initializing application" << std::endl;
	s_instance = this;

	m_window = std::make_unique<Window>(1280, 960, title);
	m_window->setVsync(true);

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	m_running = true;

	ImGuiComponent* guiComp = new ImGuiComponent("ImGuiComponent");
	m_guiComponent = guiComp;
	addComponent(guiComp);
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

		/* Update each component */
		for (auto c : comps)
		{
			c->onUpdate();
		}

		/* Add ImGui data from each component */
		m_guiComponent->startFrame();
		for (auto c : comps)
		{
			c->onRenderGui();
		}
		m_guiComponent->endFrame();

		/* Render each component */
		for (auto c : comps)
		{
			c->onRender();
		}

		m_window->onUpdate();
	}
}

void Application::stop()
{
	m_running = false;
}

void Application::addComponent(Component* comp)
{
	comp->init();
	comps.emplace_back(comp);
}

void Application::removeComponent(Component* comp)
{
	comp->destroy();
	std::remove(comps.begin(), comps.end(), comp);
}
