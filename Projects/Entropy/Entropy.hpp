/*
 * Copyright 2020, Haakon Smith.
 */


// CPP Properties
// "macFrameworkPath": [
//     "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/System/Library/Frameworks"
// ],

#include <stdlib.h>
#include <iostream>

#define GL_SILENCE_DEPRECATION

#include "Components/RenderData.hpp"
#include "Components/Components.hpp"
#include "Components/Entity.hpp"

#include "Application.hpp"
#include "Profiler.hpp"
#include "Graphics/2dRenderer.hpp"
#include "Graphics/Renderable.hpp"
#include "Graphics/Shapes/Shape.hpp"
#include "Physics/PhysicsEngine.hpp"