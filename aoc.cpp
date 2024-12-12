/* ========================================================================
   $File: aoc.cpp $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#include "aoc.h"
#include "solutions.h"

#define MINIRENT_IMPLEMENTATION
#include "minirent.h"

char HelpMessage[] = R"(
There are multiple ways you can use this tool.

The first way is to download your input data and put it into the data/
folder, where the tool will automatically determine which day
corresponds to which input by looking at the name of the file. For
example, the corresponding input file for day six should be
`data/input06.txt`. All you have to do is run the command again with
the `run` subcommand and an optional number indicating which day to
run. If you do not indicate which day to run, all days that you have
downloaded the input for will automatically be run.

The second way is to run the command again with the `test` subcommand,
which will test all of the test cases found in the tests/ folder,
which is generally all of the sample data from the Advent of Code
website, as well as some additional cases I found useful when solving
the problems.
)";

global memory_arena Arena = {};

internal u64
ReadIntField(string Line, string& Name)
{
    u64 Result = 0;

    Assert(ChopPrefix(&Line, ":i "_s));
    Line = TrimLeft(Line);
    Name = ChopBy(&Line, ' ');

    string ResultString = TrimSpace(Line);
    Result = ParseU64(ResultString);

    return(Result);
}

internal string
ReadBlobField(string Line, string& Contents, string& Name)
{
    string Result = {};

    Assert(ChopPrefix(&Line, ":b "_s));
    Line = TrimLeft(Line);
    Name = ChopBy(&Line, ' ');

    string BlobSizeString = TrimSpace(Line);
    u64 BlobSize = ParseU64(BlobSizeString);

    Assert(Contents.Length >= BlobSize);
    Assert(BlobSize <= UINT_MAX);

    Result = ChopLeft(&Contents, (u32)BlobSize);

    return(Result);
}

internal bool
RunTestCase(const char *FileName, u64 DayNumber)
{
    bool Result = true;
    test_case TestCase = {};

    if(DayNumber == 0 || DayNumber > ArrayLength(Solutions))
    {
        PrintMessage("[ERROR] Invalid day number %lu for test file '%s'\n", DayNumber, FileName);
        Result = false;
        return(Result);
    }

    memory_arena_mark Mark = ArenaSnapshot(&Arena);

    //
    // TODO: Make ReadFileData return just a pointer (so we can use mmap()).
    // char *FileData = ReadFileData(FileName);
    //
    string Contents = {};
    if(!ReadFileData(&Arena, FileName, Contents))
    {
        PrintMessage("[ERROR] Could not read the file '%s'\n", FileName);
        Result = false;
        return(Result);
    }

    while(Contents.Length > 0)
    {
        string Line = ChopBy(&Contents, '\n');

        if(HasPrefix(Line, ":i "_s))
        {
            string ValueName;
            u64 Value = ReadIntField(Line, ValueName);

            if(ValueName == "part_one")
            {
                TestCase.PartOne = Value;
                TestCase.HasPartOne = true;
            }
            else if(ValueName == "part_two")
            {
                TestCase.PartTwo = Value;
                TestCase.HasPartTwo = true;
            }
            else
            {
                PrintMessage("[WARNING] Unknown integer field '%S'\n", ValueName);
            }
        }
        else if(HasPrefix(Line, ":b "_s))
        {
            string BlobName;
            string Blob = ReadBlobField(Line, Contents, BlobName);

            if(BlobName == "input")
            {
                TestCase.Input = Blob;
            }
            else
            {
                PrintMessage("[WARNING] Unknown blob field '%S'\n", BlobName);
            }
        }
    }

    // TODO: It would be nice to print extra information, like how much time each part took and the amount of memory used.
    //
    // u32 MemoryUsed = 0;
    // for(region *Region = Mark.Region->Next; Region != NULL; Region = Region->Next)
    // {
    //     MemoryUsed += Region->Used;
    // }
    // PrintMessage("Memory used: %m\n", Mark.Region->Used - Mark.Used);
    //

    PrintMessage("[INFO] Running %s\n", FileName);

    solution& Solution = Solutions[DayNumber-1];

    if(TestCase.HasPartOne)
    {
        u64 Result = Solution.PartOneFn(&Arena, TestCase.Input);
        if(Result != TestCase.PartOne)
        {
            PrintMessage("[ERROR] Part one failed\n");
            PrintMessage("  Expected:\n");
            PrintMessage("    %lu\n", TestCase.PartOne);
            PrintMessage("  Actual:\n");
            PrintMessage("    %lu\n", Result);
            Result = false;
        }
    }

    if(TestCase.HasPartTwo)
    {
        u64 Result = Solution.PartTwoFn(&Arena, TestCase.Input);
        if(Result != TestCase.PartTwo)
        {
            PrintMessage("[ERROR] Part two failed\n");
            PrintMessage("  Expected:\n");
            PrintMessage("    %lu\n", TestCase.PartTwo);
            PrintMessage("  Actual:\n");
            PrintMessage("    %lu\n", Result);
            Result = false;
        }
    }

    ArenaRewind(&Arena, Mark);

    return(Result);
}

internal bool
RunTests()
{
    bool Result = true;

    char PathBuffer[4096];
    const char *TestsDirectory = "tests";

    DIR *dir = opendir(TestsDirectory);
    if(dir)
    {
        dirent *ent = readdir(dir);
        while(ent)
        {
            char *FileName = ent->d_name;

            if(*FileName != '.')
            {
                string ShortPath = {FileName, StringLength(FileName)};
                u64 DayNumber = ChopU64(&ShortPath);
                if(DayNumber > 0 && DayNumber <= ArrayLength(Solutions))
                {
                    FormatString(sizeof(PathBuffer), PathBuffer, "%s/%s", TestsDirectory, FileName);

                    if(!RunTestCase(PathBuffer, DayNumber))
                    {
                        Result = false;
                    }
                }
                else
                {
                    PrintMessage("[WARNING] Ignorning test cases with invalid day number %lu '%s'\n", DayNumber, FileName);
                }
            }

            ent = readdir(dir);
        }

        closedir(dir);
    }
    else
    {
        PrintMessage("[ERROR] Cannot read the directory '%s'\n", TestsDirectory);
        Result = false;
    }

    return(Result);
}

internal bool
RunDay(u64 DayNumber)
{
    bool Result = true;

    local_persist char PathBuffer[4096];
    const char *DataDirectory = "data";

    u64 MaxDays = ArrayLength(Solutions);

    if(DayNumber > 0)
    {
        if(DayNumber <= MaxDays)
        {
            FormatString(sizeof(PathBuffer), PathBuffer, "%s/input%02lu.txt", DataDirectory, DayNumber);

            memory_arena_mark Mark = ArenaSnapshot(&Arena);

            string Contents = {};
            if(ReadFileData(&Arena, PathBuffer, Contents))
            {
                solution& Solution = Solutions[DayNumber-1];

                u64 PartOne = Solution.PartOneFn(&Arena, Contents);
                PrintMessage("[INFO] Part One: %lu\n", PartOne);

                u64 PartTwo = Solution.PartTwoFn(&Arena, Contents);
                PrintMessage("[INFO] Part Two: %lu\n", PartTwo);
            }
            else
            {
                PrintMessage("[ERROR] Input file was not found for day %lu (file name is '%s')\n", DayNumber, PathBuffer);
                Result = false;
            }

            ArenaRewind(&Arena, Mark);
        }
        else
        {
            PrintMessage("[ERROR] Day number '%lu' is not valid (maximum is %lu)\n", DayNumber, MaxDays);
            Result = false;
        }
    }
    else
    {
        DIR *dir = opendir(DataDirectory);
        if(dir)
        {
            dirent *ent = readdir(dir);
            while(ent)
            {
                char *FileName = ent->d_name;

                if(*FileName != '.')
                {
                    string ShortPath = {FileName, StringLength(FileName)};

                    if(ChopPrefix(&ShortPath, "input"_s))
                    {
                        u64 DayNumber = ChopU64(&ShortPath);
                        if(DayNumber > 0 && DayNumber <= MaxDays)
                        {
                            // TODO: Use SetWorkingDirectory to not have to format the path here.
                            FormatString(sizeof(PathBuffer), PathBuffer, "%s/input%02lu.txt", DataDirectory, DayNumber);

                            memory_arena_mark Mark = ArenaSnapshot(&Arena);

                            string Contents = {};
                            if(ReadFileData(&Arena, PathBuffer, Contents))
                            {
                                solution& Solution = Solutions[DayNumber-1];

                                PrintMessage("[INFO] Running day %lu\n", DayNumber);

                                u64 PartOne = Solution.PartOneFn(&Arena, Contents);
                                PrintMessage("[INFO] - Part One: %lu\n", PartOne);

                                u64 PartTwo = Solution.PartTwoFn(&Arena, Contents);
                                PrintMessage("[INFO] - Part Two: %lu\n", PartTwo);
                            }
                            else
                            {
                                PrintMessage("[ERROR] Input file was not found for day %lu (file name is '%s')\n", DayNumber, PathBuffer);
                                Result = false;
                            }

                            ArenaRewind(&Arena, Mark);
                        }
                    }
                }

                ent = readdir(dir);
            }

            closedir(dir);
        }
        else
        {
            PrintMessage("[ERROR] Cannot read the directory '%s'\n", DataDirectory);
            Result = false;
        }
    }

    return(Result);
}

int main(int ArgCount, char **Args)
{
    int Result = 0;
    ShiftArgs(&ArgCount, &Args);

    if(ArgCount > 0)
    {
        char *Subcommand = ShiftArgs(&ArgCount, &Args);
        if("test"_s == Subcommand || "tests"_s == Subcommand)
        {
            if(ArgCount > 0)
            {
                char *FileName = ShiftArgs(&ArgCount, &Args);

                string Path = {FileName, StringLength(FileName)};
                Path = PathFileName(Path);
                u64 DayNumber = ChopU64(&Path);

                if(!RunTestCase(FileName, DayNumber))
                {
                    Result = 1;
                }
            }
            else
            {
                if(!RunTests())
                {
                    Result = 1;
                }
            }
        }
        if("run"_s == Subcommand)
        {
            u64 DayNumber = 0;
            if(ArgCount > 0)
            {
                // TODO: Use S32FromZ and check for errors?
                char *Arg = ShiftArgs(&ArgCount, &Args);
                string ValueString = {Arg, StringLength(Arg)};

                DayNumber = ParseU64(ValueString);
            }

            if(ArgCount > 0)
            {
                PrintMessage("[ERROR] The `run` subcommand takes at most 1 argument\n");
                Result = 1;
            }
            else
            {
                if(!RunDay(DayNumber))
                {
                    Result = 1;
                }
            }
        }
    }
    else
    {
        PrintMessage("Advent of Code 2024 - Alex Overstreet\n%s", HelpMessage);
    }

    return(Result);
}
