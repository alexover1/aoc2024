#include "AdventOfCode.h"

char SampleData[] =
"....#.....\n"
".........#\n"
"..........\n"
"..#.......\n"
".......#..\n"
"..........\n"
".#..^.....\n"
"........#.\n"
"#.........\n"
"......#...\n";

struct coords
{
    u32 X;
    u32 Y;
};

enum direction
{
    DirectionUp    = 0,
    DirectionRight = 1,
    DirectionDown  = 2,
    DirectionLeft  = 3,
};

char Directions[] = "^>v<";

struct map
{
    char *Data;
    u32 Width;
    u32 Height;
    u32 Stride;
};

#define MapStride 130
char MapData[MapStride*MapStride];

inline char GetMap(map Map, coords Coords)
{
    char Result = Map.Data[Coords.Y*Map.Stride + Coords.X];
    return(Result);
}

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

int main(int ArgCount, char **Args)
{
    string Input = {SampleData, sizeof(SampleData) - 1};

    if(ArgCount > 1)
    {
        char *FileName = Args[1];

        Input = ReadFileData(FileName);
        if(!Input.Data)
        {
            PrintMessage("Error: Unable to read input file.\n");
            return 1;
        }
    }

    map Map = {};
    Map.Data = MapData;
    Map.Stride = MapStride;

    coords Coords = {};
    direction Direction = DirectionUp;

    u32 Row = 0;

    while(Input.Length > 0)
    {
        string Line = TrimLeft(ChopBy(&Input, '\n'));
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
                Coords.X = Column;
                Coords.Y = Row;
            }
        }

        Row++;
    }

    Map.Height = Row;

    while(true)
    {
        Map.Data[Coords.Y*Map.Stride + Coords.X] = 'X';

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

    u32 NumSteps = 0;

    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if(Map.Data[Row*Map.Stride + Column] == 'X')
            {
                NumSteps += 1;
            }
        }
    }

    PrintMessage("Part One: %u\n", NumSteps);

    return(0);
}
