﻿/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2019 Michael R. P. Ragazzon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Input.h>
#include <Shell.h>


// Performance TODO:
//  - Improve performance of transform parser (hashtable)
//  - Replace property name strings with handle IDs (ints). Tried this and reverted, see [0e390e9], too little gain for too much complexity.
//  - Memory pools for common elements. Also, a lot of temporary objects are created and destroyed.
//  - Try replacing ElementAttributes with vector.
//  - Can we optimize the layouting? E.g. why is ElementTextDefault::GenerateLine being called even when neither text nor size have seemingly been changed.
//  - [bug] Input.range appears only after one additional frame.

class DemoWindow
{
public:
	DemoWindow(const Rocket::Core::String &title, const Rocket::Core::Vector2f &position, Rocket::Core::Context *context)
	{
		using namespace Rocket::Core;
		document = context->LoadDocument("basic/benchmark/data/benchmark.rml");
		if (document != NULL)
		{
			{
				document->GetElementById("title")->SetInnerRML(title);
				document->SetProperty("left", Property(position.x, Property::PX));
				document->SetProperty("top", Property(position.y, Property::PX));
			}

			document->Show();
		}
	}

	void performance_test()
	{
		/*
		  FPS values
		  Original: 18.5  [957f723]
		  Without property counter: 22.0
		  With std::string: 23.0  [603fd40]
		  robin_hood unordered_flat_map: 24.0  [709852f]
		  Avoid dirtying em's: 27.5
		  Restructuring update loop: 34.5  [f9892a9]
		  Element constructor, remove geometry database, remove update() from Context::render: 38.0  [1aab59e]
		  Replace Dictionary with unordered_flat_map: 40.0  [b04b4e5]
		  Dirty flag for structure changes: 43.0  [fdf6f53]
		  Replacing containers: 46.0  [c307140]
		  Replace 'resize' event with virtual function call: 53.0  [7ad658f]
		  Use all_properties_dirty flag when constructing elements: 55.0 [fa6bd0a]
		  Don't double create input elements: 58.0  [e162637]
		  Memory pool for ElementMeta: 59.0  [ece191a]
		  Include chobo flat containers: 65.0  [1696aa5]
		
		*/

		if (!document)
			return;

		Rocket::Core::String rml;

		for (int i = 0; i < 50; i++)
		{
			int index = rand() % 1000;
			int route = rand() % 50;
			int max = (rand() % 40) + 10;
			int value = rand() % max;
			Rocket::Core::String rml_row = Rocket::Core::CreateString(1000, R"(
			<div class="row">
				<div class="col col1"><button class="expand" index="%d">+</button>&nbsp;<a>Route %d</a></div>
				<div class="col col23"><input type="range" class="assign_range" min="0" max="%d" value="%d"/></div>
				<div class="col col4">Assigned</div>
				<div class="inrow unmark_collapse">
					<div class="col col123 assign_text">Assign to route</div>
					<div class="col col4">
						<input type="submit" class="vehicle_depot_assign_confirm" quantity="0">Confirm</input>
					</div>
				</div>
			</div>)",
				index, 
				route,
				max,
				value
			);
			rml += rml_row;
		}

		if (auto el = document->GetElementById("performance"))
			el->SetInnerRML(rml);
	}

	~DemoWindow()
	{
		if (document)
		{
			document->RemoveReference();
			document->Close();
		}
	}

	Rocket::Core::ElementDocument * GetDocument() {
		return document;
	}

private:
	Rocket::Core::ElementDocument *document;
};


Rocket::Core::Context* context = NULL;
ShellRenderInterfaceExtensions *shell_renderer;
DemoWindow* window = NULL;

bool pause_loop = false;
bool single_loop = false;
bool run_update = true;

