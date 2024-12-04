#include "AdventOfCode.h"

string SampleData = R"""(
MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX
)"""_s;

bool CheckColumn(array<string> Lines, u32 Row, u32 Column, string SearchString)
{
    bool Result = true;

    for(u32 Index = 0; Index < SearchString.Length; Index++)
    {
        if(Lines.Data[Row + Index].Data[Column] != SearchString.Data[Index])
        {
            Result = false;
            break;
        }
    }

    return(Result);
}

bool CheckCharAndBounds(array<string> Lines, u32 Row, u32 Column, char SearchChar)
{
    bool InBounds = Row < Lines.Length && Column < Lines.Data[Row].Length;
    bool Result   = InBounds && Lines.Data[Row].Data[Column] == SearchChar;

    return(Result);
}

u64 CheckDiagonals(array<string> Lines, u32 Row, u32 Column)
{
    bool TL = true;
    bool TR = true;
    bool BL = true;
    bool BR = true;

    string SearchString = "MAS"_s;

    for(u32 Index = 0; Index < SearchString.Length; Index++)
    {
        u32 Distance = Index+1;
        char SearchChar = SearchString.Data[Index];

        TL &= (Row >= Distance && Column >= Distance) && CheckCharAndBounds(Lines, Row-Distance, Column-Distance, SearchChar);
        TR &= (Row >= Distance) && CheckCharAndBounds(Lines, Row-Distance, Column+Distance, SearchChar);
        BL &= (Column >= Distance) && CheckCharAndBounds(Lines, Row+Distance, Column-Distance, SearchChar);
        BR &= CheckCharAndBounds(Lines, Row+Distance, Column+Distance, SearchChar);
    }

    u64 Result = TL + TR + BL + BR;

    return(Result);
}

u64 Search(array<string> Lines, u32 Row, u32 Column)
{
    // Row    : 0..Lines.Length-1
    // Column : 0..Lines[Row].Length-1

    string Line = Lines.Data[Row];

    bool Forwards  = (Column+3 < Line.Length) && StringsAreEqual(&Line.Data[Column], "XMAS", 4);
    bool Backwards = (Column >= 3) && StringsAreEqual(&Line.Data[Column - 3], "SAMX", 4);
    bool Down      = (Row+3 < Lines.Length) && CheckColumn(Lines, Row, Column, "XMAS"_s);
    bool Up        = (Row >= 3) && CheckColumn(Lines, Row - 3, Column, "SAMX"_s);
    u64  Diagonals = CheckDiagonals(Lines, Row, Column);

    u64 Result = Forwards + Backwards + Down + Up + Diagonals;

    return(Result);
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
            printf("Error: Unable to read input file.\n");
            return 1;
        }
    }

    array<string> Lines = {};

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        if(!Line.Length) continue;

        Append(Lines, Line);
    }

    u64 PartOne = 0;

    for(u32 Row = 0; Row < Lines.Length; Row++)
    {
        string& Line = Lines.Data[Row];
        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            if(Line.Data[Column] == 'X')
            {
                PartOne += Search(Lines, Row, Column);
            }
        }
    }

    printf("Part One: %llu\n", PartOne);

    return 0;
}
