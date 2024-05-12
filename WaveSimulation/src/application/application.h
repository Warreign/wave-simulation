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
	
	void run();

private:

	void onUpdate();
	void renderGui();
	void renderScene();

	bool m_running;

	std::unique_ptr<Shader> m_commonShader;
	std::unique_ptr<Shader> m_waterShader;
	std::unique_ptr<Shader> m_skyboxShader;

	std::unique_ptr<Window> m_window;
};