void GameLoop()
{
	if (!pause_loop || single_loop)
	{
		if (run_update)
		{
			window->performance_test();
		}

		context->Update();

		shell_renderer->PrepareRenderBuffer();
		context->Render();
		shell_renderer->PresentRenderBuffer();

		single_loop = false;
	}

	static double t_prev = 0.0f;
	double t = Shell::GetElapsedTime();
	float dt = float(t - t_prev);
	static int count_frames = 0;
	count_frames += 1;

	if (window && dt > 0.2f)
	{
		t_prev = t;
		auto el = window->GetDocument()->GetElementById("fps");
		float fps = float(count_frames) / dt;
		count_frames = 0;
		el->SetInnerRML(Rocket::Core::CreateString( 20, "FPS: %f", fps ));
	}
}



class Event : public Rocket::Core::EventListener
{
public:
	Event(const Rocket::Core::String& value) : value(value) {}

	void ProcessEvent(Rocket::Core::Event& event) override
	{
		using namespace Rocket::Core;

		if(value == "exit")
			Shell::RequestExit();

		if (event == "keydown")
		{
			auto key_identifier = (Rocket::Core::Input::KeyIdentifier)event.GetParameter< int >("key_identifier", 0);

			if (key_identifier == Rocket::Core::Input::KI_SPACE)
			{
				pause_loop = !pause_loop;
			}
			else if (key_identifier == Rocket::Core::Input::KI_OEM_PLUS)
			{
				pause_loop = true;
				single_loop = true;
			}
			else if (key_identifier == Rocket::Core::Input::KI_RETURN)
			{
				run_update = !run_update;
			}
			else if (key_identifier == Rocket::Core::Input::KI_ESCAPE)
			{
				Shell::RequestExit();
			}
			else if (key_identifier == Rocket::Core::Input::KI_F8)
			{
				Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
			}
		}
	}

	void OnDetach(Rocket::Core::Element* element) override { delete this; }

private:
	Rocket::Core::String value;
};


class EventInstancer : public Rocket::Core::EventListenerInstancer
{
public:

	/// Instances a new event handle for Invaders.
	Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value, Rocket::Core::Element* element) override
	{
		return new Event(value);
	}

	/// Destroys the instancer.
	void Release() override { delete this; }
};


#if defined ROCKET_PLATFORM_WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED_PARAMETER(instance_handle), HINSTANCE ROCKET_UNUSED_PARAMETER(previous_instance_handle), char* ROCKET_UNUSED_PARAMETER(command_line), int ROCKET_UNUSED_PARAMETER(command_show))
#else
int main(int ROCKET_UNUSED_PARAMETER(argc), char** ROCKET_UNUSED_PARAMETER(argv))
#endif
{
#ifdef ROCKET_PLATFORM_WIN32
	ROCKET_UNUSED(instance_handle);
	ROCKET_UNUSED(previous_instance_handle);
	ROCKET_UNUSED(command_line);
	ROCKET_UNUSED(command_show);
#else
	ROCKET_UNUSED(argc);
	ROCKET_UNUSED(argv);
#endif

	const int width = 1800;
	const int height = 1000;

	ShellRenderInterfaceOpenGL opengl_renderer;
	shell_renderer = &opengl_renderer;

	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise("../../Samples/") ||
		!Shell::OpenWindow("Benchmark Sample", shell_renderer, width, height, true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	Rocket::Core::SetRenderInterface(&opengl_renderer);
	opengl_renderer.SetViewport(width, height);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Controls::Initialise();
	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);
	shell_renderer->SetContext(context);

	EventInstancer* event_instancer = new EventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);
	event_instancer->RemoveReference();

	Shell::LoadFonts("assets/");

	window = new DemoWindow("Benchmark sample", Rocket::Core::Vector2f(81, 100), context);
	window->GetDocument()->AddEventListener("keydown", new Event("hello"));
	window->GetDocument()->AddEventListener("keyup", new Event("hello"));
	window->GetDocument()->AddEventListener("animationend", new Event("hello"));


	Shell::EventLoop(GameLoop);

	delete window;

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}