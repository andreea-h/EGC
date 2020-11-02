#include "World.h"

#include <Core/Engine.h>
#include <Component/CameraInput.h>
#include <Component/Transform/Transform.h>

World::World()
{
	previousTime = 0;
	elapsedTime = 0;
	deltaTime = 0;
	paused = false;
	shouldClose = false;

	window = Engine::GetWindow();
}

void World::Run()
{
	if (!window)
		return;

	while (!window->ShouldClose())
	{
		LoopUpdate();
	}
}

void World::Pause()
{
	paused = !paused;
}

void World::Exit()
{
	shouldClose = true;
	window->Close();
}

double World::GetLastFrameTime()
{
	return deltaTime;
}

void World::ComputeFrameDeltaTime()
{
	elapsedTime = Engine::GetElapsedTime();
	deltaTime = elapsedTime - previousTime;
	previousTime = elapsedTime;
}

void World::LoopUpdate()
{
	// Polls and buffers the events
	window->PollEvents(); //se interogeaza evenimentele
	//cum ar fi: apasarea pe o tasta, modificarea dim. ferestrei
	//acestea sunt salvate pt a fi procesate ulterior

	// Computes frame deltaTime in seconds
	ComputeFrameDeltaTime(); //se estimeaza frame-rate-ul aplicatiei

	// Calls the methods of the instance of InputController in the following order
	// OnWindowResize, OnMouseMove, OnMouseBtnPress, OnMouseBtnRelease, OnMouseScroll, OnKeyPress, OnMouseScroll, OnInputUpdate
	// OnInputUpdate will be called each frame, the other functions are called only if an event is registered
	window->UpdateObservers();

	// Frame processing
	FrameStart(); //se modifica anumiti parametri folositi la desenare
	Update(static_cast<float>(deltaTime)); //functia principala in care se face desenarea
	FrameEnd();

	// Swap front and back buffers - image will be displayed to the screen
	window->SwapBuffers(); //in program se "tin" 2 buffere, la final se face swap la buffere si se afiseaza bufferul actualizat
}