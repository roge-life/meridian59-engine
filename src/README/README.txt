
1) BOOST

1.1) APPLY FIX FOR C++ CLI STATIC LINK ISSUE FROM boost-cli-fix.html
1.2) COMPILED BOOST WITH (change debug/release):

	b2 --with-atomic --with-chrono --with-date_time --with-system --with-thread 
           variant=debug threading=multi link=static runtime-link=shared 
	   --cxxflags="/clr /O2 /Ob2 /Oi /Ot /Oy /MP /arch:SSE2 /D \"_USING_V110_SDK71_\"" 
           --linkflags="/LARGEADDRESSAWARE /MACHINE:X86 /SUBSYSTEM:WINDOWS\",5.01\""

	64BIT:	address-model=64

2) CEGUI
2.1) See static compile fixes files
2.2) eraseSelectedText() in Editbox and MultiLineEditbox was made public
     and method was fixed in Editbox
2.3) Added "eraseText" method to Window.h / Window.cpp
2.4) Prevent EditBox from raising onKeyDown twice due to parent Window class
2.5) Add include to <algorithm> to Base.h due to min/max in VS2013

3) OGRE
3.1) Changed ColorKey in D3D9 backend to M59 cyan
3.2) Enable HW mipmaps for non-power-of-two textures (see ogre-enable-hwmipmaps...patch)

4) Caelum
4.1) Patched for Ogre 1.9.0 according to
     https://bitbucket.org/jacmoe/ogitor/pull-request/46/update-for-ogre-190-sharedptr-see-ghadamon
4.2) Don't set _MCW_PC for x64 in Astronomy.cpp (see caelum-fix-64bit-debug.patch)
 
5) ParticleUniverse
5.1) Disabled exception for unknown EventHandler type in "ParticleUniverseSystemManager.cpp" because of own subclass

6) OpenEXR
Added include to <algorithm> due to min/max in VS2013