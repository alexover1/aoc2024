/* ========================================================================
$File: day13.cpp $
$Date: 12/14/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

struct claw_data
{
    s64 ButtonA[2];
    s64 ButtonB[2];
    s64 Prize[2];
};

internal claw_data
ParseClawData(string Input)
{
    claw_data Result = {};

    Assert(ChopPrefix(&Input, "Button A: X+"_s));
    Result.ButtonA[0] = ChopU64(&Input);
    Assert(ChopPrefix(&Input, ", Y+"_s));
    Result.ButtonA[1] = ChopU64(&Input);

    Input = TrimSpace(Input);

    Assert(ChopPrefix(&Input, "Button B: X+"_s));
    Result.ButtonB[0] = ChopU64(&Input);
    Assert(ChopPrefix(&Input, ", Y+"_s));
    Result.ButtonB[1] = ChopU64(&Input);

    Input = TrimSpace(Input);

    Assert(ChopPrefix(&Input, "Prize: X="_s));
    Result.Prize[0] = ChopU64(&Input);
    Assert(ChopPrefix(&Input, ", Y="_s));
    Result.Prize[1] = ChopU64(&Input);

    return(Result);
}

struct claw_solution
{
    s64 A;
    s64 B;
};

internal claw_solution
SolveDirectSubstitution(claw_data *Data)
{
    s64 X0 = Data->ButtonA[0];
    s64 Y0 = Data->ButtonA[1];
    s64 X1 = Data->ButtonB[0];
    s64 Y1 = Data->ButtonB[1];
    s64 CX = Data->Prize[0];
    s64 CY = Data->Prize[1];

    s64 Dividend;
    s64 Divisor;

    s64 DividendB = (CY*X0 - CX*Y0);
    s64 DivisorB = (Y1*X0 - Y0*X1);

    s64 DividendA = (CX - DividendB*X1/DivisorB);
    s64 DivisorA = X0;

    claw_solution Solution = {};

    if(DividendB%DivisorB == 0 && DividendA%DivisorA == 0)
    {
        Solution.A = DividendA/DivisorA;
        Solution.B = DividendB/DivisorB;
    }

    return(Solution);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    while(Input.Length > 0)
    {
        string ClawDataText = ChopBy(&Input, "\n\n"_s);

        claw_data ClawData = ParseClawData(ClawDataText);
        claw_solution Solution = SolveDirectSubstitution(&ClawData);

        if(Solution.A != 0 || Solution.B != 0)
        {
            Result += 3*Solution.A + Solution.B;
        }
    }

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    while(Input.Length > 0)
    {
        string ClawDataText = ChopBy(&Input, "\n\n"_s);

        claw_data ClawData = ParseClawData(ClawDataText);

        ClawData.Prize[0] += 10000000000000;
        ClawData.Prize[1] += 10000000000000;

        claw_solution Solution = SolveDirectSubstitution(&ClawData);

        if(Solution.A != 0 || Solution.B != 0)
        {
            Result += 3*Solution.A + Solution.B;
        }
    }

    return(Result);
}

solution Solution13 =
{
    SolvePartOne,
    SolvePartTwo,
};
