#include "AdventOfCode.h"

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

u64 SolvePartOne(string Input)
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

u64 SolvePartTwo(string Input)
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

string SampleData = R"""(
xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
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

    u64 PartOne = SolvePartOne(Input);
    u64 PartTwo = SolvePartTwo(Input);

    printf("Part One: %llu\n", PartOne);
    printf("Part Two: %llu\n", PartTwo);

    return 0;
}
