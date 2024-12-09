/* ========================================================================
   $File: day05.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"

struct pair_ordering_rule
{
    u32 First;
    u32 Last;
};

internal array<pair_ordering_rule> Rules = {};
internal array<u32> Pages = {};

internal void
ParseInput(string Input)
{
    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) break;

        pair_ordering_rule Rule = {};

        Rule.First = (u32) ChopU64(&Line);
        ChopPrefix(&Line, "|"_s);
        Rule.Last = (u32) ChopU64(&Line);

        Append(Rules, Rule);
    }
}

bool CheckPageOrder(array<u32> Pages, array<pair_ordering_rule> Rules)
{
    bool Result = true;

    for(u32 RuleIndex = 0; RuleIndex < Rules.Length; RuleIndex++) {
        pair_ordering_rule Rule = Rules.Data[RuleIndex];

        bool ContainsLast = false;

        for(u32 PageIndex = 0; PageIndex < Pages.Length; PageIndex++)
        {
            u32 Page = Pages.Data[PageIndex];

            if(Page == Rule.First)
            {
                if(ContainsLast)
                {
                    Result = false;
                    goto SkipToEnd;
                }
            }
            else if(Page == Rule.Last)
            {
                ContainsLast = true;
            }
        }
    }

SkipToEnd:

    return(Result);
}

bool RuleCompareFn(pair_ordering_rule A, pair_ordering_rule B)
{
    bool Result = (A.First > B.First) || (A.Last > B.Last);
    return(Result);
}

void SortPages(array<pair_ordering_rule>& Array)
{
    if(Array.Length < 2)
    {
        return;
    }

    int InitJ = 0;
    int LastJ = Array.Length - 1;

    for(;;)
    {
        int InitSwap = -1;
        int PrevSwap = -1;

        for(int J = InitJ; J < LastJ; J++)
        {
            if(RuleCompareFn(Array.Data[J], Array.Data[J+1]))
            {
                Swap(Array.Data[J], Array.Data[J+1]);
                PrevSwap = J;
                if(InitSwap == -1)
                {
                    InitSwap = J;
                }
            }
        }

        if(PrevSwap == -1)
        {
            return;
        }

        InitJ = Max(InitSwap-1, 0);
        LastJ = PrevSwap;
    }
}

internal array<pair_ordering_rule> RulesInUse = {};

bool PageCompareFn(u32 PageA, u32 PageB)
{
    bool Result = false;

    for(u32 RuleIndex = 0; RuleIndex < RulesInUse.Length; RuleIndex++)
    {
        pair_ordering_rule Rule = RulesInUse.Data[RuleIndex];
        if(Rule.First == PageA && Rule.Last == PageB)
        {
            Result = true;
            break;
        }
    }

    return(Result);
}

// Returns true if the page listing was updated.
void UpdatePageOrder(array<u32>& Pages, array<pair_ordering_rule> Rules)
{
    RulesInUse.Length = 0;

    for(u32 RuleIndex = 0; RuleIndex < Rules.Length; RuleIndex++) {
        pair_ordering_rule Rule = Rules.Data[RuleIndex];

        bool ContainsLast = false;
        bool ContainsFirst = false;

        for(u32 PageIndex = 0; PageIndex < Pages.Length; PageIndex++)
        {
            u32 Page = Pages.Data[PageIndex];

            if(Page == Rule.First)
            {
                ContainsFirst = true;
            }
            else if(Page == Rule.Last)
            {
                ContainsLast = true;
            }
        }

        if(ContainsFirst && ContainsLast)
        {
            Append(RulesInUse, Rule);
        }
    }

    SortBy(Pages, PageCompareFn);
}

internal u64
SolvePartOne(string Input)
{
    u64 Result = 0;

    ChopBy(&Input, "\n\n"_s);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        while(Line.Length > 0)
        {
            string Word = ChopBy(&Line, ',');
            u32 PageNumber = ParseU32(Word);

            Append(Pages, PageNumber);
        }

        u32 MiddleIndex = Pages.Length/2;

        if(CheckPageOrder(Pages, Rules))
        {
            Result += Pages.Data[MiddleIndex];
        }

        Pages.Length = 0;
    }

    return(Result);
}

internal u64
SolvePartTwo(string Input)
{
    u64 Result = 0;

    ChopBy(&Input, "\n\n"_s);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        while(Line.Length > 0)
        {
            string Word = ChopBy(&Line, ',');
            u32 PageNumber = ParseU32(Word);

            Append(Pages, PageNumber);
        }

        u32 MiddleIndex = Pages.Length/2;

        if(!CheckPageOrder(Pages, Rules))
        {
            UpdatePageOrder(Pages, Rules);
            Result += Pages.Data[MiddleIndex];
        }

        Pages.Length = 0;
    }

    return(Result);
}

solution Solution05 =
{
    ParseInput,
    SolvePartOne,
    SolvePartTwo,
};
