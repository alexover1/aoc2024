/* ========================================================================
$File: day11.cpp $
$Date: 12/11/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

#define U32_MAX 0xffffffffu
#define U64_MAX 0xffffffffffffffffull

struct node
{
    node *Next;
    node *Previous;
    u64 Value;
};

struct node_list
{
    memory_arena *Arena;

    node *Begin;
    node *End;

    u64 Length;
};

internal void
Append(node_list *Nodes, u64 Value)
{
    node *Node = (node *) ArenaAlloc(Nodes->Arena, sizeof(node));

    Node->Next = NULL;
    Node->Previous = Nodes->End;
    Node->Value = Value;

    if(Nodes->End)
    {
        Nodes->End->Next = Node;
        Nodes->End = Node;
    }
    else
    {
        Nodes->Begin = Node;
        Nodes->End = Node;
    }

    Nodes->Length++;
}

internal void
InsertAfter(node_list *Nodes, node *Node, u64 Value)
{
    node *NewNode = (node *) ArenaAlloc(Nodes->Arena, sizeof(node));

    NewNode->Next = Node->Next;
    NewNode->Previous = Node;
    NewNode->Value = Value;

    Node->Next = NewNode;

    Nodes->Length++;
}

internal node_list
ParseInput(memory_arena *Arena, string Input)
{
    node_list Nodes = {};
    Nodes.Arena = Arena;

    Input = TrimLeft(Input);

    while(Input.Length > 0)
    {
        u64 Value = ChopU64(&Input);
        Append(&Nodes, Value);

        Input = TrimLeft(Input);
    }

    return(Nodes);
}

internal u64
NumDigits(u64 X)
{
    return((X == 0) ? 1 : (u64)log10(X) + 1);
}

internal void
Transform(node_list *Nodes)
{
    for(node *Node = Nodes->Begin; Node; Node = Node->Next)
    {
        if(Node->Value == 0)
        {
            Node->Value = 1;
        }
        else
        {
            u64 Digits = NumDigits(Node->Value);
            if(Digits%2 == 0)
            {
                u64 LeftPart = Node->Value / pow(10, Digits / 2);
                u64 RightPart = Node->Value % (u64)pow(10, Digits / 2);

                Node->Value = LeftPart;
                InsertAfter(Nodes, Node, RightPart);

                Node = Node->Next;
            }
            else
            {
                Node->Value *= 2024;
            }
        }
    }
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    node_list Nodes = ParseInput(Arena, Input);

    for(u32 Attempt = 0; Attempt < 25; Attempt++)
    {
        Transform(&Nodes);
    }

    return(Nodes.Length);
}

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
ParseInput2(memory_arena *Arena, string Input)
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

internal map
Transform2(map Map)
{
    map TempMap = {};
    TempMap.Arena = Map.Arena;

    for(u32 Index = 0; Index < Map.Length; Index++)
    {
        u64 Value = Map.Keys[Index];
        u64 Counter = Map.Values[Index];

        if(Value == 0)
        {
            Insert(&TempMap, 1, Counter);
        }
        else
        {
            u64 Digits = NumDigits(Value);
            if(Digits%2 == 0)
            {
                u64 LeftPart = Value / pow(10, Digits / 2);
                u64 RightPart = Value % (u64)pow(10, Digits / 2);

                Insert(&TempMap, LeftPart, Counter);
                Insert(&TempMap, RightPart, Counter);
            }
            else
            {
                u64 NewValue = Value * 2024;
                Insert(&TempMap, NewValue, Counter);
            }
        }
    }

    return(TempMap);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    map Map = ParseInput2(Arena, Input);

    for(u32 Attempt = 0; Attempt < 75; Attempt++)
    {
        Map = Transform2(Map);
    }

    u64 Result = 0;

    for(u32 Index = 0; Index < Map.Length; Index++)
    {
        Result += Map.Values[Index];
    }

    return(Result);
}

solution Solution11 =
{
    SolvePartOne,
    SolvePartTwo,
};
