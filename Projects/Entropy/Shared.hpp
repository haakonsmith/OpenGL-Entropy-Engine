/*
 * Copyright 2020, Haakon Smith.
 *
 * Shared.hpp
 */

#pragma once

#include <iostream>
#include <memory>

#define NDEBUG
#define PROFILE
#define SUPPRESS

#ifndef COORDINATES
#define COORDINATES

enum Coordinates { x, y, z };

#endif

#include "Profiler.hpp"



#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

#ifdef PROFILE
#define PROFILE_SCOPE(LOCATION) Entropy::Performance::Timer timer(LOCATION)

#else
#define PROFILE_SCOPE() \
    do {                \
    } while (0)
#endif

#ifdef NDEBUG
#define GL_LOG(LOCATION)           \
    if (auto error = glGetError()) \
    std::cout << "OpenGL error " << error << " at " << LOCATION << " " << __LINE__ << std::endl
#else
#define GL_LOG() \
    do {         \
    } while (0)
#endif

#ifdef NDEBUG
#define LOG(MESSAGE) std::cout << MESSAGE << " at line:" << __LINE__ << std::endl
#else
#define LOG() \
    do {      \
    } while (0)
#endif

namespace Entropy {
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using Scope = std::unique_ptr<T>;
}  // namespace Entropy

#include "glm/glm.hpp"