/* ========================================================================
   $File: day06.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

struct coords
{
    s32 X;
    s32 Y;
};

inline s32 DotProduct(coords U, coords V)
{
    s32 X = U.X * V.X;
    s32 Y = U.Y * V.Y;
    s32 Result = X + Y;

    return(Result);
}

enum direction
{
    DirectionUp    = 0,
    DirectionRight = 1,
    DirectionDown  = 2,
    DirectionLeft  = 3,
};

struct map
{
    char *Data;
    u32 Width;
    u32 Height;
    u32 Stride;
};

internal direction
Rotate(direction Direction)
{
    int Value = (int)Direction;
    direction Result = (direction) ((Value + 1) % 4);

    return(Result);
}

internal bool
StepAndCheckBounds(map Map, coords& Coords, direction Direction)
{
    bool Result = false;

    switch(Direction)
    {
        case DirectionUp:
        {
            if(Coords.Y == 0)
            {
                Result = true;
            }
            else
            {
                Coords.Y -= 1;
            }
        } break;

        case DirectionRight:
        {
            if(Coords.X == Map.Width-1)
            {
                Result = true;
            }
            else
            {
                Coords.X += 1;
            }
        } break;

        case DirectionDown:
        {
            if(Coords.Y == Map.Height-1)
            {
                Result = true;
            }
            else
            {
                Coords.Y += 1;
            }
        } break;

        case DirectionLeft:
        {
            if(Coords.X == 0)
            {
                Result = true;
            }
            else
            {
                Coords.X -= 1;
            }
        } break;
    }

    return(Result);
}

internal map Map = {};
internal coords StartCoords = {};
#define StartDirection DirectionUp

#define MapStride 130
internal char MapData[MapStride*MapStride];
internal bool Visited[MapStride*MapStride];

internal void
ParseInput(string Input)
{
    Map.Data = MapData;
    Map.Stride = MapStride;

    u32 Row = 0;
    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(Line.Length > Map.Width)
        {
            Map.Width = Line.Length;
            Assert(Map.Width <= Map.Stride);
        }

        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            Map.Data[Row*Map.Stride + Column] = Line.Data[Column];

            if(Line.Data[Column] == '^')
            {
                StartCoords.X = Column;
                StartCoords.Y = Row;
            }
        }

        Row++;
    }

    Map.Height = Row;
}

internal u64
TraverseMap(map Map, u32 MaxSteps)
{
    u64 TotalSteps = 0;

    coords Coords = StartCoords;
    direction Direction = StartDirection;

    FillMemory(sizeof(Visited), Visited, 0);

    while(TotalSteps < MaxSteps)
    {
        Visited[Coords.Y*Map.Stride + Coords.X] = true;

        coords NextCoords = Coords;

        if(StepAndCheckBounds(Map, NextCoords, Direction))
        {
            break;
        }

        if(Map.Data[NextCoords.Y*Map.Stride + NextCoords.X] == '#')
        {
            Direction = Rotate(Direction);
        }
        else
        {
            Coords = NextCoords;
            TotalSteps += 1;
        }
    }

    return(TotalSteps);
}

internal u64
SolvePartOne(string Input)
{
    u64 Result = 0;

    TraverseMap(Map, UINT_MAX);

    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if(Visited[Row*Map.Stride + Column])
            {
                Result += 1;
            }
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(string Input)
{
    u64 Result = 0;

    local_persist char TempData[MapStride*MapStride];
    CopyMemory(sizeof(TempData), TempData, MapData);

    map TempMap = Map;
    TempMap.Data = TempData;

    for(u32 Row = 0; Row < TempMap.Height; Row++)
    {
        for(u32 Column = 0; Column < TempMap.Width; Column++)
        {
            if((Column == StartCoords.X && Row == StartCoords.Y) || MapData[Row*Map.Stride + Column] != '.')
            {
                continue;
            }

            char Saved = TempData[Row*Map.Stride + Column];
            TempData[Row*Map.Stride + Column] = '#';

            // The guard is looping if the path returned by the
            // traverse function is at least longer than the total
            // area of the map + 1.

            u32 MapArea = TempMap.Width*TempMap.Height;
            if(TraverseMap(TempMap, MapArea + 1) >= MapArea)
            {
                Result += 1;
            }

            TempData[Row*TempMap.Stride + Column] = Saved;
        }
    }

    return(Result);
}

solution Solution06 =
{
    ParseInput,
    SolvePartOne,
    SolvePartTwo,
};
