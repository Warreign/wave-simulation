#include "component.h"

Component::Component(const std::string& debugName)
	: m_debugName(debugName)
{
}

void Component::init()
{
}

void Component::destroy()
{
}

void Component::onUpdate(float dt)
{
}

void Component::onRender()
{
}

void Component::onRenderGui()
{
}
