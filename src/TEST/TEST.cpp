//#include "stdafx.h"

#pragma managed(push, off)
#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreViewport.h"
#include "OgreRenderSystem.h"
#include "OgreFrameListener.h"
#include "OIS.h"
#include "OISListeners.h"

#include "OgreD3D9RenderSystem.h"
#include "OgreParticleFXPlugin.h"
#include "OgreOctreePlugin.h"
#include "OgreCgPlugin.h"

#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"
#pragma managed(pop)

#define RENDERSYSTEM			"Direct3D9 Rendering Subsystem"
#define WINDOWNAME				"MyGUI Test"
#define VIDEOMODE				"800x600 @ 32-bit colour"
#define UI_RESOURCESUBFOLDER	"ui"
#define UI_FILE_LAYOUT			"empty.layout"
#define UI_FILE_SCHEME			"TaharezLook.scheme"
#define DEFAULTARROW			"TaharezLook/MouseArrow"
#define DEFAULTTOOLTIP			"TaharezLook/Tooltip"
#define RESGROUP_IMAGESETS		"ImageSets"
#define RESGROUP_FONTS			"Fonts"
#define RESGROUP_SCHEMES		"Schemes"
#define RESGROUP_LOOKNFEEL		"LookNFeel"
#define RESGROUP_LAYOUTS		"Layouts"

/* OGRE */
Ogre::Root* root;
Ogre::RenderSystem* renderSystem;
Ogre::RenderWindow* renderWindow;
Ogre::SceneManager* sceneManager;
Ogre::Camera* camera;
Ogre::Viewport* viewport;
Ogre::ResourceGroupManager* resMan;

/* OIS */
OIS::InputManager* inputManager;
OIS::Keyboard* oisKeyboard;
OIS::Mouse* oisMouse;

/* CEGUI */
CEGUI::OgreRenderer* renderer;
CEGUI::System* ceguiSystem;
CEGUI::GUIContext* guiContext;
CEGUI::Scheme* scheme;
CEGUI::Window* guiRoot;


/// <summary>
/// FrameListener class for Ogre calling injectTimePulse
/// </summary>  
public class MyFrameListener : public ::Ogre::FrameListener
{
public:
	MyFrameListener() { }

	virtual bool frameRenderingQueued (const Ogre::FrameEvent &evt) override
	{
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);		
		CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(evt.timeSinceLastFrame);

		return true;
	};
};

extern "C"{
CEGUI::FactoryModule& getWindowFactoryModule()
{
    static CEGUI::FactoryModule mod;
    return mod;
}
}

