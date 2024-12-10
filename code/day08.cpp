/* ========================================================================
   $File: day08.cpp $
   $Date: 12/09/24 $
   $Revision: 12/10/24 $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

#define MapStride 50
#define MaxFrequency CHAR_MAX

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

struct antenna_list
{
    coords Data[MapStride*MapStride];
    u32 Length;
};

struct frequency_map
{
    char Frequency;
    antenna_list *Antennas; // By pointer so if we never use the frequency we use less memory.
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

internal void
Append(antenna_list *Antennas, coords Coords)
{
    Assert(Antennas->Length < MapStride*MapStride);
    Antennas->Data[Antennas->Length] = Coords;
    Antennas->Length += 1;
}

internal void
ParseInput(memory_arena *Arena, string Input, map& Map, array<frequency_map>& FrequencyMaps)
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
            if(IsAlnum(Line.Data[Column]))
            {
                char Frequency = Line.Data[Column];

                coords Coords;
                Coords.X = Column;
                Coords.Y = Row;

                bool Found = false;

                for(u32 MapIndex = 0; MapIndex < FrequencyMaps.Length; MapIndex++)
                {
                    if(FrequencyMaps.Data[MapIndex].Frequency == Frequency)
                    {
                        Append(FrequencyMaps.Data[MapIndex].Antennas, Coords);
                        Found = true;
                        break;
                    }
                }

                if(!Found)
                {
                    antenna_list *Antennas = (antenna_list *) ArenaAlloc(Arena, sizeof(antenna_list));
                    Append(Antennas, Coords);

                    frequency_map FrequencyMap = {};
                    FrequencyMap.Frequency = Frequency;
                    FrequencyMap.Antennas = Antennas;

                    Append(FrequencyMaps, FrequencyMap);
                }
            }
        }

        Map.Height++;
    }
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

    // Maps a frequency to a list of its coordinates.
    array<frequency_map> FrequencyMaps = {};
    map Map = {};

    bool *Antinodes = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    ParseInput(Arena, Input, Map, FrequencyMaps);

    for(u32 MapIndex = 0; MapIndex < FrequencyMaps.Length; MapIndex++)
    {
        frequency_map& FrequencyMap = FrequencyMaps.Data[MapIndex];
        antenna_list *Antennas = FrequencyMap.Antennas;

        for(u32 I = 0; I < Antennas->Length; I++)
        {
            for(u32 J = 0; J < Antennas->Length; J++)
            {
                if(I != J)
                {
                    coords Displacement = Antennas->Data[I] - Antennas->Data[J];

                    coords SiteA = Antennas->Data[I] + Displacement;

                    if(InBounds(Map, SiteA))
                    {
                        Antinodes[SiteA.Y*MapStride + SiteA.X] = true;
                    }

                    coords SiteB = Antennas->Data[J] - Displacement;

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

    // TODO: Use the Arena for allocation!
    delete[] FrequencyMaps.Data;

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    // Maps a frequency to a list of its coordinates.
    array<frequency_map> FrequencyMaps = {};
    map Map = {};

    bool *Antinodes = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    ParseInput(Arena, Input, Map, FrequencyMaps);

    for(u32 MapIndex = 0; MapIndex < FrequencyMaps.Length; MapIndex++)
    {
        frequency_map& FrequencyMap = FrequencyMaps.Data[MapIndex];
        antenna_list *Antennas = FrequencyMap.Antennas;

        for(u32 I = 0; I < Antennas->Length; I++)
        {
            for(u32 J = 0; J < Antennas->Length; J++)
            {
                if(I != J)
                {
                    coords Displacement = Antennas->Data[I] - Antennas->Data[J];

                    coords SiteA = Antennas->Data[I];

                    while(InBounds(Map, SiteA))
                    {
                        Antinodes[SiteA.Y*MapStride + SiteA.X] = true;
                        SiteA = SiteA + Displacement;
                    }

                    coords SiteB = Antennas->Data[J];

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
    SolvePartOne,
    SolvePartTwo,
};
