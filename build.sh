#!/bin/bash
g++ -o GTL_NMJW main.cpp renderer.cpp model.cpp loader.cpp controls.cpp postbox.cpp glad/src/glad.c -lGL -lSDL3
./GTL_NMJW
