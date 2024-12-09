/* ========================================================================
   $File: day07.cpp $
   $Date: 12/9/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

internal u64
SolvePartOne(string Input)
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
SolvePartTwo(string Input)
{
    u64 Result = 0;
    return(Result);
}

solution Solution07 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
