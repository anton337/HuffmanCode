#ifndef BITS_H
#define BITS_H

#include <iostream>

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

void clear_bits(char & number)
{
    number = '\0';
}

int get_bit(char x,int bit)
{
    return (x >> bit) & 1;
}

void set_bits(char & number,int x,int mask,int bits)
{
    number &= ~mask;
    number |= bits << x;
}

int flip8(char x)
{
    x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
    x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
    x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
    return x;
}

int flip2(char x)
{
    return flip8(x) >> 6;
}

int flip4(char x)
{
    return flip8(x) >> 4;
}


int get_index1(char x,int offset)
{
    return   (int)((x >> offset) & 1) 
           ;
}

int get_index2(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 1) 
           + ((int)((x >> (offset+1)) & 1)     )
           ;
}

int get_index3(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 2) 
           + ((int)((x >> (offset+1)) & 1) << 1)
           + ((int)((x >> (offset+2)) & 1)     )
           ;
}

int get_index4(char x,int offset)
{
    return   ((int)((x >>  offset   ) & 1) << 3) 
           + ((int)((x >> (offset+1)) & 1) << 2)
           + ((int)((x >> (offset+2)) & 1) << 1)
           + ((int)((x >> (offset+3)) & 1)     )
           ;
}

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

