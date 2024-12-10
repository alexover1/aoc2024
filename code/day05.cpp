/* ========================================================================
   $File: day05.cpp $
   $Date: 12/08/24 $
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

internal void
ParseInput(string& Input, array<pair_ordering_rule>& Rules)
{
    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) break;

        pair_ordering_rule Rule = {};

        Rule.First = (u32) ChopU64(&Line);
        ChopPrefix(&Line, "|"_s);
        Rule.Last = (u32) ChopU64(&Line);

        Append(Rules, Rule);
    }
}

internal bool
CheckPageOrder(array<u32> Pages, array<pair_ordering_rule> Rules)
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

// TODO: This is the only part of this day that is not thread-safe!
internal array<pair_ordering_rule> RulesInUse = {};

internal bool
PageCompareFn(u32 PageA, u32 PageB)
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
internal void
UpdatePageOrder(array<u32>& Pages, array<pair_ordering_rule> Rules)
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
SolvePartOne(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<pair_ordering_rule> Rules = {};
    array<u32> Pages = {};

    ParseInput(Input, Rules);

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

    // TODO: Use the Arena for allocation!
    delete[] Rules.Data;
    delete[] Pages.Data;

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    u64 Result = 0;

    array<pair_ordering_rule> Rules = {};
    array<u32> Pages = {};

    ParseInput(Input, Rules);

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

    // TODO: Use the Arena for allocation!
    delete[] Rules.Data;
    delete[] Pages.Data;

    return(Result);
}

solution Solution05 =
{
    SolvePartOne,
    SolvePartTwo,
};
