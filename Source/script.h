#pragma once

struct FeatureStruct
{
	const char *label;
	bool *pState;
	bool pLoop;
	bool *pUpdated ;
	std::function<void()> function; //ontick
	std::function<void()> functionUpdated; //once
};


void ScriptMain();

