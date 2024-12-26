#ifndef constants_h__
#define constants_h__

extern const uint32_t ZONE_GRID;
extern const uint32_t ZONES;

extern const uint32_t MAP_AREA;  // 定义了地图有多大，必须是2的整数次方倍
extern const uint32_t MAP_SIZE;  // MAP_AREA + 1
extern const uint32_t  MAP_HALF; // MAP_AREA / 2
extern const uint32_t MAP_CELLS;
extern const char* MAP_IMAGE; //this is the name of the bitmap to use when generating data
//this is the name of the raw data file where we dump the terrain data 
//so we don't have to generate it every time we run the program.
extern const char* MAP_FILE;
#endif // constants_h__
