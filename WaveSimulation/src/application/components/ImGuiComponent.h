/**
 * @file ImGuiComponent.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "application/components/component.h"

class ImGuiComponent : public Component
{
public:

	/*
	* 
	* Component that facilitates creating and rendering of ImGui frames
	*/
	ImGuiComponent(const std::string& debugName);

	void init() override;
	void destroy() override;

	void onRender() override;
	
	void startFrame();
	void endFrame();
};