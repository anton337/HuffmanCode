#include "compression.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <math.h>

#include "bits.h"

#include "huffman.h"

#include "finite_automata.h"


int main()
{
    srand(time(NULL));
    int size = 10000;
    std::vector < long > data ( size );
    std::map < char , long > histogram;
    for ( int k(0)
        ; k < size
        ; ++k
        )
    {
        data[k] = (long)rand() % 10000;
    }
    int num_bits = 4;
    int bits = pow(2.0,num_bits)+0.001;
    switch ( num_bits )
    {
        case 2:
            {
                void* array = &data[0];
                char* carray = reinterpret_cast<char*>(array);
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    histogram[(char)b] = 0;
                }
                for ( std::size_t k(0)
                    ; k < data.size()
                    ; ++k
                    )
                {
                    for ( int i(0)
                        ; i < 8
                        ; ++i
                        )
                    {
                        int ind1 = get_index2(carray[k*8+i],0);
                        int ind2 = get_index2(carray[k*8+i],2);
                        int ind3 = get_index2(carray[k*8+i],4);
                        int ind4 = get_index2(carray[k*8+i],6);
                        histogram[(char)ind1]++;
                        histogram[(char)ind2]++;
                        histogram[(char)ind3]++;
                        histogram[(char)ind4]++;
                    }
                }
                long total = 0;
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    total += histogram[(char)b];
                }
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    std::cout << b << " " << (double)histogram[(char)b]/(double)total << std::endl;
                }
                std::cout << std::endl;
            }
            break;
        case 4:
            {
                void* array = &data[0];
                char* carray = reinterpret_cast<char*>(array);
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    histogram[(char)b] = 0;
                }
                for ( std::size_t k(0)
                    ; k < data.size()
                    ; ++k
                    )
                {
                    for ( int i(0)
                        ; i < 8
                        ; ++i
                        )
                    {
                        int ind1 = get_index4(carray[k*8+i],0);
                        int ind2 = get_index4(carray[k*8+i],4);
                        histogram[(char)ind1]++;
                        histogram[(char)ind2]++;
                    }
                }
                long total = 0;
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    total += histogram[(char)b];
                }
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    std::cout << b << " " << (double)histogram[(char)b]/(double)total << std::endl;
                }
                std::cout << std::endl;
            }
            break;
        default:
            {

            }
            break;
    }
    std::map < char , std::vector < int > > huffman_code;
    HuffmanCode ( histogram 
                , huffman_code
                );
    long check_index = 64 ;
    long iNUM = *reinterpret_cast<long*>(&data[check_index]);
    void* array = &data[0];
    long original_size = data.size() * 8;
    char* carray = reinterpret_cast<char*>(array);
    for ( std::size_t i(0)
        , j(0)
        ; i < 64
        ; ++i
        )
    {
        for ( std::size_t k(0)
            ; k < 16
            ; ++k
            , ++j
            )
        {
            std::cout << ((get_bit(carray[check_index+8*j],0)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],1)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],2)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],3)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],4)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],5)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],6)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],7)==1)?'.':' ');
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << "NUM : " << iNUM << std::endl;
    std::vector<char> ovec ( sizeof(long)*data.size() );
    char* oarray = &ovec[0];
    int output_index = encode ( carray
                              , oarray
                              , huffman_code
                              , ovec.size()
                              , num_bits
                              );
    std::cout << output_index << " " << ovec.size()*sizeof(long) << std::endl;
    std::cout << 100 * (float)(output_index) / (float)(ovec.size()*sizeof(long)) << "%" << std::endl;
    ovec . resize ( (output_index / 8 + 1) );
    std::cout << 8*data.size() << " " << ovec.size() << std::endl;
    decode ( oarray
           , carray
           , huffman_code
           , ovec.size()
           , original_size
           , num_bits
           );
    long oNUM = *reinterpret_cast<long*>(&carray[check_index*8]);
    for ( std::size_t i(0)
        , j(0)
        ; i < 64
        ; ++i
        )
    {
        for ( std::size_t k(0)
            ; k < 16
            ; ++k
            , ++j
            )
        {
            std::cout << ((get_bit(carray[check_index+8*j],0)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],1)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],2)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],3)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],4)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],5)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],6)==1)?'.':' ');
            std::cout << ((get_bit(carray[check_index+8*j],7)==1)?'.':' ');
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << "NUM : " << oNUM << std::endl;

    return 0;
}

