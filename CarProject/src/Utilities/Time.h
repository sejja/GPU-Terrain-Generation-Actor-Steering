#pragma once
#include "Composition\Singleton.h"

class TimeManager
{
	Singleton(TimeManager);

public:
	void StartFrame();
	void EndFrame();

public:
	float deltaTime = 0.016667f;
	float fixedDeltaTime = 0.016667f;
	float mFrames = 0;

private:
	int mStartFrame;
};

#define mTime (&TimeManager::Instance())