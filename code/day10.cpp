/* ========================================================================
$File: day10.cpp $
$Date: 12/10/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

#define MapStride 40

struct map
{
    char *Data;
    bool *Visited;
    u32 Width;
    u32 Height;
};

internal map
ParseInput(memory_arena *Arena, string Input)
{
    map Map = {};
    Map.Data = (char *) ArenaAlloc(Arena, sizeof(char) * MapStride * MapStride);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);

        if(Line.Length > Map.Width)
        {
            Map.Width = Line.Length;
        }

        u32 Row = Map.Height;
        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            Map.Data[Row*MapStride + Column] = Line.Data[Column];
        }

        Map.Height++;
    }

    return(Map);
}

internal u64
CheckPaths(map Map, u32 Row, u32 Column)
{
    if(Map.Visited)
    {
        if(Map.Visited[Row*MapStride + Column]) return(0);
        Map.Visited[Row*MapStride + Column] = true;
    }

    char Depth = Map.Data[Row*MapStride + Column];
    if(Depth == '9') return(1);

    u64 Result = 0;

    if(Row > 0 && Map.Data[(Row-1)*MapStride + Column] == Depth+1)
    {
        Result += CheckPaths(Map, Row-1, Column);
    }
    if(Row + 1 < Map.Height && Map.Data[(Row+1)*MapStride + Column] == Depth+1)
    {
        Result += CheckPaths(Map, Row+1, Column);
    }
    if(Column > 0 && Map.Data[Row*MapStride + (Column-1)] == Depth+1)
    {
        Result += CheckPaths(Map, Row, Column-1);
    }
    if(Column + 1 < Map.Width && Map.Data[Row*MapStride + (Column+1)] == Depth+1)
    {
        Result += CheckPaths(Map, Row, Column+1);
    }

    return(Result);
}

internal u64
CheckPaths(map Map)
{
    u64 Result = 0;
    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if(Map.Data[Row*MapStride + Column] == '0')
            {
                Result += CheckPaths(Map, Row, Column);

                if(Map.Visited)
                {
                    memset(Map.Visited, 0, sizeof(bool) * MapStride * MapStride);
                }
            }
        }
    }
    return(Result);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);
    Map.Visited = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    u64 Result = CheckPaths(Map);

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);

    u64 Result = CheckPaths(Map);

    return(Result);
}

solution Solution10 =
{
    SolvePartOne,
    SolvePartTwo,
};
