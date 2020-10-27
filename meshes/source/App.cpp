/** \file App.cpp */
#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    initGLG3D(G3DSpecification());

    GApp::Settings settings(argc, argv);

    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.caption             = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;
   
    settings.window.fullScreen          = false;
    if (settings.window.fullScreen) {
        settings.window.width           = 1920;
        settings.window.height          = 1080;
    } else {
        settings.window.width           = OSWindow::primaryDisplayWindowSize().x;
        settings.window.height          = OSWindow::primaryDisplayWindowSize().y; 

        // Make even
        settings.window.height -= settings.window.height & 1;
    }
    settings.window.resizable           = ! settings.window.fullScreen;
    settings.window.framed              = ! settings.window.fullScreen;
    settings.window.defaultIconFilename = "icon.png";

    // Set to true for a significant performance boost if your app can't
    // render at the display frequency, or if you *want* to render faster
    // than the display.
    settings.window.asynchronous        = true;

    // Render slightly larger than the screen so that screen-space refraction, bloom,
    // screen-space AO, and screen-space reflection to look good at screen edges. Set to zero for
    // maximum performance and free memory. Increase the second argument to improve AO without affecting
    // color. The third argument is the resolution. Set to 0.5f to render at half-res and upscale,
    // 2.0f to supersample.
    settings.hdrFramebuffer.setGuardBandsAndSampleRate(64, 0, 1.0f);

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = true;

    settings.dataDir                       = FileSystem::currentDirectory();

    settings.screenCapture.outputDirectory = FilePath::concat(FileSystem::currentDirectory(), "../journal");
    if (! FileSystem::exists(settings.screenCapture.outputDirectory)) {
        settings.screenCapture.outputDirectory = "";
    }
    settings.screenCapture.includeAppRevision = false;
    settings.screenCapture.includeG3DRevision = false;
    settings.screenCapture.filenamePrefix = "_";

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
}


// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {
    GApp::onInit();

    this->cube.setCenter(Point3(0, 0, 0));
    this->cube.setLength(1);
    this->cube.save();

    this->cylinder.setCenter(Point3(-5, -5, -5));
    this->cylinder.setHeight(5);
    this->cylinder.setRadius(3);
    this->cylinder.save();
    
    // height field does take sometime to generate so disable for now.
    this->heightField.setLowerLeft(Point3(5, 5, 5));
    this->heightField.setHeight(0.5f);
    this->heightField.setPixelLength(0.01f);
    this->heightField.save();

    setFrameDuration(1.0f / 240.0f);

    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.
    
    showRenderingStats      = false;

    loadScene(

#       ifndef G3D_DEBUG
        "G3D Sponza"
        //"Figure Inside Outside House"
        //"Two Rooms"
        //"G3D Veach Door"
        //"G3D Simple Cornell Box (Mirror)"
        //"G3D Simple Cornell Box (Area Light)" // Load something simple
        //"G3D Bistro Exterior (Night)"
        //"G3D Simple Texture"
#       else
        //"G3D Sponza"
        //"G3D Veach Door"
        "G3D Simple Cornell Box (Area Light)" // Load something simple
        //"G3D Simple Cornell Box (Mirror)" // Load something simple
        //"Test WaveVK Materials"
        //"Two Rooms"
#       endif
//        "G3D Debug Transparency"
        //developerWindow->sceneEditorWindow->selectedSceneName()  // Load the first scene encountered 
        );

    // Make the GUI after the scene is loaded because loading/rendering/simulation initialize
    // some variables that advanced GUIs may wish to reference with pointers.
    makeGUI();
}


