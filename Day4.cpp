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

bool CheckCorners(array<string> Lines, u32 Row0, u32 Column0, u32 Row1, u32 Column1)
{
    bool Result = (Lines.Data[Row0].Data[Column0] == 'M' || Lines.Data[Row0].Data[Column0] == 'S') &&
                  (Lines.Data[Row1].Data[Column1] == 'M' || Lines.Data[Row1].Data[Column1] == 'S') &&
                  (Lines.Data[Row0].Data[Column0] != Lines.Data[Row1].Data[Column1]);
    return(Result);
}

bool SearchStars(array<string> Lines, u32 Row, u32 Column)
{
    bool Result = false;

    if(Row >= 1 && Row < Lines.Length-1 && Column >= 1 && Column < Lines.Data[Row].Length-1)
    {
        Result = CheckCorners(Lines, Row-1, Column-1, Row+1, Column+1) && CheckCorners(Lines, Row+1, Column-1, Row-1, Column+1);
    }

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
            PrintMessage("Error: Unable to read input file.\n");
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
    u64 PartTwo = 0;

    for(u32 Row = 0; Row < Lines.Length; Row++)
    {
        string Line = Lines.Data[Row];
        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            if(Line.Data[Column] == 'X')
            {
                PartOne += Search(Lines, Row, Column);
            }
            else if(Line.Data[Column] == 'A')
            {
                if(SearchStars(Lines, Row, Column))
                {
                    PartTwo += 1;
                }
            }
        }
    }

    PrintMessage("Part One: %llu\n", PartOne);
    PrintMessage("Part Two: %llu\n", PartTwo);

    return 0;
}
