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

	const std::string& toString() const { return m_debugName; }

protected:
	std::string m_debugName;

private:
};