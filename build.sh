#!/bin/bash
g++ -o GTL_NMJW main.cpp renderer.cpp model.cpp loader.cpp controls.cpp postbox.cpp -lepoxy -lGL -lSDL3
./GTL_NMJW
