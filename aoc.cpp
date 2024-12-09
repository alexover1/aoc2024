#include "aoc.h"
#include "solutions.h"

// TODO: To make this work on windows we need minirent.h
#include <dirent.h>

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
RunTestCase(const char *FileName)
{
    bool Result = true;

    string Contents = ReadFileData(FileName);
    if(!Contents.Data)
    {
        PrintMessage("[ERROR] Could not read the file '%s'\n", FileName);
        return(false);
    }

    test_case TestCase = {};
    u64 DayNumber = 0;

    string Input = Contents;

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');

        if(HasPrefix(Line, ":i "_s))
        {
            string ValueName;
            u64 Value = ReadIntField(Line, ValueName);

            if(ValueName == "day")
            {
                DayNumber = Value;
            }
            if(ValueName == "part")
            {
                TestCase.PartOne = Value;
                TestCase.PartTwo = Value;
            }
            else if(ValueName == "part_one")
            {
                TestCase.PartOne = Value;
            }
            else if(ValueName == "part_two")
            {
                TestCase.PartTwo = Value;
            }
        }
        else if(HasPrefix(Line, ":b "_s))
        {
            string BlobName;
            string Blob = ReadBlobField(Line, Input, BlobName);

            if(BlobName == "input")
            {
                TestCase.Input = Blob;
            }
        }
    }

    PrintMessage("[INFO] Running %s\n", FileName);

    if(DayNumber == 0 || DayNumber > ArrayLength(Solutions))
    {
        PrintMessage("[ERROR] Day number '%lu' is not valid\n", DayNumber);
    }
    else
    {
        solution& Solution = Solutions[DayNumber-1];

        if(Solution.ParseFn)
        {
            Solution.ParseFn(TestCase.Input);
        }

        u64 PartOne = Solution.PartOneFn(TestCase.Input);
        if(PartOne != TestCase.PartOne)
        {
            PrintMessage("[ERROR] Part one failed\n");
            PrintMessage("  Expected:\n");
            PrintMessage("    %lu\n", TestCase.PartOne);
            PrintMessage("  Actual:\n");
            PrintMessage("    %lu\n", PartOne);
            Result = false;
        }

        u64 PartTwo = Solution.PartTwoFn(TestCase.Input);
        if(PartTwo != TestCase.PartTwo)
        {
            PrintMessage("[ERROR] Part two failed\n");
            PrintMessage("  Expected:\n");
            PrintMessage("    %lu\n", TestCase.PartTwo);
            PrintMessage("  Actual:\n");
            PrintMessage("    %lu\n", PartTwo);
            Result = false;
        }
    }

    delete[] Contents.Data;

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
                FormatString(sizeof(PathBuffer), PathBuffer, "%s/%s", TestsDirectory, FileName);

                if(!RunTestCase(PathBuffer))
                {
                    Result = false;
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

            string Contents = ReadFileData(PathBuffer);
            if(Contents.Data)
            {
                solution& Solution = Solutions[DayNumber-1];

                if(Solution.ParseFn)
                {
                    Solution.ParseFn(Contents);
                }

                u64 PartOne = Solution.PartOneFn(Contents);
                PrintMessage("[INFO] Part One: %lu\n", PartOne);

                u64 PartTwo = Solution.PartTwoFn(Contents);
                PrintMessage("[INFO] Part Two: %lu\n", PartTwo);
            }
            else
            {
                PrintMessage("[ERROR] Input file was not found for day %lu (file name is '%s')\n", DayNumber, PathBuffer);
                Result = false;
            }
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
                            FormatString(sizeof(PathBuffer), PathBuffer, "%s/input%02lu.txt", DataDirectory, DayNumber);

                            string Contents = ReadFileData(PathBuffer);
                            if(Contents.Data)
                            {
                                solution& Solution = Solutions[DayNumber-1];

                                if(Solution.ParseFn)
                                {
                                    Solution.ParseFn(Contents);
                                }

                                PrintMessage("[INFO] Running day %lu\n", DayNumber);

                                u64 PartOne = Solution.PartOneFn(Contents);
                                PrintMessage("[INFO] - Part One: %lu\n", PartOne);

                                u64 PartTwo = Solution.PartTwoFn(Contents);
                                PrintMessage("[INFO] - Part Two: %lu\n", PartTwo);

                                delete[] Contents.Data;
                            }
                            else
                            {
                                PrintMessage("[ERROR] Input file was not found for day %lu (file name is '%s')\n", DayNumber, PathBuffer);
                                Result = false;
                            }
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
            if(!RunTests())
            {
                Result = 1;
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
