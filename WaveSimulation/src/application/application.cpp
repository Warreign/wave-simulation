#include "application.h"

#include <imgui.h>

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

	SimulationComponent* simComp = new SimulationComponent("SimulationComponent");
	m_simComponent = simComp;
	addComponent(simComp);

	VisualizationComponent* visualComp = new VisualizationComponent("VisualizationComponent", simComp->getGrid());
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

		m_frameStart = glfwGetTime();

		/* Update each component */
		onUpdate();
		for (auto c : comps)
		{
			c->onUpdate(m_dt);
		}

		/* Add ImGui data from each component */
		m_guiComponent->startFrame();
		onRenderGui();
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

		m_frameEnd = glfwGetTime();
		m_dt = m_frameEnd - m_frameStart;
		m_deltaTime += m_dt;
		m_frameCount++;
		if (m_deltaTime >= 1.0)
		{
			m_frameRate = (double)m_frameCount * 0.5 + m_frameRate * 0.5;
			m_frameCount = 0;
			m_deltaTime = 0;
			m_averageFrameTime = 1.0 / (m_frameRate == 0 ? 0.001 : m_frameRate);
		}

		m_window->onUpdate();
	}

	for (auto c : comps)
	{
		std::cout << "INFO: Destroying " << c->toString() << std::endl;
		c->destroy();
		delete c;
	}
}

void Application::stop()
{
	std::cout << "INFO: Closing application" << std::endl;

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

void Application::onRenderGui()
{
	ImGui::Begin("Statistics");
	ImGui::InputFloat("FPS", &m_frameRate, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat("Average Frame Time", &m_averageFrameTime, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat("Frame dt", &m_dt, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::End();
}
