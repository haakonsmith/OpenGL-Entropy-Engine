#define CUTE_C2_IMPLEMENTATION
#include <game/cute_c2.h>



// include enumurator for shapes: main, factory and GameObject use this
#include <game/shapes/ShapesEnumuration.hpp>


#include <game/enums.hpp>

// #include "../Box2D/box2d.h"

#include <game/Collison.hpp>

#include <game/PolyFunc.hpp>

#include <game/shapes/ShapeBase.hpp>



// include shapes all use these and shape base is requrired
#include <game/shapes/Triangle.hpp>
#include <game/shapes/RightTriangle.hpp>
#include <game/shapes/Obj.hpp>

// include factory GameObject uses this
#include <game/Factory/Factory.hpp>

// include GameObject: player and main uses this
#include <game/GameObject.hpp>
#include <game/Room.hpp>

#include <game/Bullet.hpp>

// include player: main uses this
#include <game/Player.hpp>
