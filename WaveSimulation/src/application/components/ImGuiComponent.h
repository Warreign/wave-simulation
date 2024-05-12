#pragma once

#include "application/components/component.h"

class ImGuiComponent : public Component
{
public:
	void init() override;
	void destroy() override;

	void onRender() override;
	
	void startFrame();
	void endFrame();
};