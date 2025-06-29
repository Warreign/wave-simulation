/**
 * @file application.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

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
	/*
	* Main application class
	* 
	* @param title Title of the application window
	*/
	Application(const std::string& title);

	~Application();

	static Application& getInstance() { 
		if (!s_instance) exit(1);
		return *s_instance; 
	}
	
	Window& getWindow() { return *m_window; }


	/*
	* Run loop of the application
	*/
	void run();

	/*
	* exit the run loop
	*/
	void stop();

	SimulationComponent& getSimComp() { return *m_simComponent; }
	VisualizationComponent& getVisualComp() { return *m_visualComponent; }

private:

	/*
	* Add a component to the component stack and initialzie it
	*/
	void addComponent(Component* comp);

	/*
	* Remove component from the stack and destroy it
	*/
	void removeComponent(Component* comp);

	/*
	* update variables related to the applicaton
	*/
	void onUpdate();

	/*
	* render gui related to the application
	*/
	void onRenderGui();

	static Application* s_instance;

	bool m_running;

	// values used to calculate framerate and frame time (not used anymore)
	int m_targetFrameRate = 60.0f;
	float m_dt = 0;
	double m_deltaTime = 0;
	double m_frameEnd = 0;
	double m_frameStart = 0;
	float m_averageFrameTime = 0;
	float m_frameRate = 60;
	uint32_t m_frameCount = 0;
	

	// Component stack
	std::vector<Component*> comps;

	// Simulation component reference
	SimulationComponent* m_simComponent;
	// Visual component reference
	VisualizationComponent* m_visualComponent;
	// Gui component reference
	ImGuiComponent* m_guiComponent;

	std::unique_ptr<Window> m_window;
};