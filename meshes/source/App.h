/**
  \file App.h

  The G3D 10.00 default starter app is configured for OpenGL 4.1 and
  relatively recent GPUs.
 */
#pragma once
#include <G3D/G3D.h>

#include "Cube.h"
#include "Cylinder.h"
#include "HeightField.h"
#include "Glass.h"

/** \brief Application framework. */
class App : public GApp {
protected:

    float m_cylinder_height = 5.0f;
    float m_cylinder_radius = 3.0f;
    float m_cube_length = 1.0f;
    float m_height = 0.5f;
    float m_pixel_length = 0.01f;

    int m_number_of_slices = 10;

    String m_filename = "image/height_field.png";

    zl::Cube cube;
    zl::Cylinder cylinder;
    zl::HeightField heightField;
    zl::Glass glass;

    /** Called from onInit */
    void makeGUI();

public:
    
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(Array<shared_ptr<Surface> >& posed3D, Array<shared_ptr<Surface2D> >& posed2D) override;

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;

    virtual void drawDebugShapes() override;
};
