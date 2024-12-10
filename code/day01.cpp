/* ========================================================================
   $File: day01.cpp $
   $Date: 12/01/24 $
   $Revision: 12/10/24 $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

#define MaxItems 1000

internal void
ParseInput(string Input, u64 *Left, u64 *Right, u32& Size)
{
    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        Assert(Size < MaxItems);

        Left[Size] = ChopU64(&Line);
        Line = TrimLeft(Line);

        Right[Size] = ChopU64(&Line);
        Size++;
    }
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    u64 *Left  = (u64 *) ArenaAlloc(Arena, sizeof(u64) * MaxItems);
    u64 *Right = (u64 *) ArenaAlloc(Arena, sizeof(u64) * MaxItems);
    u32  Size  = 0;

    ParseInput(Input, Left, Right, Size);

    Sort(Left, Size);
    Sort(Right, Size);

    for(u32 Index = 0; Index < Size; Index++)
    {
        u32 X = Left[Index];
        u32 Y = Right[Index];

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
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    u64 *Left  = (u64 *) ArenaAlloc(Arena, sizeof(u64) * MaxItems);
    u64 *Right = (u64 *) ArenaAlloc(Arena, sizeof(u64) * MaxItems);
    u32  Size  = 0;

    ParseInput(Input, Left, Right, Size);

    for(u32 Index = 0; Index < Size; Index++)
    {
        u32 Count = 0;
        for(u32 OtherIndex = 0; OtherIndex < Size; OtherIndex++)
        {
            if(Left[Index] == Right[OtherIndex])
            {
                Count += 1;
            }
        }

        Result += Left[Index] * Count;
    }

    return(Result);
}

solution Solution01 =
{
    SolvePartOne,
    SolvePartTwo,
};
