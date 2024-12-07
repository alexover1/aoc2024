#include "AdventOfCode.h"

string SampleData = R"""(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47
)"""_s;

struct pair_ordering_rule
{
    u32 First;
    u32 Last;
};

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

bool RuleCompare(pair_ordering_rule A, pair_ordering_rule B)
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
            if(RuleCompare(Array.Data[J], Array.Data[J+1]))
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

array<pair_ordering_rule> RulesInUse = {};

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

int main(int ArgCount, char **Args)
{
    string Input = SampleData;

    array<pair_ordering_rule> Rules = {};
    array<u32> Pages = {};

    u32 PartOne = 0;
    u32 PartTwo = 0;

    if(ArgCount > 1)
    {
        char *FileName = Args[1];

        Input = ReadFileData(FileName);
        if(!Input.Data)
        {
            PrintMessage("Error: Unable to read input file.\n");
            return(1);
        }
    }

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

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
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
            PartOne += Pages.Data[MiddleIndex];
        }
        else
        {
            UpdatePageOrder(Pages, Rules);
            PartTwo += Pages.Data[MiddleIndex];
        }

        Pages.Length = 0;
    }

    PrintMessage("Part One: %u\n", PartOne);
    PrintMessage("Part Two: %u\n", PartTwo);

    return(0);
}
