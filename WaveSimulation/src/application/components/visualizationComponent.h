#pragma once

#include "application/components/component.h"

class VisualizationComponent : public Component
{
public:
	VisualizationComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onUpdate() override;
	void onRender() override;
	void onRenderGui() override;
};