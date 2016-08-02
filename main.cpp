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

int  encode ( char                                    const * raw_input
            , char                                          * encoded_output
            , std::map < char , std::vector < int > >       & huffman_code
            , std::size_t                                     num_char
            , int                                             num_bits
            )
{
    std::size_t output_index = 0;
    switch ( num_bits )
    {
        case 2:
            {
                for ( std::size_t k(0)
                    ; k < num_char
                    ; ++k
                    )
                {
                    int ind_1 = get_index2(raw_input[k],0);
                    int ind_2 = get_index2(raw_input[k],2);
                    int ind_3 = get_index2(raw_input[k],4);
                    int ind_4 = get_index2(raw_input[k],6);
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_1].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_1][i]==1
                                );
                        ++output_index;
                    }
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_2].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_2][i]==1
                                );
                        ++output_index;
                    }
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_3].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_3][i]==1
                                );
                        ++output_index;
                    }
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_4].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_4][i]==1
                                );
                        ++output_index;
                    }
                }
            }
            break;
        case 4:
            {
                for ( std::size_t k(0)
                    ; k < num_char
                    ; ++k
                    )
                {
                    int ind_1 = get_index2(raw_input[k],0);
                    int ind_2 = get_index2(raw_input[k],4);
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_1].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_1][i]==1
                                );
                        ++output_index;
                    }
                    for ( std::size_t i(0)
                        ; i < huffman_code[ind_2].size()
                        ; ++i
                        )
                    {
                        set_bit ( encoded_output[output_index/8]
                                , output_index%8
                                , huffman_code[ind_2][i]==1
                                );
                        ++output_index;
                    }
                }
            }
            break;
        default:
            {

            }
            break;
    }
    return output_index;
}


void decode ( char                                    const * encoded_input
            , char                                          * decoded_output
            , std::map < char , std::vector < int > >       & huffman_code
            , std::size_t                                     num_char
            , int                                             num_bits
            )
{

}

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
        //data[k] = *reinterpret_cast<long*>(&c);
        data[k] = (long)rand();// + 2147483647*(long)rand();
        //data[k] = (long)rand() + 2147483647*(long)rand();
        //data[k] = (long)rand() % 100;
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
    construct_huffman_code ( histogram 
                           , huffman_code
                           );
    void* array = &data[0];
    char* carray = reinterpret_cast<char*>(array);
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
    return 0;
}

