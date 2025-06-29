/**
 * @file window.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window
{
public:
	/*
	* Window initialization function
	*/
	Window(uint32_t width = 1280, uint32_t height = 960, const std::string& title = "Window");
	~Window();

private:
	GLFWwindow* m_window;

	uint32_t m_width;
	uint32_t m_height;
	std::string m_title;
	bool m_isVsync = true;


private:

	// Initialize opengl context and values associated with it
	void initContext();

public:

	// swap buffers and poll events
	void onUpdate();

	bool isVsync() const { return m_isVsync; }
	void setVsync(bool value);

	// Resize viewport
	void resize(int width, int height);

	GLFWwindow* getHandle() const { return m_window; }
	uint32_t getWidth() const { return m_width; }
	uint32_t getHeight() const { return m_height; }
	const std::string& getTitle() const { return m_title; }
};