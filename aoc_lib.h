#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cstring>

#define global static
#define local_persist static
#define internal static

#define Assert assert
#define Max(A, B) (((A) > (B)) ? (A) : (B))
#define Min(A, B) (((A) > (B)) ? (B) : (A))
#define Abs(X) ((X) > 0 ? (X) : -(X))

#define ArrayLength(Array) (sizeof((Array))/sizeof((Array)[0]))

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef int8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;

typedef float  f32;
typedef double f64;

typedef uintptr_t umm;
typedef intptr_t  smm;

inline char *ShiftArgs(int *ArgCount, char ***Args)
{
    Assert(*ArgCount > 0);

    char *Result = **Args;

    *ArgCount -= 1;
    *Args += 1;

    return(Result);
}

inline bool IsDigit(char X)
{
    bool Result = ('0' <= X && X <= '9');
    return(Result);
}

inline bool IsSpace(char X)
{
    bool Result = false;

    switch(X)
    {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        {
            Result = true;
        } break;
    }

    return(Result);
}

template<typename T>
struct array
{
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
inline s32 Find(array<T>& Array, T Item)
{
    for(u32 Index = 0; Index < Array.Length; Index++)
    {
        if(Array.Data[Index] == Item)
        {
            return (s32) Index;
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

template<typename T>
inline void SortBy(array<T>& Array, bool (*CmpFn)(T A, T B))
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
            if(CmpFn(Array.Data[J], Array.Data[J+1]))
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

struct buffer
{
    char *Data;
    u32 Length;
};

typedef buffer string;

inline constexpr string operator ""_s(const char *ZString, size_t Length)
{
    return{(char*)ZString, (u32)Length};
}

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

inline u32 StringLength(const char *ZString)
{
    u32 Length = 0;

    while(ZString[Length] != '\0')
    {
        Length += 1;
    }

    return(Length);
}

inline bool StringsAreEqual(const char *DataA, const char *DataB, u32 Length)
{
    bool Result = true;

    for(u32 Index = 0; Index < Length; Index++)
    {
        if(DataA[Index] != DataB[Index])
        {
            Result = false;
            break;
        }
    }

    return(Result);
}

inline bool operator==(string Left, string Right)
{
    bool Result = (Left.Length == Right.Length) && StringsAreEqual(Left.Data, Right.Data, Right.Length);
    return(Result);
}

inline bool operator!=(string Left, string Right)
{
    bool Result = (Left.Length != Right.Length) || !StringsAreEqual(Left.Data, Right.Data, Right.Length);
    return(Result);
}

inline bool operator==(string Left, const char *Right)
{
    u32 Length = StringLength(Right);
    bool Result = (Left.Length == Length) && StringsAreEqual(Left.Data, Right, Length);

    return(Result);
}

inline bool operator!=(string Left, const char *Right)
{
    u32 Length = StringLength(Right);
    bool Result = (Left.Length != Length) && !StringsAreEqual(Left.Data, Right, Length);

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
    while(Index < Buffer.Length && IsSpace(Buffer.Data[Index]))
    {
        Index += 1;
    }
    return {Buffer.Data + Index, Buffer.Length - Index};
}

inline string TrimRight(string Buffer)
{
    u32 Index = 0;
    while(Index < Buffer.Length && IsSpace(Buffer.Data[Buffer.Length - Index - 1]))
    {
        Index += 1;
    }
    return {Buffer.Data, Buffer.Length - Index};
}

inline string TrimSpace(string Buffer)
{
    Buffer = TrimRight(TrimLeft(Buffer));
    return(Buffer);
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

inline string ChopBy(string *Buffer, string Delim)
{
    string Window = {Buffer->Data, Delim.Length};

    u32 Index = 0;
    while(Index + Delim.Length < Buffer->Length && Window != Delim)
    {
        Index += 1;
        Window.Data += 1;
    }

    string Result = {Buffer->Data, Index};

    if(Index + Delim.Length == Buffer->Length)
    {
        Result.Length += Delim.Length;
    }

    Buffer->Data   += Index + Delim.Length;
    Buffer->Length -= Index + Delim.Length;

    return(Result);
}

inline bool HasPrefix(string Buffer, string Prefix)
{
    bool Result = false;

    if(Prefix.Length <= Buffer.Length)
    {
        Result = StringsAreEqual(Buffer.Data, Prefix.Data, Prefix.Length);
    }

    return(Result);
}

inline bool HasPrefix(string Buffer, char Delim)
{
    bool Result = (Buffer.Length > 0 && Buffer.Data[0] == Delim);
    return(Result);
}

inline bool ChopPrefix(string *Buffer, string Prefix)
{
    bool Result = HasPrefix(*Buffer, Prefix);

    if(Result)
    {
        ChopLeft(Buffer, Prefix.Length);
    }

    return(Result);
}

inline u32 ParseU32(string Buffer)
{
    u32 Result = 0;

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

inline u64 ChopU64(string *Buffer)
{
    u64 Result = 0;

    u32 Index = 0;
    while(Index < Buffer->Length && IsDigit(Buffer->Data[Index]))
    {
        Result = Result * 10 + (Buffer->Data[Index] - '0');
        Index += 1;
    }

    ChopLeft(Buffer, Index);

    return(Result);
}

inline s32
S32FromZInternal(const char **AtInit)
{
    s32 Result = 0;

    const char *At = *AtInit;
    while((*At >= '0') &&
          (*At <= '9'))
    {
        Result *= 10;
        Result += (*At - '0');
        ++At;
    }

    *AtInit = At;

    return(Result);
}

inline s32
S32FromZ(const char *At)
{
    const char *Ignored = At;
    s32 Result = S32FromZInternal(&Ignored);
    return(Result);
}

struct format_dest
{
    umm Size;
    char *At;
};

inline void
OutChar(format_dest *Dest, char Value)
{
    if(Dest->Size)
    {
        --Dest->Size;
        *Dest->At++ = Value;
    }
}

inline void
OutChars(format_dest *Dest, const char *Value)
{
    // NOTE(casey): Not particularly speedy, are we?  :P
    while(*Value)
    {
        OutChar(Dest, *Value++);
    }
}

#define ReadVarArgUnsignedInteger(Length, ArgList) ((Length) == 8) ? va_arg(ArgList, u64) : (u64)va_arg(ArgList, u32)
#define ReadVarArgSignedInteger(Length, ArgList) ((Length) == 8) ? va_arg(ArgList, s64) : (s64)va_arg(ArgList, s32)
#define ReadVarArgFloat(Length, ArgList) va_arg(ArgList, f64)

internal char DecChars[] = "0123456789";
internal char LowerHexChars[] = "0123456789abcdef";
internal char UpperHexChars[] = "0123456789ABCDEF";

inline void
U64ToASCII(format_dest *Dest, u64 Value, u32 Base, char *Digits)
{
    Assert(Base != 0);

    char *Start = Dest->At;
    do
    {
        u64 DigitIndex = (Value % Base);
        char Digit = Digits[DigitIndex];
        OutChar(Dest, Digit);

        Value /= Base;
    } while(Value != 0);
    char *End = Dest->At;

    while(Start < End)
    {
        --End;
        char Temp = *End;
        *End = *Start;
        *Start = Temp;
        ++Start;
    }
}

inline void
F64ToASCII(format_dest *Dest, f64 Value, u32 Precision)
{
    if(Value < 0)
    {
        OutChar(Dest, '-');
        Value = -Value;
    }

    u64 IntegerPart = (u64)Value;
    Value -= (f64)IntegerPart;
    U64ToASCII(Dest, IntegerPart, 10, DecChars);

    OutChar(Dest, '.');

    // TODO(casey): Note that this is NOT an accurate way to do this!
    for(u32 PrecisionIndex = 0;
        PrecisionIndex < Precision;
        ++PrecisionIndex)
    {
        Value *= 10.0f;
        u32 Integer = (u32)Value;
        Value -= (f32)Integer;
        OutChar(Dest, DecChars[Integer]);
    }
}

// NOTE(casey): Size returned __DOES NOT__ include the null terminator.
inline umm
FormatStringList(umm DestSize, char *DestInit, const char *Format, va_list ArgList)
{
    format_dest Dest = {DestSize, DestInit};
    if(Dest.Size)
    {
        const char *At = Format;
        while(At[0])
        {
            if(*At == '%')
            {
                ++At;

                b32 ForceSign = false;
                b32 PadWithZeros = false;
                b32 LeftJustify = false;
                b32 PostiveSignIsBlank = false;
                b32 AnnotateIfNotZero = false;

                b32 Parsing = true;

                //
                // NOTE(casey): Handle the flags
                //
                Parsing = true;
                while(Parsing)
                {
                    switch(*At)
                    {
                        case '+': {ForceSign = true;} break;
                        case '0': {PadWithZeros = true;} break;
                        case '-': {LeftJustify = true;} break;
                        case ' ': {PostiveSignIsBlank = true;} break;
                        case '#': {AnnotateIfNotZero = true;} break;
                        default: {Parsing = false;} break;
                    }

                    if(Parsing)
                    {
                        ++At;
                    }
                }

                //
                // NOTE(casey): Handle the width
                //
                b32 WidthSpecified = false;
                s32 Width = 0;
                if(*At == '*')
                {
                    Width = va_arg(ArgList, int);
                    WidthSpecified = true;
                    ++At;
                }
                else if((*At >= '0') && (*At <= '9'))
                {
                    Width = S32FromZInternal(&At);
                    WidthSpecified = true;
                }

                //
                // NOTE(casey): Handle the precision
                //
                b32 PrecisionSpecified = false;
                s32 Precision = 0;
                if(*At == '.')
                {
                    ++At;

                    if(*At == '*')
                    {
                        Precision = va_arg(ArgList, int);
                        PrecisionSpecified = true;
                        ++At;
                    }
                    else if((*At >= '0') && (*At <= '9'))
                    {
                        Precision = S32FromZInternal(&At);
                        PrecisionSpecified = true;
                    }
                    else
                    {
                        Assert(!"Malformed precision specifier!");
                    }
                }

                // TODO(casey): Right now our routine doesn't allow non-specified
                // precisions, so we just set non-specified precisions to a specified value
                if(!PrecisionSpecified)
                {
                    Precision = 6;
                }

                //
                // NOTE(casey): Handle the length
                //
                u32 IntegerLength = 4;
                u32 FloatLength = 8;
                // TODO(casey): Actually set different values here!
                if((At[0] == 'h') && (At[1] == 'h'))
                {
                    At += 2;
                }
                else if((At[0] == 'l') && (At[1] == 'l'))
                {
                    At += 2;
                }
                else if(*At == 'h')
                {
                    ++At;
                }
                else if(*At == 'l')
                {
                    IntegerLength = 8;
                    ++At;
                }
                else if(*At == 'j')
                {
                    ++At;
                }
                else if(*At == 'z')
                {
                    ++At;
                }
                else if(*At == 't')
                {
                    ++At;
                }
                else if(*At == 'L')
                {
                    ++At;
                }

                char TempBuffer[64];
                char *Temp = TempBuffer;
                format_dest TempDest = {ArrayLength(TempBuffer), Temp};
                const char *Prefix = "";
                b32 IsFloat = false;

                switch(*At)
                {
                    case 'd':
                    case 'i':
                    {
                        s64 Value = ReadVarArgSignedInteger(IntegerLength, ArgList);
                        b32 WasNegative = (Value < 0);
                        if(WasNegative)
                        {
                            Value = -Value;
                        }
                        U64ToASCII(&TempDest, (u64)Value, 10, DecChars);

                        // TODO(casey): Make this a common routine once floating
                        // point is available.
                        if(WasNegative)
                        {
                            Prefix = "-";
                        }
                        else if(ForceSign)
                        {
                            Assert(!PostiveSignIsBlank); // NOTE(casey): Not a problem here, but probably shouldn't be specified?
                            Prefix = "+";
                        }
                        else if(PostiveSignIsBlank)
                        {
                            Prefix = " ";
                        }
                    } break;

                    case 'u':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 10, DecChars);
                    } break;

                    case 'm':
                    {
                        // TODO(casey): Put in a fractional thing here...
                        umm Value = va_arg(ArgList, umm);
                        const char *Suffix = "b ";
                        if(Value >= Gigabytes(1))
                        {
                            Suffix = "gb";
                            Value = (Value + Gigabytes(1) - 1) / Gigabytes(1);
                        }
                        else if(Value >= Megabytes(1))
                        {
                            Suffix = "mb";
                            Value = (Value + Megabytes(1) - 1) / Megabytes(1);
                        }
                        else if(Value >= Kilobytes(1))
                        {
                            Suffix = "kb";
                            Value = (Value + Kilobytes(1) - 1) / Kilobytes(1);
                        }
                        U64ToASCII(&TempDest, Value, 10, DecChars);
                        OutChars(&TempDest, Suffix);
                    } break;

                    case 'o':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 8, DecChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0";
                        }
                    } break;

                    case 'x':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 16, LowerHexChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0x";
                        }
                    } break;

                    case 'X':
                    {
                        u64 Value = ReadVarArgUnsignedInteger(IntegerLength, ArgList);
                        U64ToASCII(&TempDest, Value, 16, UpperHexChars);
                        if(AnnotateIfNotZero && (Value != 0))
                        {
                            Prefix = "0X";
                        }
                    } break;

                    // TODO(casey): Support other kinds of floating point prints
                    // (right now we only do basic decimal output)
                    case 'f':
                    case 'F':
                    case 'e':
                    case 'E':
                    case 'g':
                    case 'G':
                    case 'a':
                    case 'A':
                    {
                        f64 Value = ReadVarArgFloat(FloatLength, ArgList);
                        F64ToASCII(&TempDest, Value, Precision);
                        IsFloat = true;
                    } break;

                    case 'c':
                    {
                        int Value = va_arg(ArgList, int);
                        OutChar(&TempDest, (char)Value);
                    } break;

                    case 's':
                    {
                        char *String = va_arg(ArgList, char *);

                        // TODO(casey): Obey precision, width, etc.

                        Temp = String;
                        if(PrecisionSpecified)
                        {
                            TempDest.Size = 0;
                            for(char *Scan = String;
                                *Scan && (TempDest.Size < Precision);
                                ++Scan)
                            {
                                ++TempDest.Size;
                            }
                        }
                        else
                        {
                            TempDest.Size = StringLength(String);
                        }
                        TempDest.At = String + TempDest.Size;
                    } break;

                    case 'S':
                    {
                        string String = va_arg(ArgList, string);

                        // TODO(casey): Obey precision, width, etc.

                        Temp = (char *)String.Data;
                        TempDest.Size = String.Length;
                        if(PrecisionSpecified && (TempDest.Size > Precision))
                        {
                            TempDest.Size = Precision;
                        }
                        TempDest.At = Temp + TempDest.Size;
                    } break;

                    case 'p':
                    {
                        void *Value = va_arg(ArgList, void *);
                        U64ToASCII(&TempDest, *(umm *)&Value, 16, LowerHexChars);
                    } break;

                    case 'n':
                    {
                        int *TabDest = va_arg(ArgList, int *);
                        *TabDest = (int)(Dest.At - DestInit);
                    } break;

                    case '%':
                    {
                        OutChar(&Dest, '%');
                    } break;

                    default:
                    {
                        Assert(!"Unrecognized format specifier");
                    } break;
                }

                if(TempDest.At - Temp)
                {
                    smm UsePrecision = Precision;
                    if(IsFloat || !PrecisionSpecified)
                    {
                        UsePrecision = (TempDest.At - Temp);
                    }

                    smm PrefixLength = StringLength(Prefix);
                    smm UseWidth = Width;
                    smm ComputedWidth = UsePrecision + PrefixLength;
                    if(UseWidth < ComputedWidth)
                    {
                        UseWidth = ComputedWidth;
                    }

                    if(PadWithZeros)
                    {
                        Assert(!LeftJustify); // NOTE(casey): Not a problem, but no way to do it?
                        LeftJustify = false;
                    }

                    if(!LeftJustify)
                    {
                        while(UseWidth > (UsePrecision + PrefixLength))
                        {
                            OutChar(&Dest, PadWithZeros ? '0' : ' ');
                            --UseWidth;
                        }
                    }

                    for(const char *Pre = Prefix;
                        *Pre && UseWidth;
                        ++Pre)
                    {
                        OutChar(&Dest, *Pre);
                        --UseWidth;
                    }

                    if(UsePrecision > UseWidth)
                    {
                        UsePrecision = UseWidth;
                    }
                    while(UsePrecision > (TempDest.At - Temp))
                    {
                        OutChar(&Dest, '0');
                        --UsePrecision;
                        --UseWidth;
                    }
                    while(UsePrecision && (TempDest.At != Temp))
                    {
                        OutChar(&Dest, *Temp++);
                        --UsePrecision;
                        --UseWidth;
                    }

                    if(LeftJustify)
                    {
                        while(UseWidth)
                        {
                            OutChar(&Dest, ' ');
                            --UseWidth;
                        }
                    }
                }

                if(*At)
                {
                    ++At;
                }
            }
            else
            {
                OutChar(&Dest, *At++);
            }
        }

        if(Dest.Size)
        {
            Dest.At[0] = 0;
        }
        else
        {
            Dest.At[-1] = 0;
        }
    }

    umm Result = Dest.At - DestInit;
    return(Result);
}

// TODO(casey): Eventually, make this return a string struct
inline umm
FormatString(umm DestSize, char *Dest, const char *Format, ...)
{
    va_list ArgList;

    va_start(ArgList, Format);
    umm Result = FormatStringList(DestSize, Dest, Format, ArgList);
    va_end(ArgList);

    return(Result);
}

// TODO(alex): Remove the dependency on libc just for fwrite?
inline void
PrintMessage(const char *Format, ...)
{
    va_list ArgList;

    char OutputBuffer[4096];

    va_start(ArgList, Format);
    umm BytesWritten = FormatStringList(sizeof(OutputBuffer), OutputBuffer, Format, ArgList);
    va_end(ArgList);

    fwrite(OutputBuffer, 1, BytesWritten, stdout);
}
