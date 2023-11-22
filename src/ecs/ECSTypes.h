#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>
#include "lib/basic.h"

// ID structure: AAEEEEEE
typedef uint64_t EntityID;
typedef uint16_t ArchetypeID;
const int EPARTSIZE = 64 - 16;

typedef unsigned char ComponentTypeID;
typedef std::vector<ComponentTypeID> ComponentSet;

typedef std::vector<ag::byte> ComponentArray;