void App::makeGUI() {
    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);

    GuiTabPane* tabPane = debugPane->addTabPane();
    GuiPane* cylinderPane = tabPane->addTab("Cylinder");
    cylinderPane->setNewChildSize(400, -1, 150);
    
    cylinderPane->addNumberBox<float>("height", &m_cylinder_height, "", GuiTheme::LINEAR_SLIDER, 1.0f, 20.0f);
    cylinderPane->addNumberBox<float>("radius", &m_cylinder_radius, "", GuiTheme::LINEAR_SLIDER, 1.0f, 10.0f);
    cylinderPane->addButton("Generate Cylinder", [this]() {
        drawMessage("Loading Cylinder...");
        cylinder.set(m_cylinder_height, m_cylinder_radius);
        cylinder.save(true);

        ArticulatedModel::clearCache();
        loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
    });
    cylinderPane->pack();

    GuiPane* cubePane = tabPane->addTab("Cube");
    cubePane->setNewChildSize(400, -1, 150);
    cubePane->addNumberBox<float>("length", &m_cube_length, "", GuiTheme::LINEAR_SLIDER, 1.0f, 20.0f);
    cubePane->addButton("Generate Cube", [this]() {
        drawMessage("Loading Cube...");
        cube.setLength(m_cube_length);
        cube.save(true);

        ArticulatedModel::clearCache();
        loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
    });

    GuiPane* heightFieldPane = tabPane->addTab("Height Field");
    heightFieldPane->setNewChildSize(400, -1, 150);
    heightFieldPane->addNumberBox<float>("pixel length", &m_pixel_length, "", GuiTheme::LINEAR_SLIDER, 0.01f, 5.0f);
    heightFieldPane->addNumberBox<float>("height", &m_height, "", GuiTheme::LINEAR_SLIDER, 0.01f, 10.0f);

    GuiTextBox* textBox = heightFieldPane->addTextBox("file name", &m_filename, GuiTextBox::IMMEDIATE_UPDATE);
    textBox->setFocused(true);
    heightFieldPane->addButton("...", [this]() {FileDialog::getFilename(m_filename, "png", false); });

    heightFieldPane->addButton("Generate Height Field", [this]() {
        drawMessage("Loading height field...");
        
        heightField.setHeight(m_height);
        heightField.setPixelLength(m_pixel_length);
        heightField.setImage(m_filename);
        heightField.save(true);

        ArticulatedModel::clearCache();
        loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
    });

    heightFieldPane->pack();
    
    cubePane->pack();
    tabPane->pack();
    
    // More examples of debugging GUI controls:
    // debugPane->addCheckBox("Use explicit checking", &explicitCheck);
    // debugPane->addTextBox("Name", &myName);
    // debugPane->addNumberBox("height", &height, "m", GuiTheme::LINEAR_SLIDER, 1.0f, 2.5f);
    // button = debugPane->addButton("Run Simulator");
    // debugPane->addButton("Generate Heightfield", [this](){ generateHeightfield(); });
    // debugPane->addButton("Generate Heightfield", [this](){ makeHeightfield(imageName, scale, "model/heightfield.off"); });

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
    // TODO: restore
    BEGIN_PROFILER_EVENT("App::onGraphics3D");
    if (! scene()) {
        if ((submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) && (!rd->swapBuffersAutomatically())) {
            swapBuffers();
        }
        rd->clear();
        rd->pushState(); {
            rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
            drawDebugShapes();
        } rd->popState();
        return;
    }

    GBuffer::Specification gbufferSpec = m_gbufferSpecification;

    gbufferSpec.encoding[GBuffer::Field::WS_POSITION] = ImageFormat::RGBA32F();
    // Needs to be the same format as the camera space normal because we are debugging precision...
    gbufferSpec.encoding[GBuffer::Field::WS_NORMAL] = ImageFormat::RGBA32F();

    extendGBufferSpecification(gbufferSpec);
    m_gbuffer->setSpecification(gbufferSpec);
    m_gbuffer->resize(m_framebuffer->width(), m_framebuffer->height());
    m_gbuffer->prepare(rd, activeCamera(), 0, -(float)previousSimTimeStep(), m_settings.hdrFramebuffer.depthGuardBandThickness, m_settings.hdrFramebuffer.colorGuardBandThickness);

    m_renderer->render(rd,
        activeCamera(),
        m_framebuffer,
        scene()->lightingEnvironment().ambientOcclusionSettings.enabled ? m_depthPeelFramebuffer : nullptr,
        scene()->lightingEnvironment(), m_gbuffer,
        allSurfaces,
        scene()->tritree());

    // Debug visualizations and post-process effects
    rd->pushState(m_framebuffer); {
        // Call to make the App show the output of debugDraw(...)
        rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
        drawDebugShapes();
        const shared_ptr<Entity>& selectedEntity = (notNull(developerWindow) && notNull(developerWindow->sceneEditorWindow)) ? developerWindow->sceneEditorWindow->selectedEntity() : nullptr;
        scene()->visualize(rd, selectedEntity, allSurfaces, sceneVisualizationSettings(), activeCamera());

        onPostProcessHDR3DEffects(rd);
    } rd->popState();

    // We're about to render to the actual back buffer, so swap the buffers now.
    // This call also allows the screenshot and video recording to capture the
    // previous frame just before it is displayed.
    if (submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) {
        swapBuffers();
    }

    // Clear the entire screen (needed even though we'll render over it, since
    // AFR uses clear() to detect that the buffer is not re-used.)
    rd->clear();

    // Perform gamma correction, bloom, and AA, and write to the native window frame buffer
    m_film->exposeAndRender(rd, activeCamera()->filmSettings(), m_framebuffer->texture(0), 
        settings().hdrFramebuffer.trimBandThickness().x,
        settings().hdrFramebuffer.depthGuardBandThickness.x,
        Texture::opaqueBlackIfNull(notNull(m_gbuffer) ? m_gbuffer->texture(GBuffer::Field::SS_POSITION_CHANGE) : nullptr),
        activeCamera()->jitterMotion());
    END_PROFILER_EVENT();
}


void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool App::onEvent(const GEvent& event) {
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { 
        const shared_ptr<DefaultRenderer>& r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
        r->setDeferredShading(! r->deferredShading());
        return true; 
    }

    return false;
}


void App::onUserInput(UserInput* ui) {
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void App::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) {
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}

void App::drawDebugShapes() {
    GApp::drawDebugShapes();

    const shared_ptr<DefaultRenderer>& defaultRenderer = dynamic_pointer_cast<DefaultRenderer>(m_renderer);

    if (isNull(defaultRenderer)) {
        return;
    }

    const Array<shared_ptr<DDGIVolume>>& volumeArray = scene()->lightingEnvironment().ddgiVolumeArray;

    // Draw debug shapes for the DDGIVolume.
    for (int i = 0; i < volumeArray.size(); ++i) {
        if (defaultRenderer->m_showProbeLocations[i]) {
            // TODO: enable probe visualization radius.
            float radius = 0.1f;
            // TODO: enable depth visualization from second to last parameter.
            defaultRenderer->m_ddgiVolumeArray[i]->debugRenderProbeVisualization(renderDevice, activeCamera(), false, radius);
        }
    }
}
