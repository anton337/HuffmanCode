//***************************************************************************************//
//
//  Author:         Anton Kodochygov
//
//  File:           bits.h
//
//  Description:    Implements binary operations.
//
//
//***************************************************************************************//

#ifndef BITS_H
#define BITS_H

#include <iostream>

//***************************************************************************************//
//  Function:       set_bit
//  Description:    Set one specific bit in a char
//***************************************************************************************//
void set_bit(char & number,int x,bool o)
{
    if ( o )
    {
        number |= 1 << x;
    }
    else
    {
        number &= ~(1 << x);
    }
}

//***************************************************************************************//
//  Function:       clear_bits
//  Description:    Set all bits to zero
//***************************************************************************************//
void clear_bits(char & number)
{
    number = '\0';
}

//***************************************************************************************//
//  Function:       get_bit
//  Description:    Get one specific bit from char
//***************************************************************************************//
int get_bit(char x,int bit)
{
    return (x >> bit) & 1;
}

//***************************************************************************************//
//  Function:       set_bits
//  Description:    Sets multiple bits simultaneously
//***************************************************************************************//
void set_bits(char & number,int x,int mask,int bits)
{
    number &= ~mask;
    number |= bits << x;
}

//***************************************************************************************//
//  Function:       flip8
//  Description:    flip 8 bits (abcdefgh) -> (hgfedcba)
//***************************************************************************************//
int flip8(char x)
{
    x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
    x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
    x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
    return x;
}

//***************************************************************************************//
//  Function:       flip2
//  Description:    flip first 2 bits (abXXXXXX) -> (baXXXXXX)
//***************************************************************************************//
int flip2(char x)
{
    return flip8(x) >> 6;
}

//***************************************************************************************//
//  Function:       flip4
//  Description:    flip first 4 bits (abcdXXXX) -> (dcbaXXXX)
//***************************************************************************************//
int flip4(char x)
{
    return flip8(x) >> 4;
}

//***************************************************************************************//
//  Function:       get_index1
//  Description:    get one bit at specific location
//***************************************************************************************//
int get_index1(char x,int offset)
{
    return   (int)((x >> offset) & 1) 
           ;
}

//***************************************************************************************//
//  Function:       get_index2
//  Description:    get two adjacent bits at specific location
//***************************************************************************************//
int get_index2(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 1) 
           + ((int)((x >> (offset+1)) & 1)     )
           ;
}

//***************************************************************************************//
//  Function:       get_index3
//  Description:    get three adjacent bits at specific location
//***************************************************************************************//
int get_index3(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 2) 
           + ((int)((x >> (offset+1)) & 1) << 1)
           + ((int)((x >> (offset+2)) & 1)     )
           ;
}

//***************************************************************************************//
//  Function:       get_index4
//  Description:    get four adjacent bits at specific location
//***************************************************************************************//
int get_index4(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 3) 
           + ((int)((x >> (offset+1)) & 1) << 2)
           + ((int)((x >> (offset+2)) & 1) << 1)
           + ((int)((x >> (offset+3)) & 1)     )
           ;
}

//***************************************************************************************//
//  Function:       print
//  Description:    print all 8 bits of char
//***************************************************************************************//
void print(char x)
{
    for ( int k(0)
        ; k < 8
        ; ++k
        )
    {
        std::cout << get_bit(x,k);
    }
    std::cout << std::endl;
}

#endif

