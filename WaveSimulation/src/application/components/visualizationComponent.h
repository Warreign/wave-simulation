#pragma once

#include "application/components/component.h"

class VisualizationComponent : public Component
{
public:
	void init() override;
	void destroy() override;

	void onUpdate() override;
	void onRender() override;
	void onRenderGui() override;
};