#pragma once

#include <string>

class Component
{
public:
	Component(const std::string& debugName);
	~Component() = default;

	virtual void init();
	virtual void destroy();

	virtual void onUpdate();
	virtual void onRender();
	virtual void onRenderGui();

protected:
	std::string m_debugName;

private:
};