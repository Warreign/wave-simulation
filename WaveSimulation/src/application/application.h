#pragma once

#include "application/window.h"
#include "application/components/ImGuiComponent.h"
#include "visualization/shader.h"

#include <string>
#include <memory>
#include <vector>

class Application
{
public:
	Application(const std::string& title);

	~Application();

	static Application& getInstance() { return *s_instance; }
	
	Window& getWindow() { return *m_window; }

	void run();
	void stop();

private:

	void addComponent(Component* comp);
	void removeComponent(Component* comp);	
	void onUpdate();
	void renderGui();
	void renderScene();

	static Application* s_instance;

	bool m_running;
	
	std::vector<Component*> comps;

	ImGuiComponent* m_guiComponent;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<Shader> m_waterShader;
	std::unique_ptr<Shader> m_skyboxShader;

	std::unique_ptr<Window> m_window;
};