/// <summary>
/// APPLICATION MAIN METHOD
/// </summary>  
int main(array<::System::String ^> ^args)
{	
	/****************************************
	        1. INIT OGRE
	*****************************************/

	// init the ogre root object
	root = OGRE_NEW Ogre::Root();

	// init the rendersystem
	renderSystem = new Ogre::D3D9RenderSystem(0);
		
	// set values on renderSystem and attach it to root
    renderSystem->setConfigOption("Full Screen", "No");
	renderSystem->setConfigOption("Video Mode", VIDEOMODE);
    root->setRenderSystem(renderSystem);
    
	// load plugins
	root->installPlugin(new Ogre::ParticleFXPlugin());
	root->installPlugin(new Ogre::OctreePlugin());
	root->installPlugin(new Ogre::CgPlugin());
	
	// initialise the root
    root->initialise(false, WINDOWNAME);

	// some more settings for the window
    Ogre::NameValuePairList misc;
    misc["FSAA"] = "4";
    misc["vsync"] = "off";
    misc["border"] = "fixed";

	// create the renderwindow
    renderWindow = root->createRenderWindow(
        WINDOWNAME,
        800,
        600,
        false,
        &misc);

	// create SceneManager
    sceneManager = root->createSceneManager(Ogre::SceneType::ST_GENERIC);
    sceneManager->setCameraRelativeRendering(true);
	sceneManager->setAmbientLight(Ogre::ColourValue::White);

	// get resourcemanager singleton
	resMan = Ogre::ResourceGroupManager::getSingletonPtr();
	
	// create camera
	camera = sceneManager->createCamera("Camera1");
    camera->setPosition(*new ::Ogre::Vector3(0, 0, 0));
    camera->setNearClipDistance(1.0f);

	// create ViewPort
    viewport = renderWindow->addViewport(camera, 0);  

	// attach framelistener to inject timepulses
	root->addFrameListener(new MyFrameListener());

	/****************************************
	        2. INIT OIS
	*****************************************/

	size_t windowHnd = 0;
    renderWindow->getCustomAttribute("WINDOW", &windowHnd);
        
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;

	OIS::ParamList oisParameters = OIS::ParamList();	
    oisParameters.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    oisParameters.insert(std::make_pair(std::string("w32_mouse"), "DISCL_NONEXCLUSIVE"));
    oisParameters.insert(std::make_pair(std::string("w32_mouse"), "DISCL_FOREGROUND"));
           
    inputManager = OIS::InputManager::createInputSystem(oisParameters);
    oisKeyboard = (OIS::Keyboard*)inputManager->createInputObject(OIS::Type::OISKeyboard, true);
    oisMouse = (OIS::Mouse*)inputManager->createInputObject(OIS::Type::OISMouse, true);
       
    const OIS::MouseState &mouseState = oisMouse->getMouseState();
	mouseState.width = viewport->getActualWidth();
    mouseState.height = viewport->getActualHeight();
            
    OISKeyListener* keylistener = new OISKeyListener();
	OISMouseListener* mouselistener = new OISMouseListener();
		
	oisKeyboard->setEventCallback(keylistener);
	oisMouse->setEventCallback(mouselistener);
	
	/****************************************
	        3. INIT CEGUI
	*****************************************/

	renderer = &CEGUI::OgreRenderer::bootstrapSystem(*((::Ogre::RenderTarget*)renderWindow));
	ceguiSystem = ::CEGUI::System::getSingletonPtr();
	guiContext = &ceguiSystem->getDefaultGUIContext();

	// load resource to ogre
	resMan->createResourceGroup(RESGROUP_IMAGESETS);
	resMan->createResourceGroup(RESGROUP_FONTS);
	resMan->createResourceGroup(RESGROUP_SCHEMES);
	resMan->createResourceGroup(RESGROUP_LOOKNFEEL);
	resMan->createResourceGroup(RESGROUP_LAYOUTS);

	resMan->addResourceLocation("./ui/"RESGROUP_IMAGESETS, "FileSystem", RESGROUP_IMAGESETS);
	resMan->addResourceLocation("./ui/"RESGROUP_FONTS, "FileSystem", RESGROUP_FONTS);
	resMan->addResourceLocation("./ui/"RESGROUP_SCHEMES, "FileSystem", RESGROUP_SCHEMES);
	resMan->addResourceLocation("./ui/"RESGROUP_LOOKNFEEL, "FileSystem", RESGROUP_LOOKNFEEL);
	resMan->addResourceLocation("./ui/"RESGROUP_LAYOUTS, "FileSystem", RESGROUP_LAYOUTS);

	resMan->initialiseResourceGroup(RESGROUP_IMAGESETS);
	resMan->initialiseResourceGroup(RESGROUP_FONTS);
	resMan->initialiseResourceGroup(RESGROUP_SCHEMES);
	resMan->initialiseResourceGroup(RESGROUP_LOOKNFEEL);
	resMan->initialiseResourceGroup(RESGROUP_LAYOUTS);
				
	resMan->loadResourceGroup(RESGROUP_IMAGESETS);
	resMan->loadResourceGroup(RESGROUP_FONTS);
	resMan->loadResourceGroup(RESGROUP_SCHEMES);
	resMan->loadResourceGroup(RESGROUP_LOOKNFEEL);
	resMan->loadResourceGroup(RESGROUP_LAYOUTS);

	// set UI resourcegroups
	CEGUI::ImageManager::setImagesetDefaultResourceGroup(RESGROUP_IMAGESETS);
	CEGUI::Font::setDefaultResourceGroup(RESGROUP_FONTS);
	CEGUI::Scheme::setDefaultResourceGroup(RESGROUP_SCHEMES);
	CEGUI::WidgetLookManager::setDefaultResourceGroup(RESGROUP_LOOKNFEEL);
	CEGUI::WindowManager::setDefaultResourceGroup(RESGROUP_LAYOUTS);
		
	// load scheme
	scheme = &CEGUI::SchemeManager::getSingleton().createFromFile(UI_FILE_SCHEME);
		
	guiContext->getMouseCursor().setDefaultImage(DEFAULTARROW);		
	guiContext->setDefaultTooltipType(DEFAULTTOOLTIP);
				
	// get windowmanager
	CEGUI::WindowManager* wndMgr = CEGUI::WindowManager::getSingletonPtr();
	
	/****************************************
	        4. SETUP CEGUI TESTSTUFF
	*****************************************/
	
	// load layout/rootelement
	guiRoot = wndMgr->loadLayoutFromFile(UI_FILE_LAYOUT); 
		
	// set rootwindow
	guiContext->setRootWindow(guiRoot);

	// create test2
	CEGUI::PushButton* testlabel = (CEGUI::PushButton*)wndMgr->createWindow(
		"TaharezLook/Button", "testlabel");
	testlabel->setText("TEST!");
	testlabel->setTooltipText("TEST TEST TEST");
	testlabel->setArea(
		CEGUI::UDim(0.0f, 0.0f),
		CEGUI::UDim(0.0f, 0.0f),
		CEGUI::UDim(0.5f, 0.0f),
		CEGUI::UDim(1.0f, 0.0f));
		

	// create layoutcontainer
	CEGUI::HorizontalLayoutContainer* layout = (CEGUI::HorizontalLayoutContainer*)wndMgr->createWindow(
		"HorizontalLayoutContainer", "testlayout");
		
	
	layout->setArea(
		CEGUI::UDim(0.5f, 0.0f),
		CEGUI::UDim(0.0f, 0.0f),
		CEGUI::UDim(1.0f, 0.0f),
		CEGUI::UDim(1.0f, 0.0f));
	

	/*layout->setSize(CEGUI::USize(
		CEGUI::UDim(0.5f, 0.0f),
		CEGUI::UDim(1.0f, 0.0f)));*/
		
	
	// create widgets
	CEGUI::Window* widget1 = (CEGUI::Window*)wndMgr->createWindow(
		"TaharezLook/StaticImage", "widget1");
		
	CEGUI::Window* widget2 = (CEGUI::Window*)wndMgr->createWindow(
		"TaharezLook/Label", "widget2");

	CEGUI::Window* widget3 = (CEGUI::Window*)wndMgr->createWindow(
		"TaharezLook/Label", "widget3");
		
	// set sizes
	widget1->setSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget2->setSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget3->setSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
		
	widget1->setMaxSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget2->setMaxSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget3->setMaxSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));

	widget1->setMinSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget2->setMinSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));
	widget3->setMinSize(CEGUI::USize(CEGUI::UDim(0, 16), CEGUI::UDim(0, 16)));

	// set textvalues
	widget2->setText("--");
	widget3->setText("00");

	// add to layout
	layout->addChild(widget1);
	layout->addChild(widget2);
	layout->addChild(widget3);

	// add layout to rootelement
	guiRoot->addChild(layout);
	guiRoot->addChild(testlabel);

	/****************************************
	        4. APP MAINLOOP
	*****************************************/

	while(!renderWindow->isClosed())
	{
		// capture input
		oisMouse->capture();
		oisKeyboard->capture();

		// render one frame
		root->renderOneFrame();

		// generic WM_MESSAGE pump
		::Ogre::WindowEventUtilities::messagePump();
	}
	
	/****************************************
	        5. SHUTDOWN
	*****************************************/

	renderSystem->destroyRenderWindow(renderWindow->getName());             
    root->shutdown();
	
    return 0;
};

