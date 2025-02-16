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
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"

// application headers
#include "World.hh"
#include "Concept/DemoExpression.hh"
#include "Utilities/Viewport.hh"
#include "Concept/ImGui/ImGuiBuilder.hh"

// macro for application process closure
static int constexpr Error = -1;
static int constexpr Success = 0;

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
	if (!SDL_Init(InitFlags))
	{
		SDL_Log("Init failed: %s", SDL_GetError());
		return Error;
	}

	//	*******
	//	window Creation
	//	*******

	// declare var running application render target
	int constexpr WindowX = 800;
	int constexpr WindowY = 600;
	SDL_WindowFlags WindowFlags = SDL_WINDOW_OPENGL;

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
	//	OpenGl Creation
	//	*******

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext GlContext = SDL_GL_CreateContext(Window);

	if (!IsValid(GlContext))
	{
		SDL_Log("OpenGl context creation failed: %s", SDL_GetError());
		return Error;
	}

	// load function ptr specific to the os sdl is compiling for
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_Log("Gl loader failed: %s", SDL_GetError());
		return Error;
	}

	//	*******
	//	imgui Creation
	//	*******

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& Io = ImGui::GetIO();
	Io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(Window, &GlContext);
	ImGui_ImplOpenGL3_Init();

	// world creation
	auto EditorWorld = FWorld::Factory(UDemoExpression());

	//	*******
	//	poll events
	//	*******

	// sdl events
	auto const PollPlatformEvents = [&](bool& bOutRequestExit)
		{
			// fetch platform events
			SDL_Event Event;
			while (SDL_PollEvent(&Event))
			{
				ImGui_ImplSDL3_ProcessEvent(&Event);
				// check for quit event
				if (Event.type == SDL_EventType::SDL_EVENT_QUIT)
				{
					bOutRequestExit = true;
				}
			}
		};

	//	*******
	//	ticking
	//	*******

	// world tick
	auto const ApplicationTick = [](FWorld& World)
		{
			World.Tick();
		};

	//	*******
	//	rendering
	//	*******

	// imgui new frame (does way more under the hood when looking at imgui_impl but will keep it simple here!)
	// handle backend new frame creation, opengl shader&program creation/link
	// sdl controller updates (mouse/gamepad) + delta time management for running app at 60 fps
	auto const ImGuiClear = [&]()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
		};

	// imgui content drawing
	auto const ImGuiDraw = [&](FWorld& World, FImGuiBuilder& Builder)
		{
			World.DrawImGui();
			ImGui::Render();
		};

	auto const ViewportClear = [&](ImGuiIO const& Data)
		{
			auto static constexpr ClearColor = ImVec4(0.f, 0.f, 0.f, 1.f);
			glViewport(0, 0, (int)Data.DisplaySize.x, (int)Data.DisplaySize.y);
			glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
			glClear(GL_COLOR_BUFFER_BIT);
		};

	// opengl back buffer handling
	auto const ViewportDraw = [&](SDL_Window* Target)
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(Target);
		};

	// world render
	auto const ApplicationDraw = [](FWorld& World)
		{
			World.Draw();
		};

	//	*******
	//	application loop
	//	*******

	bool bRequestExit = false;
	while (!bRequestExit)
	{
		// platform events
		PollPlatformEvents(bRequestExit);

		// application tick
		ApplicationTick(EditorWorld);

		// imgui clear - doesnt affect rendering backend
		ImGuiClear();

		// imgui draw - doesnt affect rendering backend
		ImGuiDraw(EditorWorld, FImGuiBuilder::Builder);

		// viewport clear
		ViewportClear(Io);

		// application draw
		ApplicationDraw(EditorWorld);

		// opengl viewport rendering
		ViewportDraw(Window);
	}

	//	*******
	//	lib clean up
	//	*******

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(GlContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return Success;
}