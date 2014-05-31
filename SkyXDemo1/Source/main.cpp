/// SkyX 0.4 Demo 1: Terrain integration - Only until Ogre3d 1.7.X
/// The terrain scene manager was removed from Ogre in 1.8

// ---------------------------------------------------------------------------
// Include SkyX header files
// ---------------------------------------------------------------------------
#include <SkyX.h>

// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------
#include <Ogre.h>
#include <Overlay/OgreTextAreaOverlayElement.h>

// ----------------------------------------------------------------------------
// Include the OGRE example framework
// This includes the classes defined to make getting an OGRE application running
// a lot easier. It automatically sets up all the main objects and allows you to
// just override the bits you want to instead of writing it all from scratch.
// ----------------------------------------------------------------------------
#include <ExampleApplication.h>

// ----------------------------------------------------------------------------
// SkyX-related ptrs
// ----------------------------------------------------------------------------
SkyX::SkyX* mSkyX = 0;
SkyX::BasicController* mBasicController = 0;

// ----------------------------------------------------------------------------
// Misc
// ----------------------------------------------------------------------------
Ogre::TextAreaOverlayElement* mTextArea = 0;
bool mShowInformation = false;


Ogre::String getConfigStringFromSkyXAtmosphereOptions(const SkyX::AtmosphereManager::Options& Options)
{
	Ogre::Vector3 time = mBasicController->getTime();

	int hour = static_cast<int>(time.x);
	int min  = static_cast<int>((time.x - hour)*60);

	Ogre::String timeStr = Ogre::StringConverter::toString(hour) + ":" + Ogre::StringConverter::toString(min);
	Ogre::String str = "SkyX 0.4 demo (Press F1 to show/hide information)" + static_cast<Ogre::String>(mShowInformation ? " - Simuation paused - \n" : "\n-------------------------------------------------------------\nTime: " + timeStr + "\n");

	if (mShowInformation)
	{
		str += "-------------------------------------------------------------\n";
		str += "Time: " + timeStr + " [1, Shift+1] (+/-).\n";
		str += "Rayleigh multiplier: " + Ogre::StringConverter::toString(Options.RayleighMultiplier) + " [2, Shift+2] (+/-).\n";
		str += "Mie multiplier: " + Ogre::StringConverter::toString(Options.MieMultiplier) + " [3, Shift+3] (+/-).\n";
		str += "Exposure: " + Ogre::StringConverter::toString(Options.Exposure) + " [4, Shift+4] (+/-).\n";
		str += "Inner radius: " + Ogre::StringConverter::toString(Options.InnerRadius) + " [5, Shift+5] (+/-).\n";
		str += "Outer radius: " + Ogre::StringConverter::toString(Options.OuterRadius) + " [6, Shift+6] (+/-).\n";
		str += "Number of samples: " + Ogre::StringConverter::toString(Options.NumberOfSamples) + " [7, Shift+7] (+/-).\n";
		str += "Height position: " + Ogre::StringConverter::toString(Options.HeightPosition) + " [8, Shift+8] (+/-).\n";
		str += "Moon phase: " + Ogre::StringConverter::toString(mBasicController->getMoonPhase()) + " [9, Shift+9] (+/-).\n";
	}

	return str;
}

// ----------------------------------------------------------------------------
// Define the application object
// This is derived from ExampleApplication which is the class OGRE provides to
// make it easier to set up OGRE without rewriting the same code all the time.
// You can override extra methods of ExampleApplication if you want to further
// specialise the setup routine, otherwise the only mandatory override is the
// 'createScene' method which is where you set up your own personal scene.
// ----------------------------------------------------------------------------
class SkyXDemoFrameListener : public ExampleFrameListener
{
public:
    SceneManager *mSceneMgr;
    Real mKeyBuffer;

    SkyXDemoFrameListener(RenderWindow* win, Camera* cam, SceneManager *sm)
            : ExampleFrameListener(win,cam)
            , mSceneMgr(sm)
            , mKeyBuffer(-1)
    {
    }

