
#include "ofApp.h"


void ofApp::setup()
{
    ofDisableArbTex();
 
    ofJson json = ofLoadJson("provider.json");

	tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(json));
    Poco::ThreadPool::defaultPool().addCapacity(64);
    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "cache/");
    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);

    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, 1 * 1920, 1 * 1080);

    chicago = ofxGeo::Coordinate(41.8827, -87.6233);
    sydney = ofxGeo::Coordinate(-33.856958, 151.210337);
    estanciaSanPablo = ofxGeo::Coordinate(-54.31188628368298, -66.8373083046737);
    astanda = ofxGeo::Coordinate(51.16818999537737, 71.42305816258578);
    
    tileLayer->setCenter(chicago, 6);
    
    earthTex.load("earth.jpg");
    
    globe.setRadius(globeRadius);
    globe.setResolution(1000);
    globe.mapTexCoordsFromTexture(earthTex.getTexture());
    
    mapFbo.allocate(1920, 1080);
  
}


void ofApp::update()

{
    tileLayer->update();

    //just draw the map into the FBO so we can use the texture easily
    mapFbo.begin();
    ofClear(0,0);
    tileLayer->draw(0, 0, 1920, 1080);
    mapFbo.end();


}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor(0), ofColor(0));
    
    ofxGeo::Coordinate mapTileTopLeftGPS, mapTileTopRightGPS, mapTileBottomLeftGPS, mapTileBottomRightGPS;
    
    mapTileTopLeftGPS = tileLayer->pixelsToGeo(ofVec2f(0, 0));
    mapTileTopRightGPS = tileLayer->pixelsToGeo(ofVec2f(tileLayer->getWidth(), 0));
    mapTileBottomLeftGPS = tileLayer->pixelsToGeo(ofVec2f(0, tileLayer->getHeight()));
    mapTileBottomRightGPS = tileLayer->pixelsToGeo(ofVec2f(tileLayer->getWidth(), tileLayer->getHeight()));
    
    //extract the latitude and longditude from the coordinates
    double latTopLeft = ofDegToRad(mapTileTopLeftGPS.getLatitude());
    double lonTopLeft = ofDegToRad(mapTileTopLeftGPS.getLongitude());

    double latTopRight = ofDegToRad(mapTileTopRightGPS.getLatitude());
    double lonTopRight = ofDegToRad(mapTileTopRightGPS.getLongitude());

    double latBottomLeft = ofDegToRad(mapTileBottomLeftGPS.getLatitude());
    double lonBottomLeft = ofDegToRad(mapTileBottomLeftGPS.getLongitude());

    double latBottomRight = ofDegToRad(mapTileBottomRightGPS.getLatitude());
    double lonBottomRight = ofDegToRad(mapTileBottomRightGPS.getLongitude());

    //this is the total angle covered by the latitude and longditude
    float totalLatAngle = (abs(latTopLeft - latBottomLeft));
    float totalLonAngle = (abs(lonTopRight - lonTopLeft)) ;

    //Ill start with a new mesh becuase I am messy
    ofMesh mesh;

    // Define the number of rows and columns - this is just the resolution of the mesh
    int numRows = 100;
    int numCols = 100;

    // Calculate the angular step size for latitude and longitude
    float latStep = totalLatAngle / (numRows - 1);
    float lonStep = totalLonAngle / (numCols - 1);

    // Loop to create the vertices and texture coordinates
    for (int i = 0; i < numRows; i++) {
        // Calculate the latitude for this row
        float lat = latTopLeft - i * latStep;

        for (int j = 0; j < numCols; j++) {
            // Calculate the longitude for this column
            float lon = lonTopLeft + j * lonStep;

            // Convert spherical coordinates to Cartesian coordinates
            //I use a slightly larger globe radius so the mesh is easy to see and not exaclty the same as the globe
            float x =  globeRadius * 1.001 * sin(lat) * cos(lon);
            float y =  globeRadius * 1.001 * sin(lat) * sin(lon);
            float z =  globeRadius * 1.001 * cos(lat);

            // Add the vertex to the mesh
            mesh.addVertex(ofVec3f(x, y, z));

            // Calculate texture coordinates
            float u = static_cast<float>(j) / (numCols - 1);
            float v = static_cast<float>(i) / (numRows - 1);

            // Add texture coordinates to the mesh
            mesh.addTexCoord(ofVec2f(u, v));
        }
    }


    // Loop to create the indices for the triangles
    for (int i = 0; i < numRows - 1; i++) {
        for (int j = 0; j < numCols - 1; j++) {
            // Calculate the indices for the four vertices of the current quad
            int index1 = i * numCols + j;
            int index2 = i * numCols + j + 1;
            int index3 = (i + 1) * numCols + j;
            int index4 = (i + 1) * numCols + j + 1;

            // Create two triangles for the quad
            mesh.addTriangle(index1, index2, index3);
            mesh.addTriangle(index2, index4, index3);
        }
    }

    //draw the map small so we can see what is going on
    mapFbo.draw(0, 0, 192, 108);
    
    cam.begin();
    ofEnableDepthTest();
    
    //this is jsut the earth with its standard texture
    earthTex.bind();
    globe.draw();
    earthTex.unbind();
    
    //now the mesh with the map texture bound to it
    mapFbo.getTexture().bind();
    mesh.draw();
    mapFbo.getTexture().unbind();
    
    ofDisableDepthTest();
    cam.end();
   
    

}


void ofApp::keyPressed(int key)
{
//we can move aorund the world a bit to see what is wrong
    if(key == '1'){
        tileLayer->setCenter(chicago, 6);

    }
    else if (key == '2')
    {
        tileLayer->setCenter(sydney, 6);
    }
    else if (key == '3')
    {
        tileLayer->setCenter(estanciaSanPablo, 6);
    }
    else if (key == '4')
    {
        tileLayer->setCenter(astanda, 6);
    }
    else if (key == '-')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(-1));
    }
    else if (key == '=')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(1));
    }
   

}

