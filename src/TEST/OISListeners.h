#pragma once

#pragma managed(push, off)
#include "OISKeyboard.h"
#include "OISMouse.h"
#include "CEGUI\CEGUI.h"
#include "CEGUI\MouseCursor.h"
#include <windows.h>
#include <WinUser.h>
#pragma managed(pop)

using namespace ::OIS;

public class OISKeyListener : public OIS::KeyListener
{
public:
	OISKeyListener(void) { };
		
	virtual bool keyPressed(const OIS::KeyEvent &arg) override
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)arg.key);
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)arg.text);

		return true;
	};

	virtual bool keyReleased(const OIS::KeyEvent &arg) override
	{
		return 
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	};
};

public class OISMouseListener : public OIS::MouseListener
{
protected:
	bool mouseInWindow;

public:
	OISMouseListener(void) { };
		
	virtual bool mouseMoved(const OIS::MouseEvent &arg) override
	{
		if (mouseInWindow)
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);
			
			if (arg.state.X.abs == 0 || arg.state.Y.abs == 0 ||
				arg.state.X.abs == arg.state.width || arg.state.Y.abs == arg.state.height)
			{
				CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
				ShowCursor(true); // windows only
				mouseInWindow = false;
			}

			if (arg.state.Z.rel)
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
		}
		else if (arg.state.X.abs > 0 && arg.state.Y.abs > 0 &&
					arg.state.X.abs < arg.state.width && arg.state.Y.abs < arg.state.height)
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
			ShowCursor(false); // windows only
			mouseInWindow = true;
		}

		return true;
	};

	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override
	{
		return 
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	};

	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override
	{
		return CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	};

	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
	{
		switch (buttonID)
		{
		case OIS::MB_Left:
			return CEGUI::LeftButton;
 
		case OIS::MB_Right:
			return CEGUI::RightButton;
 
		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
 
		case OIS::MB_Button3:
			return CEGUI::X1Button;

		case OIS::MB_Button4:
			return CEGUI::X2Button;

		default:
			return CEGUI::LeftButton;
		}
	};
};
