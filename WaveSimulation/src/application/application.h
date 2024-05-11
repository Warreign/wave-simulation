#pragma once

#include <string>
#include <memory>

#include "application/window.h"

class Application
{
public:
	Application();
	Application(const std::string& title);
	
	void run();

private:
	void setup();
	void setupSettingsBar();

	void onUpdate();
	void renderGui();
	void renderScene();

	std::unique_ptr<Window> m_window;
};