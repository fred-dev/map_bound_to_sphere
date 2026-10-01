# map_bound_to_sphere

openFrameworks test that renders map tiles (via `ofxMaps`) into an FBO, wraps them onto a sphere and plots lat/long locations on the globe. A prototype for the location view in Synthetic Ornithology (2023).

## Build

- Addons: `ofxMaps` and its dependencies (`ofxHTTP`, `ofxIO`, `ofxMediaType`, `ofxNetworkUtils`, `ofxSSLManager`, `ofxSQLiteCpp`, `ofxTaskQueue`, `ofxCache`, `ofxGeo`, `ofxSpatialHash`, `ofxPoco` or [ofxPocoHeaders](https://github.com/fred-dev/ofxPocoHeaders))
- Generate the project with projectGenerator.
