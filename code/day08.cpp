/* ========================================================================
   $File: day08.cpp $
   $Date: 12/9/24 $
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
internal bool Antinodes[MapStride*MapStride];
internal array<coords> FreqMap[CHAR_MAX];

internal bool
InBounds(coords Coords, u32 Rows, u32 Columns)
{
    bool Result = (Coords.X >= 0 && Coords.X < Columns) && (Coords.Y >= 0 && Coords.Y < Rows);
    return(Result);
}

internal u64
SolvePartOne(string Input)
{
    u64 Result = 0;

    u32 Rows = 0;
    u32 Columns = 0;

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

    for(u32 Freq = 0; Freq < ArrayLength(FreqMap); Freq++)
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

                    if(InBounds(SiteA, Rows, Columns))
                    {
                        Antinodes[SiteA.Y*MapStride + SiteA.X] = true;
                    }

                    coords SiteB = Antennas.Data[J] - Displacement;

                    if(InBounds(SiteB, Rows, Columns))
                    {
                        Antinodes[SiteB.Y*MapStride + SiteB.X] = true;
                    }
                }
            }
        }
    }

    for(u32 Row = 0; Row < Rows; Row++)
    {
        for(u32 Column = 0; Column < Columns; Column++)
        {
            if(Antinodes[Row*MapStride + Column])
            {
                Result += 1;
            }
        }
    }

    for(u32 Index = 0; Index < ArrayLength(FreqMap); Index++)
    {
        FreqMap[Index].Length = 0;
    }

    FillMemory(sizeof(Antinodes), Antinodes, 0);

    return(Result);
}

internal u64
SolvePartTwo(string Input)
{
    u64 Result = 0;

    return(Result);
}

solution Solution08 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
