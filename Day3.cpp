#include "AdventOfCode.h"

bool CheckMatch(string Line, u64& X, u64& Y)
{
    string Prefix = "mul("_s;
    u32 MinimumLength = "mul(X,Y)"_s.Length;

    if(Line.Length < MinimumLength || !StartsWith(Line, Prefix))
    {
        return(false);
    }

    ChopLeft(&Line, Prefix.Length);

    X = ChopU64(&Line);

    if(!StartsWith(Line, ','))
    {
        return(false);
    }
    ChopLeft(&Line, 1);

    Y = ChopU64(&Line);

    if(!StartsWith(Line, ')'))
    {
        return(false);
    }
    ChopLeft(&Line, 1);

    // if(StartsWith(Line, Prefix))
    // {


    //     ChopU64(&Line);
    //     ChopPrefix(&Line, ")");

    //     u32 Index = 0;
    //     while(Index < Line.Length && IsDigit(Line.Data[Index]))
    //     {
    //         Index += 1;
    //     }

    //     if(Index > 0 && StartsWith(Line, ','))
    //     {
    //         Index = 0;
    //         while(IsDigit(Line.Data[Index]))
    //         {
    //             Index += 1;
    //         }
    //     }
    // }

    return(true);
}

void HandleLinePartOne(string Line, u64& Result)
{
    for(u32 Index = 0; Index < Line.Length; Index++)
    {
        string LineSubStr = {Line.Data + Index, Line.Length - Index};

        u64 X, Y;
        if(CheckMatch(LineSubStr, X, Y))
        {
            Result += X * Y;
        }
    }
}

string SampleData = R"""(
xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
)"""_s;

int main(int ArgCount, char **Args)
{
    string Input = SampleData;

    char *Program = ShiftArgs(&ArgCount, &Args);
    if(ArgCount > 0)
    {
        char *FileName = ShiftArgs(&ArgCount, &Args);

        Input = ReadFileData(FileName);
        if(!Input.Data)
        {
            printf("Error: Unable to read input file.\n");
            return 1;
        }
    }

    u64 PartOne = 0;

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        HandleLinePartOne(Line, PartOne);
    }

    printf("Part One: %llu\n", PartOne);

    return 0;
}
