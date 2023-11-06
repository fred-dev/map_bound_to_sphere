//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxMaps.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;

    std::shared_ptr<ofxMaps::MBTilesCache> bufferCache;
    std::shared_ptr<ofxMaps::MapTileLayer> tileLayer;
    std::shared_ptr<ofxMaps::MapTileSet> tileSet;
    std::shared_ptr<ofxMaps::MapTileProvider> tileProvider;


    ofEasyCam cam;
    ofSpherePrimitive globe;
    float globeRadius = 100.0;
    ofFbo mapFbo;
    ofImage earthTex;
    
    ofMesh tempMesh;
    
    ofxGeo::Coordinate chicago;
    ofxGeo::Coordinate sydney;
    ofxGeo::Coordinate estanciaSanPablo;
    ofxGeo::Coordinate astanda;
   
};
