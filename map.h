#pragma once

#include "basics.h"

#define MAP_NULL (int[20][20]) {\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
}
/// <summary>
/// Structure representing a map with a name and 20x20 grid data.
/// </summary>
/// <remarks>
/// This structure holds the name of the map and a 2D array representing the map layout.
/// </remarks>
typedef struct Map
{
    char* name;
    int data[20][20];
} Map;

/// <summary>
/// Creates and initializes a Map structure with the given name and map data.
/// </summary>
/// <param name="name">The name of the map.</param>
/// <param name="mapData">A 2D array representing the map data.</param>
/// <returns>The newly created Map structure.</returns>
Map CreateMap(char* name, int mapData[20][20]);