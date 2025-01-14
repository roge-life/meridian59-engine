/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#include "win32/Win32Mouse.h"
#include "win32/Win32InputManager.h"
#include "OISException.h"
#include "OISEvents.h"

using namespace OIS;

//--------------------------------------------------------------------------------------------------//
Win32Mouse::Win32Mouse( InputManager* creator, IDirectInput8* pDI, bool buffered, DWORD coopSettings )
	: Mouse(creator->inputSystemName(), buffered, 0, creator)
{
	mMouse = 0;
	mDirectInput = pDI;
	coopSetting = coopSettings;
	mHwnd = 0;

	static_cast<Win32InputManager*>(mCreator)->_setMouseUsed(true);
}

//--------------------------------------------------------------------------------------------------//
void Win32Mouse::_initialize()
{
	DIPROPDWORD dipdw;

	//Clear old state
	mState.clear();

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MOUSE_DX_BUFFERSIZE;
	
	if( FAILED(mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to create device" );

	if( FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set format" );
	
	mHwnd = ((Win32InputManager*)mCreator)->getWindowHandle();

	if( FAILED(mMouse->SetCooperativeLevel(mHwnd, coopSetting)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set coop level" );
	
	if( FAILED(mMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph )) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set property" );

	HRESULT hr = mMouse->Acquire();
	if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to aquire mouse!" );

	mMouse->SetDataFormat(&c_dfDIMouse2);
}

//--------------------------------------------------------------------------------------------------//
Win32Mouse::~Win32Mouse()
{
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}

	static_cast<Win32InputManager*>(mCreator)->_setMouseUsed(false);
}

//--------------------------------------------------------------------------------------------------//
void Win32Mouse::capture()
{
	if (!mMouse)
		return;
	
	// clear old relative values
	mState.X.rel = mState.Y.rel = mState.Z.rel = 0;

	/**** TRY GET NEW INPUT ****/

	DWORD entries = MOUSE_DX_BUFFERSIZE;
	DIMOUSESTATE2 mouseState;

	// try read input
	HRESULT hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	
	// issue: try re-aquire
	if (FAILED(hr))
	{
		// these errors indicate that the device must be re-aquired
		if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST)
			hr = mMouse->Acquire();

		// give up this time
		if (FAILED(hr))
			return;
	
		// try get input again
		hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
		
		// give up this time
		if (FAILED(hr))
			return;		
	}

	/**** GOT VALID NEW INPUT ****/

	// update relative move
	mState.X.rel = mouseState.lX;
	mState.Y.rel = mouseState.lY;
	mState.Z.rel = mouseState.lZ;

	bool axesMoved = mouseState.lX != 0 || mouseState.lY != 0 || mouseState.lZ != 0;

	// handle mouse-move
	if (axesMoved)
	{
		if (coopSetting & DISCL_NONEXCLUSIVE)
		{
			// DirectInput provides us with meaningless values, so correct that
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(mHwnd, &point);
			mState.X.abs = point.x;
			mState.Y.abs = point.y;
		}
		else
		{
			mState.X.abs +=  mState.X.rel;
			mState.Y.abs +=  mState.Y.rel;
		}

		mState.Z.abs +=  mState.Z.rel;

		// bound x
		if (mState.X.abs < 0)
			mState.X.abs = 0;

		else if (mState.X.abs > mState.width)
			mState.X.abs = mState.width;

		// bound y
		if (mState.Y.abs < 0)
			mState.Y.abs = 0;

		else if (mState.Y.abs > mState.height)
			mState.Y.abs = mState.height;

		// do the move
		if (mListener && mBuffered)
			mListener->mouseMoved(MouseEvent(this, mState));
	}

	// handle mouse-click
	for (unsigned int i=0;i<8;i++)
		if(!_doMouseClick(i, mouseState.rgbButtons[i])) 
			return;
}

//--------------------------------------------------------------------------------------------------//
bool Win32Mouse::_doMouseClick(int mouseButton, unsigned char di)
{
	// pressed
	if (di & 0x80 && !mState.buttonDown((MouseButtonID)mouseButton))
	{
		mState.buttons |= 1 << mouseButton; //turn the bit flag on
		if (mListener && mBuffered)
			return mListener->mousePressed(MouseEvent(this, mState), (MouseButtonID)mouseButton);
	}

	// released
	else if (!(di & 0x80) && mState.buttonDown((MouseButtonID)mouseButton))
	{
		mState.buttons &= ~(1 << mouseButton); //turn the bit flag off
		if (mListener && mBuffered)
			return mListener->mouseReleased(MouseEvent(this, mState), (MouseButtonID)mouseButton);
	}

	return true;
}

//--------------------------------------------------------------------------------------------------//
void Win32Mouse::setBuffered(bool buffered)
{
	mBuffered = buffered;
}
