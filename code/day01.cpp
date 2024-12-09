/* ========================================================================
   $File: day01.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

internal array<u64> ListOne = {};
internal array<u64> ListTwo = {};

internal void
ParseInput(string Input)
{
    ListOne.Length = 0;
    ListTwo.Length = 0;

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        string StrA = ChopBy(&Line, ' ');
        Line = TrimLeft(Line);
        string StrB = ChopBy(&Line, ' ');

        u64 A = ParseU64(StrA);
        Append(ListOne, A);

        u64 B = ParseU64(StrB);
        Append(ListTwo, B);
    }

    Sort(ListOne);
    Sort(ListTwo);
}

internal u64
SolvePartOne(string Input)
{
    u64 Result = 0;

    for(u32 Index = 0; Index < ListOne.Length; Index++)
    {
        u32 X = ListOne.Data[Index];
        u32 Y = ListTwo.Data[Index];

        if(X > Y)
        {
            Result += X - Y;
        }
        else
        {
            Result += Y - X;
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(string Input)
{
    u64 Result = 0;

    for(u32 Index = 0; Index < ListOne.Length; Index++)
    {
        u32 Count = 0;
        for(u32 OtherIndex = 0; OtherIndex < ListTwo.Length; OtherIndex++)
        {
            if(ListOne.Data[Index] == ListTwo.Data[OtherIndex])
            {
                Count += 1;
            }
        }

        Result += ListOne.Data[Index] * Count;
    }

    return(Result);
}

solution Solution01 =
{
    ParseInput,
    SolvePartOne,
    SolvePartTwo,
};
