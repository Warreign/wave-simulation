#pragma once

#include <string>
#include <memory>

#include "application/window.h"
#include "visualization/shader.h"

class Application
{
public:
	Application();
	Application(const std::string& title);

	~Application();

	static Application& getInstance() { return *s_instance; }
	
	Window* getWindow() { return m_window.get(); }

private:

	void run();
	void stop();
	void onUpdate();
	void renderGui();
	void renderScene();

	static Application* s_instance;

	bool m_running;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<Shader> m_waterShader;
	std::unique_ptr<Shader> m_skyboxShader;

	std::unique_ptr<Window> m_window;
};