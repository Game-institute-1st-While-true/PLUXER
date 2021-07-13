#pragma once

#if defined(_DEBUG)||defined(DEBUG)
#if _WIN64
#pragma comment(lib,"Recast_x64D")
#pragma comment(lib,"Detour_x64D")
#pragma comment(lib,"DebugUtils_x64D")
#pragma comment(lib,"DetourCrowd_x64D")
#pragma comment(lib,"DetourTileCache_x64D")
#else
#pragma comment(lib,"Recast_x86D")
#pragma comment(lib,"Detour_x86D")
#pragma comment(lib,"DebugUtils_x86D")
#pragma comment(lib,"DetourCrowd_x86D")
#pragma comment(lib,"DetourTileCache_x86D")
#endif // _WIN64
#else
#if _WIN64
#pragma comment(lib,"Recast_x64R")
#pragma comment(lib,"Detour_x64R")
#pragma comment(lib,"DebugUtils_x64R")
#pragma comment(lib,"DetourCrowd_x64R")
#pragma comment(lib,"DetourTileCache_x64R")
#else
#pragma comment(lib,"Recast_x86R")
#pragma comment(lib,"Detour_x86R")
#pragma comment(lib,"DebugUtils_x86R")
#pragma comment(lib,"DetourCrowd_x86R")
#pragma comment(lib,"DetourTileCache_x86R")
#endif // _WIN64
#endif // defined(_DEBUG)||defined(DEBUG)


#include "PerfTimer.h"
#include "Recast\Recast.h"
#include "Detour\DetourAlloc.h"
#include "Detour\DetourAssert.h"
#include "Detour\DetourNavMesh.h"
#include "Detour\DetourNavMeshQuery.h"
#include "Detour\DetourNavMeshBuilder.h"
#include "Detour\DetourCommon.h"

namespace hos::nv
{
constexpr float DEFAULT_CELL_SIZE = 0.3f;
constexpr float DEFAULT_CELL_HEIGHT = 0.2f;

constexpr float DEFAULT_AGENT_HEIGHT = 2.0f;
constexpr float DEFAULT_AGENT_RAIDUS = 0.6f;
constexpr float DEFAULT_AGENT_MAX_CLIMB = 0.9f;
constexpr float DEFAULT_AGENT_MAX_SLOPE = 45.0f;

constexpr float DEFAULT_REGION_MIN_SIZE = 8.f;
constexpr float DEFAULT_REGION_MERGE_SIZE = 20.f;

constexpr float DEFAULT_EDGE_MAX_LEN = 12.f;
constexpr float DEFAULT_EDGE_MAX_ERROR = 1.3f;

constexpr float DEFAULT_VERTSPERPOLY = 6.f;

constexpr float DEFAULT_DETAIL_SAMPLE_DIST = 6.f;
constexpr float DEFAULT_DETAIL_SAMPLE_MAX_ERROR = 1.f;

constexpr int MAX_POLYS = 256;

constexpr int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
constexpr int NAVMESHSET_VERSION = 1;

enum PolyAreas
{
	POLYAREA_GROUND,
	POLYAREA_WATER,
	POLYAREA_ROAD,
	POLYAREA_DOOR,
	POLYAREA_GRASS,
	POLYAREA_JUMP,
};
enum PolyFlags
{
	POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
	POLYFLAGS_ALL = 0xffff		// All abilities.
};}