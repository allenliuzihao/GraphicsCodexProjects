#include "Staircase.h"

Staircase::Staircase(): 
    center(0.0, 0.0, 0.0), rotateDegree(15), 
    stairLength(1.5), stairWidth(0.35), stairHeight(0.1),
    numSteps(100) {
    generate();
}

Staircase::Staircase(const G3D::Point3& pt, float r, float sl, float sw, float sh, int ns):
    center(pt), rotateDegree(r),
    stairLength(sl), stairWidth(sw), stairHeight(sh),
    numSteps(ns) {
    generate();
}

void Staircase::saveAny(const std::string& fileLoc) {
    any.save(G3D::String(fileLoc));
}

void Staircase::generate() {
    any.clear();

    any["name"] = "Staircase";

    Any cubeModel(Any::TABLE, "ArticulatedModel::Specification");
    cubeModel["filename"] = "model/crate/crate.obj";

    Any preprocess(Any::ARRAY);    

    Any setMaterial(Any::ARRAY, "setMaterial");
    setMaterial.append(Any(Any::ARRAY, "all"));
    setMaterial.append("model/crate/metalcrate-L.png");
    preprocess.append(setMaterial);
    
    Any transformGeometry(Any::ARRAY, "transformGeometry");
    transformGeometry.append(Any(Any::ARRAY, "all"));
    transformGeometry.append(Matrix4::scale(stairLength, stairHeight, stairWidth));
    preprocess.append(transformGeometry);
    
    cubeModel["preprocess"] = preprocess;

    Any models(Any::TABLE);
    models["cubeModel"] = cubeModel;
    any["models"] = models;

    Any entities(Any::TABLE);
    
    Any skybox(Any::TABLE, "Skybox");
    skybox["texture"] = "cubemap/whiteroom/whiteroom-*.png";
    entities["skybox"] = skybox;

    Any sun(Any::TABLE, "Light");
    sun["attenuation"] = G3D::Vector3(0, 0, 1);
    sun["bulbPower"] = G3D::Power3(1e+005);
    sun["frame"] = G3D::CFrame::fromXYZYPRDegrees(-15, 207, -41, -164, -77, 77);
    sun["shadowMapSize"] = G3D::Vector2int16(2048, 2048);
    sun["spotHalfAngleDegrees"] = 5;
    sun["rectangular"] = true;
    sun["type"] = "SPOT";
    entities["sun"] = sun;

    Any camera(Any::TABLE, "Camera");
    camera["frame"] = G3D::CFrame::fromXYZYPRDegrees(0, 0, 5);
    entities["camera"] = camera;

    float angle = 0.0;
    float currHeight = 0.0;

    Any cube(Any::TABLE, "VisibleEntity");
    cube["model"] = "cubeModel";

    for (int i = 0; i < numSteps; ++i) {
        cube["frame"] = 
            (Matrix4::translation(center) 
                * Matrix4::yawDegrees(angle) 
                * Matrix4::translation(G3D::Point3(stairLength / 2.0f, 0, 0) + Vector3(0, currHeight, 0))).approxCoordinateFrame();

        entities.set(G3D::String("cube" + std::to_string(i)), cube);

        angle += rotateDegree;
        currHeight += stairHeight;
    }

    any["entities"] = entities;
}