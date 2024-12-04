#include "AdventOfCode.h"

string SampleData = R""""(
3   4
4   3
2   5
1   3
3   9
3   3
)""""_s;

void ParseInput(string Input, array<u64>& ListOne, array<u64>& ListTwo)
{
    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        string StrA = ChopBy(&Line, ' ');
        Line = TrimLeft(Line);
        string StrB = ChopBy(&Line, ' ');

        u64 A = ParseU64(StrA);
        u64 B = ParseU64(StrB);

        Append(ListOne, A);
        Append(ListTwo, B);
    }
}

void PartOne(string Input)
{
    array<u64> ListOne = {};
    array<u64> ListTwo = {};

    ParseInput(Input, ListOne, ListTwo);

    Sort(ListOne);
    Sort(ListTwo);

    u64 Result = 0;

    for(u32 Index = 0; Index < ListOne.Length; Index++)
    {
        u32 X = ListOne.Data[Index];
        u32 Y = ListTwo.Data[Index];

        if(X > Y)
        {
            Result += X - Y;
        }
        else
        {
            Result += Y - X;
        }
    }

    PrintMessage("Part One: %llu\n", Result);
}

void PartTwo(string Input)
{
    array<u64> ListOne = {};
    array<u64> ListTwo = {};

    ParseInput(Input, ListOne, ListTwo);

    u64 Result = 0;

    for(u32 Index = 0; Index < ListOne.Length; Index++)
    {
        u32 Count = 0;
        for(u32 OtherIndex = 0; OtherIndex < ListTwo.Length; OtherIndex++)
        {
            if(ListOne.Data[Index] == ListTwo.Data[OtherIndex])
            {
                Count += 1;
            }
        }

        Result += ListOne.Data[Index] * Count;
    }

    PrintMessage("Part Two: %llu\n", Result);
}

int main(int ArgCount, char **Args)
{
    string Input = SampleData;

    if(ArgCount > 1)
    {
        char *FileName = Args[1];

        Input = ReadFileData(FileName);
        if(!Input.Data)
        {
            PrintMessage("Error: Unable to read input file.\n");
            return 1;
        }
    }

    PartOne(Input);
    PartTwo(Input);

    return 0;
}
