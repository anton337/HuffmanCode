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

int get_bit(char x,int bit)
{
    return (x >> bit) & 1;
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

