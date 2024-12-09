/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#pragma once
#include "aoc_lib.h"

struct test_case
{
    u64 PartOne;
    u64 PartTwo;
    string Input;
};

struct solution
{
    void (*ParseFn)(string Input);
    u64  (*PartOneFn)(string Input);
    u64  (*PartTwoFn)(string Input);
};
