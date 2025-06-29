/**
 * @file component.h
 *
 * @brief Generic component of the application
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include <string>

class Component
{
public:

	/**
	* A generic component that has to be inherited from
	*
	*/
	Component(const std::string& debugName);
	~Component() = default;

	virtual void init();
	virtual void destroy();

	/*
	* update function will be called each frame
	* 
	* @param dt time step
	*
	*/
	virtual void onUpdate(float dt);

	/*
	* step when the component has to render its contents
	*/
	virtual void onRender();

	/*
	* function to submit gui data to ImGui frame
	*/
	virtual void onRenderGui();

	const std::string& toString() const { return m_debugName; }

protected:
	std::string m_debugName;

private:
};