/* ========================================================================
   $File: day01.cpp $
   $Date: 12/08/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

internal void
ParseInput(string Input, array<u64>& ListOne, array<u64>& ListTwo)
{
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
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<u64> ListOne = {};
    array<u64> ListTwo = {};

    ParseInput(Input, ListOne, ListTwo);

    Sort(ListOne);
    Sort(ListTwo);

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

    // TODO: Use the Arena for allocation!
    delete[] ListOne.Data;
    delete[] ListTwo.Data;

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<u64> ListOne = {};
    array<u64> ListTwo = {};

    ParseInput(Input, ListOne, ListTwo);

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

    // TODO: Use the Arena for allocation!
    delete[] ListOne.Data;
    delete[] ListTwo.Data;

    return(Result);
}

solution Solution01 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
