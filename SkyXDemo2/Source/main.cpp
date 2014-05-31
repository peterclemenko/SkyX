/// SkyX 0.4 Demo 2: Presets

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
Ogre::uint32 mCurrentPreset = 0;
Ogre::Camera* mRenderingCamera = 0;

/** SkyX settings struct
    @remarks These just are the most important SkyX parameters, not all SkyX parameters.
 */
struct SkyXSettings
{
	/** Constructor
	    @remarks Skydome + vol. clouds + lightning settings
	 */
	SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt,
		const bool& lc, const bool& vc, const Ogre::Real& vcws, const bool& vcauto, const Ogre::Radian& vcwd, 
		const Ogre::Vector3& vcac, const Ogre::Vector4& vclr,  const Ogre::Vector4& vcaf, const Ogre::Vector2& vcw,
		const bool& vcl, const Ogre::Real& vclat, const Ogre::Vector3& vclc, const Ogre::Real& vcltm)
		: time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws)
		, vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw)
		, vcLightnings(vcl), vcLightningsAT(vclat), vcLightningsColor(vclc), vcLightningsTM(vcltm)
	{}

	/** Constructor
	    @remarks Skydome + vol. clouds
	 */
	SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt,
		const bool& lc, const bool& vc, const Ogre::Real& vcws, const bool& vcauto, const Ogre::Radian& vcwd, 
		const Ogre::Vector3& vcac, const Ogre::Vector4& vclr,  const Ogre::Vector4& vcaf, const Ogre::Vector2& vcw)
		: time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws)
		, vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw), vcLightnings(false)
	{}

	/** Constructor
	    @remarks Skydome settings
	 */
	SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt, const bool& lc)
		: time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(false), vcLightnings(false)
	{}

	/// Time
	Ogre::Vector3 time;
	/// Time multiplier
	Ogre::Real timeMultiplier;
	/// Moon phase
	Ogre::Real moonPhase;
	/// Atmosphere options
	SkyX::AtmosphereManager::Options atmosphereOpt;
	/// Layered clouds?
	bool layeredClouds;
	/// Volumetric clouds?
	bool volumetricClouds;
	/// VClouds wind speed
	Ogre::Real vcWindSpeed;
	/// VClouds autoupdate
	bool vcAutoupdate;
	/// VClouds wind direction
	Ogre::Radian vcWindDir;
	/// VClouds ambient color
	Ogre::Vector3 vcAmbientColor;
	/// VClouds light response
	Ogre::Vector4 vcLightResponse;
	/// VClouds ambient factors
	Ogre::Vector4 vcAmbientFactors;
	/// VClouds wheater
	Ogre::Vector2 vcWheater;
	/// VClouds lightnings?
	bool vcLightnings;
	/// VClouds lightnings average aparition time
	Ogre::Real vcLightningsAT;
	/// VClouds lightnings color
	Ogre::Vector3 vcLightningsColor;
	/// VClouds lightnings time multiplier
	Ogre::Real vcLightningsTM;
};

/** Demo presets
    @remarks The best way of determinate each parameter value is by using a real-time editor.
			 These presets have been quickly designed using the Paradise Editor, which is a commercial solution.
			 At the time I'm writting these lines, SkyX 0.1 is supported by Ogitor. Hope that the Ogitor team will 
			 support soon SkyX 0.4, this way you all are going to be able to quickly create cool SkyX configurations.
 */
SkyXSettings mPresets[] = {
	// Sunset
	SkyXSettings(Ogre::Vector3(8.85f, 7.5f, 20.5f),  -0.08f, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Ogre::Radian(270), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.35, 0.2, 0.92, 0.1), Ogre::Vector4(0.4, 0.7, 0, 0), Ogre::Vector2(0.8,1)),
	// Clear
	SkyXSettings(Ogre::Vector3(17.16f, 7.5f, 20.5f), 0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4), false),
	// Thunderstorm 1
	SkyXSettings(Ogre::Vector3(12.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.25, 0.4, 0.5, 0.1), Ogre::Vector4(0.45, 0.3, 0.6, 0.1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(1,0.976,0.92), 2),
	// Thunderstorm 2
	SkyXSettings(Ogre::Vector3(10.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0, 0.02, 0.34, 0.24), Ogre::Vector4(0.29, 0.3, 0.6, 1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(0.95,1,1), 2),
	// Desert
	SkyXSettings(Ogre::Vector3(7.59f, 7.5f, 20.5f), 0, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1), true),
	// Night
	SkyXSettings(Ogre::Vector3(21.5f, 7.5, 20.5), 0.03, -0.25, SkyX::AtmosphereManager::Options(), true)
};

Ogre::String buildInfoStr()
{
	Ogre::String str = "SkyX 0.4 demo - Select preset: [1] Desert, [2] Clear, [3] Thunderstorm (1), [4] Thunderstorm (2), [5] Sunset, [6] Night\nCurrent preset: ";

	switch (mCurrentPreset)
	{
		case 0: {str += "1 - Sunset";} break;
		case 1: {str += "2 - Clear";} break;
		case 2: {str += "3 - Thunderstorm (1)";} break;
		case 3: {str += "4 - Thunderstorm (2)";} break;
		case 4: {str += "5 - Desert";} break;
		case 5: {str += "6 - Night";} break;
	}

	return str;
}

