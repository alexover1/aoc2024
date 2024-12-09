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

internal char Directions[] = "^>v<";

struct map
{
    char *Data;
    u32 Width;
    u32 Height;
    u32 Stride;
};

inline direction Rotate(direction Direction)
{
    int Value = (int)Direction;
    direction Result = (direction) ((Value + 1) % 4);

    return(Result);
}

inline bool StepAndCheckBounds(map Map, coords& Coords, direction Direction)
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
SolvePartOne(string Input)
{
    u64 Result = 0;

    coords Coords = StartCoords;
    direction Direction = DirectionUp;

    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            Visited[Row*Map.Stride + Column] = false;
        }
    }

    while(true)
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
        }
    }

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

    coords Coords = StartCoords;
    direction Direction = DirectionUp;

    local_persist array<coords> Turns = {};
    Turns.Length = 0;

    while(true)
    {
        coords NextCoords = Coords;

        if(StepAndCheckBounds(Map, NextCoords, Direction))
        {
            break;
        }

        if(Map.Data[NextCoords.Y*Map.Stride + NextCoords.X] == '#')
        {
            Direction = Rotate(Direction);
            Append(Turns, Coords);
        }
        else
        {
            Coords = NextCoords;
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
