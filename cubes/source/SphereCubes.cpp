#include "SphereCubes.h"

SphereCubes::SphereCubes():
    center(0, 0, 0), radius(3), 
    longitudeSteps(60), latitudeSteps(30) {
    generate();
}

SphereCubes::SphereCubes(const G3D::Point3& c, float r, int los, int las):
    center(c), radius(r), 
    longitudeSteps(los), latitudeSteps(las) {
    generate();
}

void SphereCubes::saveAny(const std::string& fileLoc) {
    any.save(G3D::String(fileLoc));
}

void SphereCubes::generate() {
    any.clear();

    any["name"] = "SphereCubes";

    Any cubeModel(Any::TABLE, "ArticulatedModel::Specification");
    cubeModel["filename"] = "model/crate/crate.obj";

    Any preprocess(Any::ARRAY);

    Any setMaterial(Any::ARRAY, "setMaterial");
    setMaterial.append(Any(Any::ARRAY, "all"));
    setMaterial.append("model/crate/woodcrate-L.png");
    preprocess.append(setMaterial);

    Any transformGeometry(Any::ARRAY, "transformGeometry");
    transformGeometry.append(Any(Any::ARRAY, "all"));
    transformGeometry.append(Matrix4::scale(0.2, 0.2, 0.2));
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

    Any cube(Any::TABLE, "VisibleEntity");
    cube["model"] = "cubeModel";

    int i = 0;
    float longitudeStepDegrees = 360.0f / longitudeSteps;
    float latitudeStepDegrees = 180.0f / latitudeSteps;
    float longtitude = 0.0f, latitude = 0.0f;

    bool southPole = false, northPole = false;

    for (int longitudeStep = 0; longitudeStep < longitudeSteps; ++longitudeStep) {
        latitude = 90.0f;
        for (int latitudeStep = 0; latitudeStep <= latitudeSteps; ++latitudeStep) {
            if (latitudeStep == 0 && northPole || latitudeStep == latitudeSteps && southPole) {
                latitude -= latitudeStepDegrees;
                continue;
            }

            if (latitudeStep == 0) {
                northPole = true;
            } else if (latitudeStep == latitudeSteps) {
                southPole = true;
            }

            cube["frame"] =
                (Matrix4::translation(center)
                    * Matrix4::yawDegrees(longtitude)
                    * Matrix4::rollDegrees(latitude)
                    * Matrix4::translation(Vector3(radius, 0, 0))).approxCoordinateFrame();
            entities.set(G3D::String("cube" + std::to_string(i++)), cube);

            latitude -= latitudeStepDegrees;
        }
        
        longtitude += longitudeStepDegrees;
    }

    any["entities"] = entities;
}