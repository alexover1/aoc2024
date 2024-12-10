/* ========================================================================
   $File: day08.cpp $
   $Date: 12/09/24 $
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

struct map
{
    u32 Width;
    u32 Height;
};

inline coords
operator+(coords Left, coords Right)
{
    coords Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;

    return(Result);
}

inline coords
operator-(coords Left, coords Right)
{
    coords Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;

    return(Result);
}

#define MapStride 50
#define MaxFrequency CHAR_MAX
// internal bool Antinodes[MapStride*MapStride];
// internal array<coords> FreqMap[CHAR_MAX];
// internal map Map = {};

internal void
ParseInput(string Input, map& Map, array<coords> *FreqMap)
{
    u32 Rows = 0;
    u32 Columns = 0;

    for(u32 Index = 0; Index < MaxFrequency; Index++)
    {
        FreqMap[Index].Length = 0;
    }

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(Line.Length > Columns)
        {
            Columns = Line.Length;
        }

        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            char C = Line.Data[Column];

            coords Coords;
            Coords.X = Column;
            Coords.Y = Rows;

            if(IsDigit(C) || IsAlpha(C))
            {
                Append(FreqMap[C], Coords);
            }
        }

        Rows++;
    }

    Map.Width = Columns;
    Map.Height = Rows;
}

internal bool
InBounds(map Map, coords Coords)
{
    bool Result = (Coords.X >= 0 && Coords.X < Map.Width) && (Coords.Y >= 0 && Coords.Y < Map.Height);
    return(Result);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    bool *Antinodes = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);
    array<coords> *FreqMap = (array<coords> *) ArenaAlloc(Arena, sizeof(array<coords>) * MaxFrequency); // @Leak
    map Map = {};

    ParseInput(Input, Map, FreqMap);

    for(u32 Freq = 0; Freq < MaxFrequency; Freq++)
    {
        array<coords>& Antennas = FreqMap[Freq];

        for(u32 I = 0; I < Antennas.Length; I++)
        {
            for(u32 J = 0; J < Antennas.Length; J++)
            {
                if(I != J)
                {
                    coords Displacement = Antennas.Data[I] - Antennas.Data[J];

                    coords SiteA = Antennas.Data[I] + Displacement;

                    if(InBounds(Map, SiteA))
                    {
                        Antinodes[SiteA.Y*MapStride + SiteA.X] = true;
                    }

                    coords SiteB = Antennas.Data[J] - Displacement;

                    if(InBounds(Map, SiteB))
                    {
                        Antinodes[SiteB.Y*MapStride + SiteB.X] = true;
                    }
                }
            }
        }
    }

    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if(Antinodes[Row*MapStride + Column])
            {
                Result += 1;
            }
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    bool *Antinodes = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);
    array<coords> *FreqMap = (array<coords> *) ArenaAlloc(Arena, sizeof(array<coords>) * MaxFrequency); // @Leak
    map Map = {};

    ParseInput(Input, Map, FreqMap);

    for(u32 Freq = 0; Freq < MaxFrequency; Freq++)
    {
        array<coords>& Antennas = FreqMap[Freq];

        for(u32 I = 0; I < Antennas.Length; I++)
        {
            for(u32 J = 0; J < Antennas.Length; J++)
            {
                if(I != J)
                {
                    coords Displacement = Antennas.Data[I] - Antennas.Data[J];

                    coords SiteA = Antennas.Data[I];

                    while(InBounds(Map, SiteA))
                    {
                        Antinodes[SiteA.Y*MapStride + SiteA.X] = true;
                        SiteA = SiteA + Displacement;
                    }

                    coords SiteB = Antennas.Data[J];

                    while(InBounds(Map, SiteB))
                    {
                        Antinodes[SiteB.Y*MapStride + SiteB.X] = true;
                        SiteB = SiteB - Displacement;
                    }
                }
            }
        }
    }

    for(u32 Row = 0; Row < Map.Height; Row++)
    {
        for(u32 Column = 0; Column < Map.Width; Column++)
        {
            if(Antinodes[Row*MapStride + Column])
            {
                Result += 1;
            }
        }
    }

    return(Result);
}

solution Solution08 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
