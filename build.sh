#!/bin/bash
g++ -o GTL_NMJW main.cpp renderer.cpp shader.cpp texture.cpp model.cpp loader.cpp controls.cpp postbox.cpp -lepoxy -lGL -lSDL3
./GTL_NMJW
