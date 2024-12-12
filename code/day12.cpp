/* ========================================================================
$File: day12.cpp $
$Date: 12/12/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

#define MapStride 140

struct map
{
    char *Data;
    s32 Width;
    s32 Height;
};

struct point
{
    s32 Row;
    s32 Column;
};

inline bool
operator==(point Left, point Right)
{
    return((Left.Row == Right.Row) && (Left.Column == Right.Column));
}

inline bool
operator!=(point Left, point Right)
{
    return((Left.Row != Right.Row) || (Left.Column != Right.Column));
}

#define _SIZE_T_BITS ((sizeof (size_t)) * 8)

#define RotateLeft(X, N)   (((X) << (N)) | ((X) >> (_SIZE_T_BITS - (N))))
#define RotateRight(X, N)  (((X) >> (N)) | ((X) << (_SIZE_T_BITS - (N))))

internal size_t
HashBytes(void *Bytes_, size_t Length, size_t Seed)
{
    u8 *Bytes = (u8 *) Bytes_;

    if(Length == 8 && sizeof(size_t) == 8)
    {
        size_t Hash = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);
        Hash |= (size_t) (Bytes[4] | (Bytes[5] << 8) | (Bytes[6] << 16) | (Bytes[7] << 24)) << 16 << 16; // Avoid warning if size_t == 4
        Hash ^= Seed;
        Hash = (~Hash) + (Hash << 21);
        Hash ^= RotateRight(Hash,24);
        Hash *= 265;
        Hash ^= RotateRight(Hash,14);
        Hash ^= Seed;
        Hash *= 21;
        Hash ^= RotateRight(Hash,28);
        Hash += (Hash << 31);
        Hash = (~Hash) + (Hash << 18);
        return(Hash);
    }
    else if(Length == 4)
    {
        u32 Hash = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);
        // HASH32-BB  Bob Jenkin's presumably-accidental version of Thomas Wang hash with rotates turned into shifts.
        // Note that converting these back to rotates makes it run a lot slower, presumably due to collisions, so I'm
        // not really sure what's going on.
        Hash ^= Seed;
        Hash = (Hash ^ 61) ^ (Hash >> 16);
        Hash = Hash + (Hash << 3);
        Hash = Hash ^ (Hash >> 4);
        Hash = Hash * 0x27d4eb2d;
        Hash ^= Seed;
        Hash = Hash ^ (Hash >> 15);
        return((((size_t) Hash << 16 << 16) | Hash) ^ Seed);
    }
    else
    {
        Assert(!"Only hash of 4 or 8 bytes is currently supported");
    }
}

#define UNORDERED_SET_INITIAL_CAPACITY 16

struct unordered_set
{
    memory_arena *Arena;
    size_t Seed;
    point *Values;
    bool *Occupied;
    u32 Length;
    u32 Allocated;
};

internal void Insert(unordered_set *Set, point Value);

internal void
ExtendCapacity(unordered_set *Set)
{
    if(Set->Values == NULL)
    {
        Assert(Set->Allocated == 0);
        Assert(Set->Length == 0);

        Assert(Set->Arena);

        Set->Allocated = UNORDERED_SET_INITIAL_CAPACITY;
        Set->Values = (point *) ArenaAlloc(Set->Arena, (sizeof(point) + sizeof(bool)) * Set->Allocated);
        Set->Occupied = (bool *) ((u8 *)Set->Values + sizeof(point) * Set->Allocated);
    }
    else
    {
        unordered_set NewSet = {};
        NewSet.Arena = Set->Arena;
        NewSet.Seed = Set->Seed;

        NewSet.Allocated = Set->Allocated * 2;
        NewSet.Values = (point *) ArenaAlloc(NewSet.Arena, (sizeof(point) + sizeof(bool)) * NewSet.Allocated);
        NewSet.Occupied = (bool *) ((u8 *)NewSet.Values + sizeof(point) * NewSet.Allocated);

        for(u32 Index = 0; Index < Set->Length; Index++)
        {
            Insert(&NewSet, Set->Values[Index]);
        }

        memcpy(Set, &NewSet, sizeof(NewSet));
    }
}

internal void
Insert(unordered_set *Set, point Value)
{
    if(Set->Length >= Set->Allocated)
    {
        ExtendCapacity(Set);
    }

    size_t Hash = HashBytes(&Value, sizeof(Value), Set->Seed);

    u32 Mask = (Set->Allocated - 1);
    size_t Index = Hash & Mask;

    while(Set->Occupied[Index])
    {
        if(Set->Values[Index] == Value)
        {
            return;
        }

        Index = (Index + 1) & Mask;
    }

    Set->Occupied[Index] = true;
    Set->Values[Index] = Value;

    Set->Length++;
}

internal point *
FindValue(unordered_set *Set, s32 Row, s32 Column)
{
    point Value = {Row, Column};
    size_t Hash = HashBytes(&Value, sizeof(Value), Set->Seed);

    u32 Mask = (Set->Allocated - 1);
    size_t Index = Hash & Mask;
    for(u32 I = 0; I < Set->Allocated && Set->Occupied[Index] && Set->Values[Index] != Value; I++)
    {
        Index = (Index + 1) & Mask;
    }

    point *Result = NULL;
    if(Set->Values && Set->Occupied[Index] && Set->Values[Index] == Value)
    {
        Result = &Set->Values[Index];
    }
    return(Result);
}

internal map
ParseInput(memory_arena *Arena, string Input)
{
    map Map = {};
    Map.Data = (char *) ArenaAlloc(Arena, sizeof(char) * MapStride * MapStride);

    while(Input.Length > 0)
    {
        string Line = ChopBy(&Input, '\n');
        Line = TrimSpace(Line);
        if(!Line.Length) continue;

        if(Line.Length > Map.Width)
        {
            Map.Width = Line.Length;
            Assert(Map.Width <= MapStride);
        }

        s32 Row = Map.Height;
        for(u32 Column = 0; Column < Line.Length; Column++)
        {
            Map.Data[Row*MapStride + Column] = Line.Data[Column];
        }

        Map.Height++;
    }

    return(Map);
}

internal void
FillRegion(unordered_set *Region, char AreaLabel, map Map, s32 Row, s32 Column, bool *Visited)
{
    s32 Index = Row*MapStride + Column;

    if(Map.Data[Index] != AreaLabel || Visited[Index])
    {
        return;
    }

    Visited[Index] = true;

    point Point = {Row, Column};
    Insert(Region, Point);

    if(Row > 0) FillRegion(Region, AreaLabel, Map, Row-1, Column, Visited);
    if(Row + 1 < Map.Height) FillRegion(Region, AreaLabel, Map, Row+1, Column, Visited);
    if(Column > 0) FillRegion(Region, AreaLabel, Map, Row, Column-1, Visited);
    if(Column + 1 < Map.Width) FillRegion(Region, AreaLabel, Map, Row, Column+1, Visited);
}

#define MaxRegions 1024

struct regions
{
    unordered_set *Data;
    u32 Count;
};

internal regions
FindAllRegions(memory_arena *Arena, map Map)
{
    regions Regions = {};
    Regions.Data = (unordered_set *) ArenaAlloc(Arena, sizeof(unordered_set) * MaxRegions);

    memory_arena_mark Mark = ArenaSnapshot(Arena);

    bool *Visited  = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);
    bool *Searched = (bool *) ArenaAlloc(Arena, sizeof(bool) * MapStride * MapStride);

    for(s32 Row = 0; Row < Map.Height; Row++)
    {
        for(s32 Column = 0; Column < Map.Width; Column++)
        {
            if(!Searched[Row*MapStride + Column])
            {
                Assert(Regions.Count < MaxRegions);
                unordered_set *Region = &Regions.Data[Regions.Count];
                Regions.Count++;

                Region->Arena = Arena;

                char AreaLabel = Map.Data[Row*MapStride + Column];

                memset(Visited, 0, sizeof(bool) * MapStride * MapStride);
                FillRegion(Region, AreaLabel, Map, Row, Column, Visited);

                for(u32 Index = 0; Index < Region->Allocated; Index++)
                {
                    if(Region->Occupied[Index])
                    {
                        point P = Region->Values[Index];
                        Searched[P.Row*MapStride + P.Column] = true;
                    }
                }
            }
        }
    }

    ArenaRewind(Arena, Mark);

    return(Regions);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);
    regions Regions = FindAllRegions(Arena, Map);

    u64 Result = 0;

    for(u32 RegionIndex = 0; RegionIndex < Regions.Count; RegionIndex++)
    {
        u64 Perimeter = 0;
        unordered_set *Region = &Regions.Data[RegionIndex];

        for(u32 Index = 0; Index < Region->Allocated; Index++)
        {
            if(Region->Occupied[Index])
            {
                point P = Region->Values[Index];

                if((P.Row == 0) || FindValue(Region, P.Row-1, P.Column) == NULL) Perimeter++;
                if((P.Row + 1 == Map.Height) || FindValue(Region, P.Row+1, P.Column) == NULL) Perimeter++;
                if((P.Column == 0) || FindValue(Region, P.Row, P.Column-1) == NULL) Perimeter++;
                if((P.Column + 1 == Map.Width) || FindValue(Region, P.Row, P.Column+1) == NULL) Perimeter++;
            }
        }

        Result += Perimeter * Region->Length;
    }

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    map Map = ParseInput(Arena, Input);
    regions Regions = FindAllRegions(Arena, Map);

    u64 Result = 0;

    for(u32 RegionIndex = 0; RegionIndex < Regions.Count; RegionIndex++)
    {
        u64 Sides = 0;
        unordered_set *Region = &Regions.Data[RegionIndex];

        for(u32 Index = 0; Index < Region->Allocated; Index++)
        {
            if(Region->Occupied[Index])
            {
                point P = Region->Values[Index];
                s32 Row = P.Row;
                s32 Col = P.Column;

                // Outer corners
                Sides += FindValue(Region, Row, Col-1) == NULL && FindValue(Region, Row-1, Col) == NULL;
                Sides += FindValue(Region, Row, Col+1) == NULL && FindValue(Region, Row-1, Col) == NULL;
                Sides += FindValue(Region, Row, Col-1) == NULL && FindValue(Region, Row+1, Col) == NULL;
                Sides += FindValue(Region, Row, Col+1) == NULL && FindValue(Region, Row+1, Col) == NULL;
                // Inner corners
                Sides += FindValue(Region, Row, Col-1) != NULL && FindValue(Region, Row-1, Col) != NULL && FindValue(Region, Row - 1, Col - 1) == NULL;
                Sides += FindValue(Region, Row, Col+1) != NULL && FindValue(Region, Row-1, Col) != NULL && FindValue(Region, Row - 1, Col + 1) == NULL;
                Sides += FindValue(Region, Row, Col-1) != NULL && FindValue(Region, Row+1, Col) != NULL && FindValue(Region, Row + 1, Col - 1) == NULL;
                Sides += FindValue(Region, Row, Col+1) != NULL && FindValue(Region, Row+1, Col) != NULL && FindValue(Region, Row + 1, Col + 1) == NULL;
            }
        }

        Result += Sides * Region->Length;
    }

    return(Result);
}

solution Solution12 =
{
    SolvePartOne,
    SolvePartTwo,
};
