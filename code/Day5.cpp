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

bool CheckPagesOrder(array<u32> Pages, array<pair_ordering_rule> Rules)
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

int main(int ArgCount, char **Args)
{
    string Input = SampleData;

    array<pair_ordering_rule> Rules = {};
    array<u32> Pages = {};

    u32 PartOne = 0;

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

        if(Pages.Length > 0 && CheckPagesOrder(Pages, Rules))
        {
            u32 MiddleIndex = Pages.Length/2;
            PartOne += Pages.Data[MiddleIndex];
        }

        Pages.Length = 0;
    }

    PrintMessage("Part One: %u\n", PartOne);

    return(0);
}
