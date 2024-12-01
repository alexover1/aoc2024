#include <cmath>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cstring>

#define Assert assert
#define Max(A, B) (((A) > (B)) ? (A) : (B))
#define Min(A, B) (((A) > (B)) ? (B) : (A))
#define Abs(A) ((A) < 0 ? -(A) : (A))

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef int8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef s8 b8;
typedef s16 b16;
typedef s32 b32;

typedef float f32;
typedef double f64;

inline char *ShiftArgs(int *ArgCount, char ***Args)
{
    Assert(*ArgCount > 0);

    char *Result = **Args;

    *ArgCount -= 1;
    *Args += 1;

    return(Result);
}

template<typename T>
struct array {
    T  *Data;
    u32 Allocated;
    u32 Length;
};

template<typename T>
inline void CheckSpace(array<T>& Array, u32 Amount)
{
    if(Array.Length + Amount > Array.Allocated)
    {
        u32 NewSize = Array.Allocated > 0 ? Array.Allocated * 2 : 4;
        T *NewData = new T[NewSize];
        memcpy(NewData, Array.Data, Array.Length * sizeof(T));
        if(Array.Data) delete[] Array.Data;
        Array.Data = NewData;
        Array.Allocated = NewSize;
    }
}

template<typename T>
inline void Append(array<T>& Array, T Item)
{
    CheckSpace(Array, 1);
    Array.Data[Array.Length] = Item;
    Array.Length += 1;
}

template<typename T>
inline int Find(array<T>& Array, T Item)
{
    for(u32 Index = 0; Index < Array.Length; Index++)
    {
        if(Array.Data[Index] == Item)
        {
            return (int) Index;
        }
    }
    return -1;
}

template<typename T>
inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

template<typename T>
inline void Sort(array<T>& Array)
{
    if(Array.Length < 2)
    {
        return;
    }

    int InitJ = 0;
    int LastJ = Array.Length - 1;

    for(;;)
    {
        int InitSwap = -1;
        int PrevSwap = -1;

        for(int J = InitJ; J < LastJ; J++)
        {
            if(Array.Data[J] > Array.Data[J+1])
            {
                Swap(Array.Data[J], Array.Data[J+1]);
                PrevSwap = J;
                if(InitSwap == -1)
                {
                    InitSwap = J;
                }
            }
        }

        if(PrevSwap == -1)
        {
            return;
        }

        InitJ = Max(InitSwap-1, 0);
        LastJ = PrevSwap;
    }
}

struct buffer {
    char *Data;
    u32 Length;
};

typedef buffer string;

inline string ReadFileData(const char *FilePath)
{
    string Result = {};

    FILE *File = fopen(FilePath, "rb");
    if(File == NULL) return(Result);

    fseek(File, 0, SEEK_END);

    long Size = ftell(File);
    if(Size < 0) return(Result);

    fseek(File, 0, SEEK_SET);

    Result.Data = new char[Size];
    fread(Result.Data, 1, Size, File);
    if(ferror(File)) return(Result);

    Result.Length = Size;

    return(Result);
}

inline string ChopLeft(string *Buffer, u32 Amount)
{
    if(Amount > Buffer->Length)
    {
        Amount = Buffer->Length;
    }

    string Result = {Buffer->Data, Amount};

    Buffer->Data += Amount;
    Buffer->Length -= Amount;

    return(Result);
}

inline string TrimLeft(string Buffer)
{
    u32 Index = 0;
    while(Index < Buffer.Length && isspace(Buffer.Data[Index]))
    {
        Index += 1;
    }
    return {Buffer.Data + Index, Buffer.Length - Index};
}

inline string ChopBy(string *Buffer, char Delim)
{
    u32 Index = 0;
    while(Index < Buffer->Length && Buffer->Data[Index] != Delim)
    {
        Index += 1;
    }

    string Result = {Buffer->Data, Index};

    if(Index < Buffer->Length)
    {
        Buffer->Length -= Index + 1;
        Buffer->Data  += Index + 1;
    }
    else
    {
        Buffer->Length -= Index;
        Buffer->Data  += Index;
    }

    return(Result);
}

inline u64 ParseU64(string Buffer)
{
    u64 Result = 0;

    for(u32 Index = 0; Index < Buffer.Length; Index++)
    {
        if(Buffer.Data[Index] < '0' || Buffer.Data[Index] > '9')
        {
            break;
        }

        Result = Result * 10 + (Buffer.Data[Index] - '0');
    }

    return(Result);
}
