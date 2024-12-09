/* ========================================================================
   $File: day06.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include <thread>

#include "aoc.h"

struct coords
{
    u32 X;
    u32 Y;
};

inline bool
operator==(coords Left, coords Right)
{
    bool Result = (Left.X == Right.X) && (Left.Y == Right.Y);
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
    u32 Width;
    u32 Height;
};

#define MaxThreads 8

internal map Map = {};
internal coords StartCoords = {};
#define StartDirection DirectionUp

#define MapStride 130
internal char MapData[MapStride*MapStride];
thread_local bool MapVisited[MapStride*MapStride];

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

internal void
ParseInput(string Input)
{
    u32 Row = 0;
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

        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            MapData[Row*MapStride + Column] = Line.Data[Column];

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
TraverseMap(map Map, u32 MaxSteps, coords *Obstacle = NULL)
{
    u64 TotalSteps = 0;

    coords Coords = StartCoords;
    direction Direction = StartDirection;

    FillMemory(sizeof(MapVisited), MapVisited, 0);

    while(TotalSteps < MaxSteps)
    {
        MapVisited[Coords.Y*MapStride + Coords.X] = true;

        coords NextCoords = Coords;

        if(StepAndCheckBounds(Map, NextCoords, Direction))
        {
            break;
        }

        if(MapData[NextCoords.Y*MapStride + NextCoords.X] == '#' || (Obstacle && *Obstacle == NextCoords))
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
            if(MapVisited[Row*MapStride + Column])
            {
                Result += 1;
            }
        }
    }

    return(Result);
}

internal void
PartTwoWorkerFn(u32 ThreadIndex, u32 ThreadCount, std::atomic_uint64_t *Result)
{
    u32 RowsPerThread = Map.Height / ThreadCount;

    u32 StartRow = ThreadIndex * RowsPerThread;
    u32 EndRow = (ThreadIndex + 1 == ThreadCount) ? Map.Height : (StartRow + RowsPerThread);

    for(u32 Row = StartRow; Row < EndRow; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            coords Coords;
            Coords.X = Column;
            Coords.Y = Row;

            if(Coords == StartCoords || MapData[Row*MapStride + Column] != '.')
            {
                continue;
            }

            // The guard is looping if the path returned by the
            // traverse function is at least longer than the total
            // area of the map + 1.

            u32 MapArea = Map.Width*Map.Height;
            if(TraverseMap(Map, MapArea + 1, &Coords) >= MapArea)
            {
                (*Result)++;
            }
        }
    }
}

internal u64
SolvePartTwo(string Input)
{
    std::atomic_uint64_t Result = {};

    u32 ThreadCount = MaxThreads;
    if(Map.Width < 50)
    {
        ThreadCount = 1;
    }

    std::thread Threads[MaxThreads];

    for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ThreadIndex++)
    {
        Threads[ThreadIndex] = std::thread(PartTwoWorkerFn, ThreadIndex, ThreadCount, &Result);
    }

    for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ThreadIndex++)
    {
        Threads[ThreadIndex].join();
    }

    return(Result);
}

solution Solution06 =
{
    ParseInput,
    SolvePartOne,
    SolvePartTwo,
};
