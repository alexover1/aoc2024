/* ========================================================================
   $File: aoc.h $
   $Date: 12/8/24 $
   $Revision: $
   $Creator: Alex Overstreet $
   $Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
   ======================================================================== */

#pragma once

#include "utils.h"

struct test_case
{
    string Input;
    u64 PartOne;
    u64 PartTwo;
    bool HasPartOne;
    bool HasPartTwo;
};

struct solution
{
    u64  (*PartOneFn)(memory_arena *Arena, string Input);
    u64  (*PartTwoFn)(memory_arena *Arena, string Input);
};
