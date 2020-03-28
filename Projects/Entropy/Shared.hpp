/*
 * Copyright 2020, Haakon Smith.
 */

#pragma once

#define NDEBUG
#define SUPRESS

#ifndef COORDINATES
#define COORDINATES

enum Coordinates
{
  x,
  y,
  z
};

#endif

#ifdef NDEBUG
#define GL_LOG(LOCATION)         \
  if (auto error = glGetError()) \
  std::cout << "OpenGL error " << error << " at " << LOCATION << " " << __LINE__ << std::endl
#else
#define GL_LOG() \
  do             \
  {              \
  } while (0)
#endif

#ifdef NDEBUG
#define LOG(MESSAGE) std::cout << MESSAGE << " at line:" << __LINE__ << std::endl
#else
#define LOG() \
  do          \
  {           \
  } while (0)
#endif

#include <memory>

namespace Entropy
{
template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scope = std::unique_ptr<T>;
} // namespace Entropy

#include "glm/glm.hpp"