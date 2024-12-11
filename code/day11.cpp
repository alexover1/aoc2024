/* ========================================================================
$File: day11.cpp $
$Date: 12/11/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

struct map
{
    memory_arena *Arena;
    u64 *Keys;
    u64 *Values;
    u32 Length;
    u32 Allocated;
};

internal void
Insert(map *Map, u64 Key, u64 Value)
{
    for(u32 Index = 0; Index < Map->Length; Index++)
    {
        if(Map->Keys[Index] == Key)
        {
            Map->Values[Index] += Value;
            return;
        }
    }

    if(Map->Length >= Map->Allocated)
    {
        u32 NewAllocated = Map->Allocated > 0 ? Map->Allocated * 2 : 16;
        Map->Keys = (u64 *) ArenaRealloc(Map->Arena, Map->Keys, sizeof(u64) * Map->Allocated, sizeof(u64) * NewAllocated);
        Map->Values = (u64 *) ArenaRealloc(Map->Arena, Map->Values, sizeof(u64) * Map->Allocated, sizeof(u64) * NewAllocated);
        Map->Allocated = NewAllocated;
    }

    Map->Keys[Map->Length] = Key;
    Map->Values[Map->Length] = Value;

    Map->Length++;
}

internal map
CopyMap(memory_arena *Arena, map Map)
{
    map Result = {};

    Result.Arena = Arena;

    if(Map.Allocated > 0)
    {
        u32 Size = sizeof(u64) * Map.Allocated;
        u8 *Memory = (u8 *) ArenaAlloc(Arena, Size * 2);

        Result.Keys = (u64 *) Memory;
        memcpy(Result.Keys, Map.Keys, sizeof(u64) * Map.Length);
        Result.Values = (u64 *) (Memory + Size);
        memcpy(Result.Values, Map.Values, sizeof(u64) * Map.Length);

        Result.Allocated = Map.Allocated;
        Result.Length = Map.Length;
    }

    return(Result);
}

internal map
ParseInput(memory_arena *Arena, string Input)
{
    map Map = {};
    Map.Arena = Arena;

    Input = TrimLeft(Input);

    while(Input.Length > 0)
    {
        u64 Value = ChopU64(&Input);
        Insert(&Map, Value, 1);

        Input = TrimLeft(Input);
    }

    return(Map);
}

internal u64
NumDigits(u64 X)
{
    return((X == 0) ? 1 : (u64)log10(X) + 1);
}

internal void
Transform(memory_arena *TempArena, map *Map)
{
    memory_arena_mark Mark = ArenaSnapshot(TempArena);
    map TempMap = CopyMap(TempArena, *Map);

    Map->Length = 0;

    for(u32 Index = 0; Index < TempMap.Length; Index++)
    {
        u64 Value = TempMap.Keys[Index];
        u64 Counter = TempMap.Values[Index];

        if(Value == 0)
        {
            Insert(Map, 1, Counter);
        }
        else
        {
            u64 Digits = NumDigits(Value);
            if(Digits%2 == 0)
            {
                u64 LeftPart = Value / pow(10, Digits / 2);
                u64 RightPart = Value % (u64)pow(10, Digits / 2);

                Insert(Map, LeftPart, Counter);
                Insert(Map, RightPart, Counter);
            }
            else
            {
                u64 NewValue = Value * 2024;
                Insert(Map, NewValue, Counter);
            }
        }
    }

    ArenaRewind(TempArena, Mark);
}

internal u64
Solve(memory_arena *Arena, string Input, u64 MaxAttempts)
{
    // TODO: Temporary memory should be part of the arena itself.
    memory_arena TempArena = {};

    map Map = ParseInput(Arena, Input);

    for(u32 Attempt = 0; Attempt < MaxAttempts; Attempt++)
    {
        Transform(&TempArena, &Map);
    }

    ArenaFree(&TempArena);

    u64 Result = 0;

    for(u32 Index = 0; Index < Map.Length; Index++)
    {
        Result += Map.Values[Index];
    }

    return(Result);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    return(Solve(Arena, Input, 25));
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    return(Solve(Arena, Input, 75));
}

solution Solution11 =
{
    SolvePartOne,
    SolvePartTwo,
};
