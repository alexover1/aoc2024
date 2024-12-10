/* ========================================================================
$File: day09.cpp $
$Date: 12/09/24 $
$Revision: $
$Creator: Alex Overstreet $
$Notice: (C) Copyright 2024 by Alex Overstreet. All Rights Reserved. $
======================================================================== */

#include "aoc.h"

struct block_value
{
    s32 FileID = -1;
    s32 Length;
};

struct block
{
    block *Next;
    block *Previous;
    block_value Value;
};

struct fs
{
    memory_arena *Arena;

    block *Begin = NULL;
    block *End = NULL;

    b32 AllowFragmentation = true;
};

internal void
Append(fs *Fs, block_value Value)
{
    block *Block = (block *) ArenaAlloc(Fs->Arena, sizeof(block));

    Block->Next = NULL;
    Block->Previous = Fs->End;
    Block->Value = Value;

    if(Fs->End)
    {
        Fs->End->Next = Block;
        Fs->End = Block;
    }
    else
    {
        Fs->Begin = Block;
        Fs->End = Block;
    }
}

internal void
InsertAfter(fs *Fs, block *Block, block_value Value)
{
    block *NewBlock = (block *) ArenaAlloc(Fs->Arena, sizeof(block));

    NewBlock->Next = Block->Next;
    NewBlock->Previous = Block;
    NewBlock->Value = Value;

    Block->Next = NewBlock;
}

internal void
RelocateBlock(fs *Fs, block *Begin, block *End)
{
    for(block *Block = Begin; Block != End; Block = Block->Next)
    {
        if(Block->Value.FileID != -1)
        {
            // Nothing to do.
        }
        else if(Block->Value.Length == End->Value.Length)
        {
            Swap(&Block->Value, &End->Value);
            break;
        }
        else if(Block->Value.Length > End->Value.Length)
        {
            s32 Distance = Block->Value.Length - End->Value.Length;
            Block->Value = End->Value;
            End->Value.FileID = -1;

            block_value EmptyBlock = {};
            EmptyBlock.Length = Distance;

            InsertAfter(Fs, Block, EmptyBlock);
            break;
        }
        else if(Block->Value.Length < End->Value.Length && Fs->AllowFragmentation)
        {
            s32 Distance = End->Value.Length - Block->Value.Length;
            Block->Value.FileID = End->Value.FileID;
            End->Value.Length = Distance;

            block_value EmptyBlock = {};
            EmptyBlock.Length = Block->Value.Length;

            InsertAfter(Fs, End, EmptyBlock);
        }
    }
}

internal void
CompactFS(fs *Fs)
{
    block *Begin = Fs->Begin;
    block *End = Fs->End;

    while(Begin != End)
    {
        if(Begin->Value.FileID != -1)
        {
            Begin = Begin->Next;
        }
        else if(End->Value.FileID == -1)
        {
            End = End->Previous;
        }
        else
        {
            RelocateBlock(Fs, Begin, End);
            End = End->Previous;
        }
    }
}

internal u64
Checksum(fs *Fs)
{
    u64 Result = 0;
    u64 BlockPos = 0;
    for(block *Block = Fs->Begin; Block; Block = Block->Next)
    {
        for(s32 Index = 0; Index < Block->Value.Length; Index++)
        {
            if(Block->Value.FileID != -1)
            {
                Result += BlockPos * Block->Value.FileID;
            }
            BlockPos++;
        }
    }
    return(Result);
}

internal fs
ParseInput(memory_arena *Arena, string Input)
{
    Input = TrimSpace(Input);

    fs Fs = {};
    Fs.Arena = Arena;

    for(u32 Index = 0; Index < Input.Length; Index++)
    {
        block_value Block = {};
        Block.Length = Input.Data[Index] - '0';

        if(Index%2 == 0)
        {
            Block.FileID = Index / 2;
        }

        Append(&Fs, Block);
    }

    return(Fs);
}

internal u64
SolvePartOne(memory_arena *Arena, string Input)
{
    fs Fs = ParseInput(Arena, Input);

    CompactFS(&Fs);

    u64 Result = Checksum(&Fs);

    return(Result);
}

internal u64
SolvePartTwo(memory_arena *Arena, string Input)
{
    fs Fs = ParseInput(Arena, Input);
    Fs.AllowFragmentation = false;

    CompactFS(&Fs);

    u64 Result = Checksum(&Fs);

    return(Result);
}

solution Solution09 =
{
    SolvePartOne,
    SolvePartTwo,
};