    bool frameStarted(const FrameEvent &e)
    {
        mKeyboard->capture();

		// Check camera height
		Ogre::RaySceneQuery * raySceneQuery = 
			mSceneMgr->
			     createRayQuery(Ogre::Ray(mCamera->getPosition() + Ogre::Vector3(0,1000000,0), 
				                Vector3::NEGATIVE_UNIT_Y));
		Ogre::RaySceneQueryResult& qryResult = raySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
        if (i != qryResult.end() && i->worldFragment)
        {
			if (mCamera->getDerivedPosition().y < i->worldFragment->singleIntersection.y + 30)
			{
                mCamera->
				     setPosition(mCamera->getPosition().x, 
                                 i->worldFragment->singleIntersection.y + 30, 
                                 mCamera->getPosition().z);
			}
        }

		SkyX::AtmosphereManager::Options SkyXOptions = mSkyX->getAtmosphereManager()->getOptions();

		// Time
		if (!mShowInformation)
		{
			mSkyX->setTimeMultiplier(0.1f);
		}
		else
		{
			mSkyX->setTimeMultiplier(0.0f);
		}

		if (mKeyboard->isKeyDown(OIS::KC_1) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			mSkyX->setTimeMultiplier(1.0f);
		if (mKeyboard->isKeyDown(OIS::KC_1) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			mSkyX->setTimeMultiplier(-1.0f);

		// Rayleigh multiplier
		if (mKeyboard->isKeyDown(OIS::KC_2) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.RayleighMultiplier += e.timeSinceLastFrame*0.025f;
		if (mKeyboard->isKeyDown(OIS::KC_2) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.RayleighMultiplier -= e.timeSinceLastFrame*0.025f;

		// Mie multiplier
		if (mKeyboard->isKeyDown(OIS::KC_3) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.MieMultiplier += e.timeSinceLastFrame*0.025f;
		if (mKeyboard->isKeyDown(OIS::KC_3) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.MieMultiplier -= e.timeSinceLastFrame*0.025f;

		// Exposure
		if (mKeyboard->isKeyDown(OIS::KC_4) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.Exposure += e.timeSinceLastFrame*0.5f;
		if (mKeyboard->isKeyDown(OIS::KC_4) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.Exposure -= e.timeSinceLastFrame*0.5f;

		// Inner radius
		if (mKeyboard->isKeyDown(OIS::KC_5) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.InnerRadius += e.timeSinceLastFrame*0.25f;
		if (mKeyboard->isKeyDown(OIS::KC_5) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.InnerRadius -= e.timeSinceLastFrame*0.25f;

		// Outer radius
		if (mKeyboard->isKeyDown(OIS::KC_6) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.OuterRadius += e.timeSinceLastFrame*0.25f;
		if (mKeyboard->isKeyDown(OIS::KC_6) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.OuterRadius -= e.timeSinceLastFrame*0.25f;

		// Number of samples
		if (mKeyboard->isKeyDown(OIS::KC_7) && mKeyBuffer < 0 &&!(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
		{
			SkyXOptions.NumberOfSamples ++;
			mKeyBuffer = 0.25f;
		}
		if (mKeyboard->isKeyDown(OIS::KC_7) && mKeyBuffer < 0 && (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
		{
			SkyXOptions.NumberOfSamples --;
			mKeyBuffer = 0.25f;
		}

		// Height position
		if (mKeyboard->isKeyDown(OIS::KC_8) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.HeightPosition += e.timeSinceLastFrame*0.05f;
		if (mKeyboard->isKeyDown(OIS::KC_8) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
			SkyXOptions.HeightPosition -= e.timeSinceLastFrame*0.05f;

		// Moon phase
		if (mKeyboard->isKeyDown(OIS::KC_9) && !(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
		{
			mBasicController->setMoonPhase(mBasicController->getMoonPhase() + e.timeSinceLastFrame*0.25f);
			
			if (mBasicController->getMoonPhase() > 1)
			{
				mBasicController->setMoonPhase(1);
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_9) &&  (mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_RSHIFT)))
		{
			mBasicController->setMoonPhase(mBasicController->getMoonPhase() - e.timeSinceLastFrame*0.25f);
			
			if (mBasicController->getMoonPhase() < -1)
			{
				mBasicController->setMoonPhase(-1);
			}
		}

		mSkyX->getAtmosphereManager()->setOptions(SkyXOptions);

		// Show/Hide information
		if (mKeyboard->isKeyDown(OIS::KC_F1) && mKeyBuffer < 0)
        {
			mShowInformation = !mShowInformation;

			mKeyBuffer = 0.25f;
		}

		mTextArea->setCaption(getConfigStringFromSkyXAtmosphereOptions(SkyXOptions));

		mKeyBuffer -= e.timeSinceLastFrame;
		
		// Update terrain material
		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Terrain"))->getTechnique(0)->getPass(0)
			->getFragmentProgramParameters()->setNamedConstant("uLightY", mBasicController->getSunDirection().y);
		
        return true;
    }
};

class SampleApp : public ExampleApplication
{
public:
    // Basic constructor
    SampleApp()
    {}

protected:

    // Just override the mandatory create scene method
    void createScene(void)
    {
		// Set some camera params
        mCamera->setFarClipDistance(30000);
		mCamera->setNearClipDistance(20);
		mCamera->setPosition(20000, 1800, 20000);
		mCamera->setDirection(1, 0, 1);

		// Create our text area for display SkyX parameters
		createTextArea();

		// Create SkyX
		mBasicController = new SkyX::BasicController();
		mSkyX = new SkyX::SkyX(mSceneMgr, mBasicController);
		mSkyX->create();

		mBasicController->setMoonPhase(0.75f);

		// Register listeners
		mRoot->addFrameListener(mSkyX);
		mWindow->addListener(mSkyX);

		// Add the atmospheric scattering pass to our terrain material
		// The ground atmospheric scattering pass must be added to all materials that are going to be used to 
		// render object which we want to have atmospheric scattering effects on. 
		mSkyX->getGPUManager()->addGroundPass(
			static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().
			getByName("Terrain"))->getTechnique(0)->createPass(), 5000, Ogre::SBT_TRANSPARENT_COLOUR);

		// Create the terrain
	    mSceneMgr->setWorldGeometry("Terrain.cfg");

		// Add a basic cloud layer
		mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
    }

	void createFrameListener()
	{
		// Add frame listener
		mRoot->addFrameListener(new SkyXDemoFrameListener(mWindow, mCamera, mSceneMgr));
	}

	void chooseSceneManager()
    {
        // Create the SceneManager, in this case terrain scene manager
        mSceneMgr = mRoot->createSceneManager("TerrainSceneManager");
    }

	// Create text area for SkyX parameters
	void createTextArea()
	{
		// Create a panel
		Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
			OverlayManager::getSingleton().createOverlayElement("Panel", "SkyXParametersPanel"));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(10, 10);
		panel->setDimensions(400, 400);

		// Create a text area
		mTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
			OverlayManager::getSingleton().createOverlayElement("TextArea", "SkyXParametersTextArea"));
		mTextArea->setMetricsMode(Ogre::GMM_PIXELS);
		mTextArea->setPosition(0, 0);
		mTextArea->setDimensions(100, 100);
		mTextArea->setCaption("SkyX plugin demo");
		mTextArea->setCharHeight(16);
		mTextArea->setFontName("BlueHighway");
		mTextArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
		mTextArea->setColourTop(ColourValue(0.5, 0.7, 0.5));

		// Create an overlay, and add the panel
		Ogre::Overlay* overlay = OverlayManager::getSingleton().create("OverlayName");
		overlay->add2D(panel);

		// Add the text area to the panel
		panel->addChild(mTextArea);

		// Show the overlay
		overlay->show();
	}
};

// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	try
	{
		SampleApp app;
		app.go();
	}
	catch (Ogre::Exception &e)
	{
		std::cerr << "An exception has occured: " << e.getFullDescription();

		return -1;
	}

	return 0;
}
