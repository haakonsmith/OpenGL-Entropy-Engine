#pragma once

#include <OpenGL/gl3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
