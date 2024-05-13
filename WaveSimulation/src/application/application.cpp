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

	VisualizationComponent* visualComp = new VisualizationComponent("VisualizationComponent");
	m_visualComponent = visualComp;
	addComponent(visualComp);

	ImGuiComponent* guiComp = new ImGuiComponent("ImGuiComponent");
	m_guiComponent = guiComp;
	addComponent(guiComp);
}

Application::~Application()
{
	
}

void Application::run()
{
	std::cout << "INFO: Running applicaiton" << std::endl;

	while (m_running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		/* Update each component */
		onUpdate();
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

	for (auto c : comps)
	{
		c->destroy();
		delete c;
	}
}

void Application::stop()
{
	std::cout << "INFO: Closing appliction" << std::endl;

	m_running = false;
}

void Application::addComponent(Component* comp)
{
	std::cout << "INFO: Adding " << comp->toString() << " to component stack" << std::endl;

	comp->init();
	comps.emplace_back(comp);
}

void Application::removeComponent(Component* comp)
{
	std::cout << "INFO: Removing " << comp->toString() << " from component stack" << std::endl;

	comp->destroy();
	std::remove(comps.begin(), comps.end(), comp);
	delete comp;
}

void Application::onUpdate()
{
}
