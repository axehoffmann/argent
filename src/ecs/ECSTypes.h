#pragma once

#include <vector>
#include <cstdint>
#include <array>

// ID structure: AAEEEEEE
typedef uint64_t EntityID;
typedef uint16_t ArchetypeID;
const int EPARTSIZE = 64 - 16;

typedef unsigned char byte;

typedef unsigned char ComponentTypeID;
typedef std::array<ComponentTypeID> ComponentSet;

typedef std::vector<byte> ComponentArray;