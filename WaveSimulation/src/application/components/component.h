#pragma once

#include <string>

class Component
{
public:
	Component(const std::string& debugName);

	virtual ~Component();

	virtual void onUpdate();
	virtual void onRenderGui();

protected:
	std::string m_debugName;

private:
};