#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

#include "events/Event.h"

namespace Boar
{
	class Window
	{
	public:
		Window(uint32_t width = 1280, uint32_t height = 960, const std::string& title = "Window");
		~Window();

	private:
		GLFWwindow* m_window;

		uint32_t m_width;
		uint32_t m_height;
		std::string m_title;
		bool m_isVsync = true;


	private:
		void initContext();

	public:
		EventCallback eventCallback;

		void onUpdate();

		bool isVsync() const { return m_isVsync; }
		void setVsync(bool value);

		void setEventCallback(EventCallback func) { eventCallback = func; }

		GLFWwindow* getHandle() const { return m_window; }
		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }
		const std::string& getTitle() const { return m_title; }
	};
}