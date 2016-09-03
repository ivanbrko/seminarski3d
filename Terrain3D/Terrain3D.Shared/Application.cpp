#include "pch.h"
#include "Game.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;




ref class App sealed : public IFrameworkView
{
private:
	bool windowClosed;
	CGame Game;
public:
	virtual void Initialize(CoreApplicationView^ AppView)
	{
		AppView->Activated += ref new TypedEventHandler <CoreApplicationView^, IActivatedEventArgs^ >(this, &App::onActivated);
		CoreApplication::Suspending += ref new EventHandler <SuspendingEventArgs^>(this, &App::Suspending);
		CoreApplication::Resuming += ref new EventHandler <Object^>(this, &App::Resuming);
		windowClosed = false;
	}
	virtual void SetWindow(CoreWindow^ Window) 
	{
		Window->PointerPressed += ref new TypedEventHandler <CoreWindow^, PointerEventArgs^>(this, &App::PointerPressed);
		Window->PointerMoved += ref new TypedEventHandler <CoreWindow^, PointerEventArgs^>(this, &App::PointerMoved);
		Window->Closed += ref new TypedEventHandler <CoreWindow^, CoreWindowEventArgs^>(this, &App::Closed);
		Window->KeyDown += ref new TypedEventHandler <CoreWindow^, KeyEventArgs^>(this, &App::KeyPressed);
		Window->KeyUp += ref new TypedEventHandler <CoreWindow^, KeyEventArgs^>(this, &App::KeyUp);
	}

	virtual void Load(String^ EntryPoint) {}

	virtual void Run() 
	{
		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		Game.Initialize();

		while (!windowClosed)
		{
			window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			Game.Update();
			Game.Render();
		}
	}

	virtual void Uninitialize() {}

	void onActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		//Get a pointer to the window and activate it

		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		window->Activate();
	}

	void PointerPressed(CoreWindow^ window, PointerEventArgs^ args)
	{
		String^ s = " ";
		MessageDialog Dialog(Game.dbgInt, "Test");
		Dialog.ShowAsync();
		Game.goDown = false;
		Game.goUp = false;
		Game.goFwd = false;
		Game.goBack = false;
	}

	void PointerMoved(CoreWindow^ window, PointerEventArgs^ args)
	{
		Game.horizontal = (args->CurrentPoint->Position.X / window->Bounds.Width - 0.5);
		Game.vertical = (args->CurrentPoint->Position.Y / window->Bounds.Height- 0.5);
	}

	void KeyPressed(CoreWindow^ Window, KeyEventArgs^ args)
	{
		if (args->VirtualKey == VirtualKey::F1)
			Game.wireframe = true;
		else if (args->VirtualKey == VirtualKey::F2)
			Game.wireframe = false;
		else if (args->VirtualKey == VirtualKey::F3)
			Game.walkMode = true;
		else if (args->VirtualKey == VirtualKey::F4)
			Game.walkMode = false;
		else if (args->VirtualKey == VirtualKey::F5)
			if (!Game.waves)
				Game.waves = true;
			else
				Game.waves = false;
		else if (args->VirtualKey == VirtualKey::Left || args->VirtualKey == VirtualKey::A)
			Game.goDown = true;
		else if (args->VirtualKey == VirtualKey::Right || args->VirtualKey == VirtualKey::D)
			Game.goUp = true;
		else if (args->VirtualKey == VirtualKey::Up || args->VirtualKey == VirtualKey::W)
			Game.goFwd = true;
		else if (args->VirtualKey == VirtualKey::Down || args->VirtualKey == VirtualKey::S)
			Game.goBack = true;
		else if (args->VirtualKey == VirtualKey::F5)
			if (!Game.waves)
				Game.waves = true;
			else
				Game.waves = false;

	}

	void KeyUp(CoreWindow^ Window, KeyEventArgs^ args)
	{
		if (args->VirtualKey == VirtualKey::Left || args->VirtualKey == VirtualKey::A)
			Game.goDown = false;
		else if (args->VirtualKey == VirtualKey::Right || args->VirtualKey == VirtualKey::D)
			Game.goUp = false;
		else if (args->VirtualKey == VirtualKey::Up || args->VirtualKey == VirtualKey::W)
			Game.goFwd = false;
		else if (args->VirtualKey == VirtualKey::Down || args->VirtualKey == VirtualKey::S)
			Game.goBack = false;

	}

	void Suspending(Object^ Sender, SuspendingEventArgs^ args)
	{

	}

	void Resuming(Object^ Sender, Object^ args)
	{

	}

	void Closed(CoreWindow^ Sender, CoreWindowEventArgs^ args)
	{
		windowClosed = true;
	}
};




ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();
	}
};




[MTAThread]

int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}

