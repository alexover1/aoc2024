/* ========================================================================
   $File: day03.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

bool CheckMatch(string& Input, u64& X, u64& Y)
{
    string Prefix = "mul("_s;
    u32 MinimumLength = "mul(X,Y)"_s.Length;

    if(Input.Length < MinimumLength || !ChopPrefix(&Input, Prefix))
    {
        return(false);
    }

    X = ChopU64(&Input);

    if(!ChopPrefix(&Input, ","_s))
    {
        return(false);
    }

    Y = ChopU64(&Input);

    if(!ChopPrefix(&Input, ")"_s))
    {
        return(false);
    }

    return(true);
}

internal u64
SolvePartOne(string Input)
{
    u64 Result = 0;
    u64 X, Y;

    while(Input.Length > 0)
    {
        if(CheckMatch(Input, X, Y))
        {
            Result += X * Y;
        }
        else
        {
            ChopLeft(&Input, 1);
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(string Input)
{
    u64 Result = 0;
    bool MulsEnabled = true;
    u64 X, Y;

    while(Input.Length > 0)
    {
        if(ChopPrefix(&Input, "do()"_s))
        {
            MulsEnabled = true;
        }
        else if(ChopPrefix(&Input, "don't()"_s))
        {
            MulsEnabled = false;
        }
        else
        {
            if(MulsEnabled && CheckMatch(Input, X, Y))
            {
                Result += X * Y;
            }
            else
            {
                ChopLeft(&Input, 1);
            }
        }
    }

    return(Result);
}

solution Solution03 =
{
    0,
    SolvePartOne,
    SolvePartTwo,
};
