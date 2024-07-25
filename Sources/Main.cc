//Copyright(c) 2024 gdemers
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

// system headers
#include <cstdio>

// vendor headers
#include "SDL3/SDL.h"

// macro for application process closure
static constexpr int Error = -1;
static constexpr int Success = 0;

// extern check
template<typename T>
bool IsValid(T* Ptr)
{
	return Ptr != nullptr;
}

// application entry point
int main(int argc /*arg count*/, char* argv[] /*arg values*/)
{
	//	*******
	//	lib Init
	//	*******

	SDL_InitFlags InitFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	if (SDL_Init(InitFlags) < 0)
	{
		SDL_Log("Init failed: %s", SDL_GetError());
		return Error;
	}

	//	*******
	//	window Creation
	//	*******

	// declare var running application render target
	const int WindowX = 800;
	const int WindowY = 600;
	SDL_WindowFlags WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOWPOS_CENTERED | SDL_WINDOW_ALWAYS_ON_TOP;

	SDL_Window* Window = SDL_CreateWindow("Title",
		WindowX,
		WindowY,
		WindowFlags);

	if (!IsValid(Window))
	{
		SDL_Log("Window creation failed: %s", SDL_GetError());
		return Error;
	}

	//	*******
	//	poll events
	//	*******

	const auto& PollPlatformEvents = [](bool& bOutRequestExit)
		{
			// fetch platform events
			SDL_Event Event;
			if (!SDL_PollEvent(&Event))
			{
				return;
			}

			// check for quit event
			if (Event.quit.type == SDL_EventType::SDL_EVENT_QUIT)
			{
				bOutRequestExit = true;
			}
		};

	//	*******
	//	application loop
	//	*******

	bool bRequestExit = false;
	while (!bRequestExit)
	{
		PollPlatformEvents(bRequestExit);
	}

	//	*******
	//	lib clean up
	//	*******

	SDL_DestroyWindow(Window);
	SDL_Quit();

	return Success;
}