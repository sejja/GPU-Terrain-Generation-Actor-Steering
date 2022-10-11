#include <Shared.h>
void TimeManager::StartFrame()
{
	mStartFrame = 0;
	mFrames += 0.016;
}

void TimeManager::EndFrame()
{
	// @TODO: FixedDeltaTime
	deltaTime = fixedDeltaTime = 0.016f;
}