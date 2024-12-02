#include "AdventOfCode.h"

void PartOne(string Input)
{
    u64 Result = 0;

    for(u32 LineNumber = 0; Input.Length > 0; LineNumber++)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        string StrLevel = ChopBy(&Line, ' ');
        u64 Level = ParseU64(StrLevel);

        bool Increasing = false;
        bool Decreasing = false;

        while(Line.Length > 0)
        {
            Line = TrimLeft(Line);
            StrLevel = ChopBy(&Line, ' ');

            u64 NextLevel = ParseU64(StrLevel);

            u64 Difference;
            if(NextLevel > Level)
            {
                Difference = NextLevel - Level;
            }
            else
            {
                Difference = Level - NextLevel;
            }

            if(Difference < 1 || Difference > 3)
            {
                Increasing = false;
                Decreasing = false;
                break;
            }

            if(Increasing)
            {
                if(NextLevel < Level)
                {
                    Increasing = false;
                    break;
                }
            }
            else if(Decreasing)
            {
                if(NextLevel > Level)
                {
                    Decreasing = false;
                    break;
                }
            }
            else
            {
                if(NextLevel > Level)
                {
                    Increasing = true;
                }
                else if(NextLevel < Level)
                {
                    Decreasing = true;
                }
            }

            Level = NextLevel;
        }

        if(Increasing || Decreasing)
        {
            Result += 1;
        }
    }

    printf("Part One: %llu\n", Result);
}

string SampleData = R""""(
7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
)""""_s;

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

    PartOne(Input);
    // PartTwo(Input);

    return 0;
}