void setPreset(const SkyXSettings& preset)
{
	mSkyX->setTimeMultiplier(preset.timeMultiplier);
	mBasicController->setTime(preset.time);
	mBasicController->setMoonPhase(preset.moonPhase);
	mSkyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

	// Layered clouds
	if (preset.layeredClouds)
	{
		// Create layer cloud
		if (mSkyX->getCloudsManager()->getCloudLayers().empty())
		{
			mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
		}
	}
	else
	{
		// Remove layer cloud
		if (!mSkyX->getCloudsManager()->getCloudLayers().empty())
		{
			mSkyX->getCloudsManager()->removeAll();
		}
	}

	mSkyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
	mSkyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

	SkyX::VClouds::VClouds* vclouds = mSkyX->getVCloudsManager()->getVClouds();

	vclouds->setWindDirection(preset.vcWindDir);
	vclouds->setAmbientColor(preset.vcAmbientColor);
	vclouds->setLightResponse(preset.vcLightResponse);
	vclouds->setAmbientFactors(preset.vcAmbientFactors);
	vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

	if (preset.volumetricClouds)
	{
		// Create VClouds
		if (!mSkyX->getVCloudsManager()->isCreated())
		{
			// SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
			mSkyX->getVCloudsManager()->create(mSkyX->getMeshManager()->getSkydomeRadius(mRenderingCamera));
		}
	}
	else
	{
		// Remove VClouds
		if (mSkyX->getVCloudsManager()->isCreated())
		{
			mSkyX->getVCloudsManager()->remove();
		}
	}

	vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
	vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
	vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
	vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);

	mTextArea->setCaption(buildInfoStr());

	// Reset camera position/orientation
	mRenderingCamera->setPosition(0,0,0);
	mRenderingCamera->setDirection(0,0,1);

	mSkyX->update(0);
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

		// Clamp camera height
		if (mCamera->getDerivedPosition().y < -400)
		{
			mCamera->setPosition(Ogre::Vector3(mCamera->getDerivedPosition().x, -400, mCamera->getDerivedPosition().z));
		}

		// Switch presets
		if (mKeyboard->isKeyDown(OIS::KC_1) && mKeyBuffer < 0)
		{
			mCurrentPreset = 0;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}
		else if (mKeyboard->isKeyDown(OIS::KC_2) && mKeyBuffer < 0)
		{
			mCurrentPreset = 1;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}
		else if (mKeyboard->isKeyDown(OIS::KC_3) && mKeyBuffer < 0)
		{
			mCurrentPreset = 2;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}
		else if (mKeyboard->isKeyDown(OIS::KC_4) && mKeyBuffer < 0)
		{
			mCurrentPreset = 3;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}
		else if (mKeyboard->isKeyDown(OIS::KC_5) && mKeyBuffer < 0)
		{
			mCurrentPreset = 4;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}
		else if (mKeyboard->isKeyDown(OIS::KC_6) && mKeyBuffer < 0)
		{
			mCurrentPreset = 5;
			setPreset(mPresets[mCurrentPreset]);

			mKeyBuffer = 0.25f;
		}

		mKeyBuffer -= e.timeSinceLastFrame;

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
		// Set camera far/near clip distances
        mCamera->setFarClipDistance(30000);
		mCamera->setNearClipDistance(20);

		// Create our text area for display SkyX parameters
		createTextArea();

		// Create SkyX
		mBasicController = new SkyX::BasicController();
		mSkyX = new SkyX::SkyX(mSceneMgr, mBasicController);
		mSkyX->create();

		// Distance geometry falling is a feature introduced in SkyX 0.2
		// When distance falling is enabled, the geometry linearly falls with the distance and the
		// amount of falling in world units is determinated by the distance between the cloud field "plane"
		// and the camera height multiplied by the falling factor.
		// For this demo, a falling factor of two is good enough for the point of view we're using. That means that if the camera
		// is at a distance of 100 world units from the cloud field, the fartest geometry will fall 2*100 = 200 world units.
		// This way the cloud field covers a big part of the sky even if the camera is in at a very low altitude.
		// The second parameter is the max amount of falling distance in world units. That's needed when for example, you've an 
		// ocean and you don't want to have the volumetric cloud field geometry falling into the water when the camera is underwater.
		// -1 means that there's not falling limit.
		mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));

		// Register SkyX listeners
		mRoot->addFrameListener(mSkyX);
		mWindow->addListener(mSkyX);

		mRenderingCamera = mCamera;

		setPreset(mPresets[mCurrentPreset]);
    }

	void createFrameListener()
	{
		// Add frame listener
		mRoot->addFrameListener(new SkyXDemoFrameListener(mWindow, mCamera, mSceneMgr));
	}

	void chooseSceneManager()
    {
        // Create the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
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
