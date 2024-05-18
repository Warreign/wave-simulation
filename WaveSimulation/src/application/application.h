#pragma once

#include "application/window.h"
#include "application/components/ImGuiComponent.h"
#include "application/components/simulationComponent.h"
#include "application/components/visualizationComponent.h"

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

class Application
{
public:
	Application(const std::string& title);

	~Application();

	static Application& getInstance() { 
		if (!s_instance) exit(1);
		return *s_instance; 
	}
	
	Window& getWindow() { return *m_window; }

	void run();
	void stop();

	SimulationComponent& getSimComp() { return *m_simComponent; }
	VisualizationComponent& getVisualComp() { return *m_visualComponent; }

private:

	void addComponent(Component* comp);
	void removeComponent(Component* comp);	
	void onUpdate();
	void onRenderGui();

	static Application* s_instance;

	bool m_running;
	int m_targetFrameRate = 60.0f;
	float m_dt = 0;
	double m_deltaTime = 0;
	double m_frameEnd = 0;
	double m_frameStart = 0;
	float m_averageFrameTime = 0;
	float m_frameRate = 60;
	uint32_t m_frameCount = 0;
	
	std::vector<Component*> comps;

	SimulationComponent* m_simComponent;
	VisualizationComponent* m_visualComponent;
	ImGuiComponent* m_guiComponent;

	std::unique_ptr<Window> m_window;
};