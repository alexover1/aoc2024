/* ========================================================================
$File: day12.cpp $
$Date: 12/12/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

#define MapStride 140

struct map
{
    char *Data;
    s32 Width;
    s32 Height;
};

struct point
{
    s32 X;
    s32 Y;
};

inline bool
operator==(point Left, point Right)
{
    return((Left.X == Right.X) && (Left.Y == Right.Y));
}

inline bool
operator!=(point Left, point Right)
{
    return((Left.X != Right.X) || (Left.Y != Right.Y));
}

inline point
operator+(point Left, point Right)
{
    return{Left.X+Right.X, Left.Y+Right.Y};
}

inline point
operator-(point Left, point Right)
{
    return{Left.X-Right.X, Left.Y-Right.Y};
}

internal char
GetMap(map Map, point P)
{
    if(P.X < 0 || P.X == Map.Width || P.Y < 0 || P.Y == Map.Height)
    {
        return('.');
    }
    return(Map.Data[P.Y*MapStride + P.X]);
}

internal map
ParseInput(memory_arena *Arena, string Input)
{
    map Map = {};
    Map.Data = (char *) ArenaAlloc(Arena, sizeof(char) * MapStride * MapStride);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(Line.Length > Map.Width)
        {
            Map.Width = Line.Length;
            Assert(Map.Width <= MapStride);
        }

        s32 Row = Map.Height;
        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            Map.Data[Row*MapStride + Column] = Line.Data[Column];
        }

        Map.Height++;
    }

    return(Map);
}

struct dfs_result
{
    u32 Area;
    u32 Perimeter;
    u32 Sides;
};

internal dfs_result
DFS(bool *Visited, map Map, char Color, s32 X, s32 Y, s32 DX, s32 DY)
{
    s32 I = Y*MapStride + X;

    if(Map.Data[I] != Color)
    {
        point P0 = {X - DY, Y + DX};
        point P1 = {X - DX - DY, Y - DY + DX};
        if(GetMap(Map,P0) == Color || GetMap(Map,P1) != Color)
            return{0,1,1};
        else
            return{0,1,0};
    }

    if(Visited[I])
    {
        return{0,0,0};
    }
    Visited[I] = true;

    dfs_result Result = {};
    Result.Area = 1;

    local_persist point Directions[4] =
    {
        { 1,  0},
        {-1,  0},
        { 0,  1},
        { 0, -1},
    };
    for(u32 Index = 0; Index < ArrayLength(Directions); Index++)
    {
        point D = Directions[Index];
        dfs_result NextResult = DFS(Visited, Map, Color, X+D.X, Y+D.Y, D.X, D.Y);
        Result.Area += NextResult.Area;
        Result.Perimeter += NextResult.Perimeter;
        Result.Sides += NextResult.Sides;
    }

    return(Result);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);

    bool *Visited = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    u64 Result = 0;

    for(s32 Y = 0; Y < Map.Height; Y++)
    {
        for(s32 X = 0; X < Map.Width; X++)
        {
            s32 I = Y*MapStride + X;
            if(!Visited[I])
            {
                dfs_result R = DFS(Visited, Map, Map.Data[I], X, Y, 1, 0);
                Result += R.Area * R.Perimeter;
            }
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);

    bool *Visited = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    u64 Result = 0;

    for(s32 Y = 0; Y < Map.Height; Y++)
    {
        for(s32 X = 0; X < Map.Width; X++)
        {
            s32 I = Y*MapStride + X;
            if(!Visited[I])
            {
                dfs_result R = DFS(Visited, Map, Map.Data[I], X, Y, 1, 0);
                Result += R.Area * R.Sides;
            }
        }
    }

    return(Result);
}

solution Solution12 =
{
    SolvePartOne,
    SolvePartTwo,
};
