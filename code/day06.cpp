/* ========================================================================
   $File: day06.cpp $
   $Date: 12/08/24 $
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
    char *Data;
    u32 Width;
    u32 Height;
    u32 StartColumn;
    u32 StartRow;
};

#define MaxThreads 8

#define MapStride 130
#define StartDirection DirectionUp

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
ParseInput(string Input, map& Map)
{
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

        u32 Row = Map.Height;

        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            Map.Data[Row*MapStride + Column] = Line.Data[Column];

            if(Line.Data[Column] == '^')
            {
                Map.StartColumn = Column;
                Map.StartRow = Row;
            }
        }

        Map.Height++;
    }
}

internal void
Traverse(map Map, bool *Visited)
{
    coords Coords = {Map.StartColumn, Map.StartRow};
    direction Direction = StartDirection;

    for(;;)
    {
        Visited[Coords.Y*MapStride + Coords.X] = true;

        coords NextCoords = Coords;

        if(StepAndCheckBounds(Map, NextCoords, Direction))
        {
            break;
        }

        if(Map.Data[NextCoords.Y*MapStride + NextCoords.X] == '#')
        {
            Direction = Rotate(Direction);
        }
        else
        {
            Coords = NextCoords;
        }
    }
}

internal u64
TraverseWithObstacle(map Map, u32 MaxSteps, coords Obstacle)
{
    u64 TotalSteps = 0;

    coords Coords = {Map.StartColumn, Map.StartRow};
    direction Direction = StartDirection;

    while(TotalSteps < MaxSteps)
    {
        coords NextCoords = Coords;

        if(StepAndCheckBounds(Map, NextCoords, Direction))
        {
            break;
        }

        if(Map.Data[NextCoords.Y*MapStride + NextCoords.X] == '#' || NextCoords == Obstacle)
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
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    char *MapData    = (char *) ArenaAlloc(Arena, sizeof(char) * MapStride * MapStride);
    bool *MapVisited = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    map Map = {};
    Map.Data = MapData;

    ParseInput(Input, Map);

    Traverse(Map, MapVisited);

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
PartTwoWorkerFn(u32 ThreadIndex, u32 ThreadCount, map Map, std::atomic_uint64_t *Result)
{
    u32 RowsPerThread = Map.Height / ThreadCount;

    u32 StartRow = ThreadIndex * RowsPerThread;
    u32 EndRow = (ThreadIndex + 1 == ThreadCount) ? Map.Height : (StartRow + RowsPerThread);

    for(u32 Row = StartRow; Row < EndRow; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if((Column == Map.StartColumn && Row == Map.StartRow) || Map.Data[Row*MapStride + Column] != '.')
            {
                continue;
            }

            // The guard is looping if the path returned by the
            // traverse function is at least longer than the total
            // area of the map + 1.

            coords Coords;
            Coords.X = Column;
            Coords.Y = Row;

            u32 MapArea = Map.Width*Map.Height;
            if(TraverseWithObstacle(Map, MapArea + 1, Coords) >= MapArea)
            {
                (*Result)++;
            }
        }
    }
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    std::atomic_uint64_t Result = {};

    char *MapData    = (char *) ArenaAlloc(Arena, sizeof(char) * MapStride * MapStride);
    bool *MapVisited = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    map Map = {};
    Map.Data = MapData;

    ParseInput(Input, Map);

    u32 ThreadCount = MaxThreads;
    if(Map.Width < 50)
    {
        ThreadCount = 1;
    }

    std::thread Threads[MaxThreads];

    for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ThreadIndex++)
    {
        Threads[ThreadIndex] = std::thread(PartTwoWorkerFn, ThreadIndex, ThreadCount, Map, &Result);
    }

    for(u32 ThreadIndex = 0; ThreadIndex < ThreadCount; ThreadIndex++)
    {
        Threads[ThreadIndex].join();
    }

    return(Result);
}

solution Solution06 =
{
    SolvePartOne,
    SolvePartTwo,
};
