#!/bin/bash

g++ scene_change.cpp -o app `pkg-config --cflags --libs opencv`
