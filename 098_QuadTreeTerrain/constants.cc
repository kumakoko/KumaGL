#include "constants.h"
#include "../klib/kgl_lib_pch.h"


const uint32_t ZONE_GRID = 8;
const uint32_t ZONES = ZONE_GRID * ZONE_GRID;

const uint32_t MAP_AREA = 1024;
const uint32_t MAP_SIZE = MAP_AREA + 1;
const uint32_t MAP_HALF = MAP_AREA / 2;
const uint32_t MAP_CELLS = MAP_SIZE * MAP_SIZE;

const char* MAP_IMAGE = "terrain_map6.bmp";
const char* MAP_FILE = "map.dat";