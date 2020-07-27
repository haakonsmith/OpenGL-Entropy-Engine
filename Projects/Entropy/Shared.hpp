/*
 * Copyright 2020, Haakon Smith.
 *
 * Shared.hpp
 */

#pragma once

#include <iostream>
#include <memory>

#ifndef COORDINATES
    #define COORDINATES

enum Coordinates { x, y, z };

#endif

#include "Profiler.hpp"

#ifndef ENTROPY_MACROS
    #define ENTROPY_MACROS

    #define PROFILE_CALL(CPP_CALL)    \
        {                             \
            PROFILE_SCOPE(#CPP_CALL); \
            CPP_CALL;                 \
        }

    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

    #ifdef PROFILE
        #define PROFILE_SCOPE(NAME) Entropy::Performance::Timer entropy_performance_timer(NAME)

    #else
        #define PROFILE_SCOPE(NAME) \
            do {                    \
            } while (0)
    #endif

    #ifdef NDEBUG
        #define GL_LOG(LOCATION)           \
            if (auto error = glGetError()) \
            std::cout << "OpenGL error " << error << " at " << LOCATION << " " << __LINE__ << std::endl
    #else
        #define GL_LOG(LOCATION) \
            do {                 \
            } while (0)
    #endif

    #ifdef NDEBUG
        #define LOG(MESSAGE) std::cout << MESSAGE << " at line:" << __LINE__ << std::endl
    #else
        #define LOG(MESSAGE) \
            do {             \
            } while (0)
    #endif
#endif

namespace Entropy {
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using Scope = std::unique_ptr<T>;
}  // namespace Entropy

#include "glm/glm.hpp"