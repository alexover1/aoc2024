/* ========================================================================
   $File: day07.cpp $
   $Date: 12/09/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<u64> Values = {};

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        string TestValueString = ChopBy(&Line, ':');
        u64 TestValue = ParseU64(TestValueString);

        while(Line.Length > 0)
        {
            Line = TrimSpace(Line);
            u64 Value = ChopU64(&Line);
            Append(Values, Value);
        }

        u32 Combinations = 1 << (Values.Length-1);

        for(u32 Index = 0; Index < Combinations; Index++)
        {
            u64 RunningTotal = Values.Data[0];

            for(u32 ValueIndex = 1; ValueIndex < Values.Length; ValueIndex++)
            {
                if(Index & (1<<(ValueIndex-1)))
                {
                    RunningTotal *= Values.Data[ValueIndex];
                }
                else
                {
                    RunningTotal += Values.Data[ValueIndex];
                }
            }

            if(RunningTotal == TestValue)
            {
                Result += TestValue;
                break;
            }
        }

        Values.Length = 0;
    }

    return(Result);
}

internal u64
ConcatenateIntegers(u64 A, u64 B) {
    u64 NumDigitsB = (B == 0) ? 1 : (u64)log10(B) + 1;

    // Shift `a` to the left and add `b`.
    u64 Result = A * (u64)pow(10, NumDigitsB) + B;

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<u64> Values = {};

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        string TestValueString = ChopBy(&Line, ':');
        u64 TestValue = ParseU64(TestValueString);

        while(Line.Length > 0)
        {
            Line = TrimSpace(Line);
            u64 Value = ChopU64(&Line);
            Append(Values, Value);
        }

        if(Values.Length == 0)
        {
            continue;
        }

        u32 Combinations = (u32)pow(3, Values.Length-1);

        for(u32 Index = 0; Index < Combinations; Index++)
        {
            u64 RunningTotal = Values.Data[0];
            u32 TempIndex = Index;

            for(u32 ValueIndex = 1; ValueIndex < Values.Length; ValueIndex++)
            {
                u32 Operation = TempIndex%3;
                TempIndex /= 3;

                if(Operation == 0)
                {
                    RunningTotal += Values.Data[ValueIndex];
                }
                else if(Operation == 1)
                {
                    RunningTotal *= Values.Data[ValueIndex];
                }
                else
                {
                    RunningTotal = ConcatenateIntegers(RunningTotal, Values.Data[ValueIndex]);
                }

                if(RunningTotal > TestValue)
                {
                    break;
                }
            }

            if(RunningTotal == TestValue)
            {
                Result += TestValue;
                break;
            }
        }

        Values.Length = 0;
    }

    return(Result);
}

solution Solution07 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
