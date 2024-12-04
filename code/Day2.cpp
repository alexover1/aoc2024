#include "AdventOfCode.h"

bool PairIsSafe(u64 Level, u64 NextLevel)
{
    u64 Difference;
    if(NextLevel > Level)
    {
        Difference = NextLevel - Level;
    }
    else
    {
        Difference = Level - NextLevel;
    }

    bool Result = (Difference > 0 && Difference < 4);

    return(Result);
}

// Check if a line is safe without any skipped values.
bool LineIsSafe(u64 *Levels, u32 LevelCount, bool& Increasing)
{
    bool Result = true;

    const u32 FirstIndex = 1;

    for(u32 Index = FirstIndex; Index < LevelCount; Index++)
    {
        if(!PairIsSafe(Levels[Index-1], Levels[Index]))
        {
            Result = false;
            break;
        }

        bool NextIncreasing = Levels[Index] > Levels[Index-1];

        if(Index > FirstIndex)
        {
            if(Increasing != NextIncreasing)
            {
                Result = false;
                break;
            }
        }
        else
        {
            Increasing = NextIncreasing;
        }
    }

    return(Result);
}

bool CheckLinePartOne(string Line)
{
    string StrLevel = ChopBy(&Line, ' ');
    u64 Level = ParseU64(StrLevel);

    bool Increasing = false;
    bool Decreasing = false;

    while(Line.Length > 0)
    {
        Line = TrimLeft(Line);
        StrLevel = ChopBy(&Line, ' ');

        u64 NextLevel = ParseU64(StrLevel);

        if(!PairIsSafe(Level, NextLevel))
        {
            return false;
        }

        if(NextLevel > Level)
        {
            if(Decreasing) return false;
            Increasing = true;
        }
        else
        {
            if(Increasing) return false;
            Decreasing = true;
        }

        Level = NextLevel;
    }

    return true;
}

array<u64> Levels = {};

bool CheckLinePartTwo(string Line)
{
    bool Result = true;

    bool AlreadySkipped = false;
    bool Increasing = false;
    bool SetOrder = false;

    while(Line.Length > 0)
    {
        string StrLevel = ChopBy(&Line, ' ');
        u64 Level = ParseU64(StrLevel);
        Line = TrimLeft(Line);

        Append(Levels, Level);
    }

    const u32 LineCount = 3;
    Assert(Levels.Length >= LineCount);

    for(u32 Index = 0; Index < Levels.Length-LineCount; Index++)
    {
        const u32 WindowCount = 4;

        bool NextIncreasing;

        if(LineIsSafe(&Levels.Data[Index], LineCount, NextIncreasing))
        {
            if(SetOrder)
            {
                if(Increasing != NextIncreasing)
                {
                    if(AlreadySkipped)
                    {
                        Result = false;
                        break;
                    }

                    AlreadySkipped = true;
                }
            }
            else
            {
                SetOrder = true;
                Increasing = NextIncreasing;
            }

            continue;
        }

        if(AlreadySkipped)
        {
            Result = false;
            break;
        }

        for(u32 SkipIndex = 0; SkipIndex < WindowCount; SkipIndex++)
        {
            u64 TempLevels[LineCount];
            u32 TempIndex = 0;
            for(u32 J = 0; J < WindowCount; J++)
            {
                if(J == SkipIndex) continue;
                TempLevels[TempIndex++] = Levels.Data[Index+J];
            }

            if(LineIsSafe(TempLevels, LineCount, NextIncreasing))
            {
                // If we already determined ascending or descending,
                // and removing the element causes the remaining
                // elements to have a different order that the rest of
                // the set, the removal of this element is useless.
                if(SetOrder)
                {
                    if(Increasing != NextIncreasing)
                    {
                        continue;
                    }
                }
                else
                {
                    SetOrder = true;
                    Increasing = NextIncreasing;
                }

                // Otherwise, we have found a valid element to skip
                // and can remove it from the set and continue
                // checking the rest of the line.
                for(u32 J = 0; J < LineCount; J++)
                {
                    Levels.Data[Index+J+1] = TempLevels[J];
                }

                AlreadySkipped = true;
                break;
            }
        }

        if(!AlreadySkipped)
        {
            Result = false;
            break;
        }
    }

    bool NextIncreasing;
    if(!LineIsSafe(&Levels.Data[Levels.Length-LineCount], LineCount, NextIncreasing))
    {
        if(AlreadySkipped || (SetOrder && Increasing != NextIncreasing))
        {
            Result = false;
        }
    }

    Levels.Length = 0;

    return(Result);
}

string SampleData = R""""(
7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
)""""_s;

string EdgeCaseData = R""""(
48 46 47 49 51 54 56
1 1 2 3 4 5
1 2 3 4 5 5
5 1 2 3 4 5
1 4 3 2 1
1 6 7 8 9
1 2 3 4 3
9 8 7 6 7
7 10 8 10 11
29 28 27 25 26 25 22 20
)""""_s;

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

    u64 PartOne = 0;
    u64 PartTwo = 0;

    for(u32 LineNumber = 0; Input.Length > 0; LineNumber++)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        if(CheckLinePartOne(Line))
        {
            PartOne += 1;
        }

        if(CheckLinePartTwo(Line))
        {
            PartTwo += 1;
        }
    }

    PrintMessage("Part One: %llu\n", PartOne);
    PrintMessage("Part Two: %llu\n", PartTwo);

    return 0;
}
