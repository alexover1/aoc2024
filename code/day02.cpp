/* ========================================================================
   $File: day02.cpp $
   $Date: 12/02/24 $
   $Revision: 12/10/24 $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

#define MaxLevels 10

internal bool
PairIsSafe(u64 Level, u64 NextLevel)
{
    u64 Difference;
    if(NextLevel > Level)
    {
        Difference = NextLevel - Level;
    }
    else
    {
        Difference = Level - NextLevel;
    }

    bool Result = (Difference > 0 && Difference < 4);

    return(Result);
}

// Check if a line is safe without any skipped values.
internal bool
LineIsSafe(u64 *Levels, u32 LevelCount, bool& Increasing)
{
    bool Result = true;

    const u32 FirstIndex = 1;

    for(u32 Index = FirstIndex; Index < LevelCount; Index++)
    {
        if(!PairIsSafe(Levels[Index-1], Levels[Index]))
        {
            Result = false;
            break;
        }

        bool NextIncreasing = Levels[Index] > Levels[Index-1];

        if(Index > FirstIndex)
        {
            if(Increasing != NextIncreasing)
            {
                Result = false;
                break;
            }
        }
        else
        {
            Increasing = NextIncreasing;
        }
    }

    return(Result);
}

internal bool
CheckLinePartOne(string Line)
{
    string StrLevel = ChopBy(&Line, ' ');
    u64 Level = ParseU64(StrLevel);

    bool Increasing = false;
    bool Decreasing = false;

    while(Line.Length > 0)
    {
        Line = TrimLeft(Line);
        StrLevel = ChopBy(&Line, ' ');

        u64 NextLevel = ParseU64(StrLevel);

        if(!PairIsSafe(Level, NextLevel))
        {
            return false;
        }

        if(NextLevel > Level)
        {
            if(Decreasing) return false;
            Increasing = true;
        }
        else
        {
            if(Increasing) return false;
            Decreasing = true;
        }

        Level = NextLevel;
    }

    return true;
}

internal bool
CheckLinePartTwo(string Line, u64 *Levels)
{
    bool Result = true;

    bool AlreadySkipped = false;
    bool Increasing = false;
    bool SetOrder = false;

    u32 LevelCount = 0;

    Line = TrimSpace(Line);

    while(Line.Length > 0)
    {
        Assert(LevelCount < MaxLevels);

        Levels[LevelCount] = ChopU64(&Line);
        LevelCount++;

        Line = TrimLeft(Line);
    }

    const u32 LineCount = 3;
    Assert(LevelCount >= LineCount);

    for(u32 Index = 0; Index < LevelCount-LineCount; Index++)
    {
        const u32 WindowCount = 4;

        bool NextIncreasing;

        if(LineIsSafe(&Levels[Index], LineCount, NextIncreasing))
        {
            if(SetOrder)
            {
                if(Increasing != NextIncreasing)
                {
                    if(AlreadySkipped)
                    {
                        Result = false;
                        break;
                    }

                    AlreadySkipped = true;
                }
            }
            else
            {
                SetOrder = true;
                Increasing = NextIncreasing;
            }

            continue;
        }

        if(AlreadySkipped)
        {
            Result = false;
            break;
        }

        for(u32 SkipIndex = 0; SkipIndex < WindowCount; SkipIndex++)
        {
            u64 TempLevels[LineCount];
            u32 TempIndex = 0;
            for(u32 J = 0; J < WindowCount; J++)
            {
                if(J == SkipIndex) continue;
                TempLevels[TempIndex++] = Levels[Index+J];
            }

            if(LineIsSafe(TempLevels, LineCount, NextIncreasing))
            {
                // If we already determined ascending or descending,
                // and removing the element causes the remaining
                // elements to have a different order that the rest of
                // the set, the removal of this element is useless.
                if(SetOrder)
                {
                    if(Increasing != NextIncreasing)
                    {
                        continue;
                    }
                }
                else
                {
                    SetOrder = true;
                    Increasing = NextIncreasing;
                }

                // Otherwise, we have found a valid element to skip
                // and can remove it from the set and continue
                // checking the rest of the line.
                for(u32 J = 0; J < LineCount; J++)
                {
                    Levels[Index+J+1] = TempLevels[J];
                }

                AlreadySkipped = true;
                break;
            }
        }

        if(!AlreadySkipped)
        {
            Result = false;
            break;
        }
    }

    bool NextIncreasing;
    if(!LineIsSafe(&Levels[LevelCount-LineCount], LineCount, NextIncreasing))
    {
        if(AlreadySkipped || (SetOrder && Increasing != NextIncreasing))
        {
            Result = false;
        }
    }

    return(Result);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(CheckLinePartOne(Line))
        {
            Result += 1;
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    u64 *Levels = (u64 *) ArenaAlloc(Arena, sizeof(u64) * MaxLevels);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(CheckLinePartTwo(Line, Levels))
        {
            Result += 1;
        }
    }

    return(Result);
}

solution Solution02 =
{
    SolvePartOne,
    SolvePartTwo,
};
