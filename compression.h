#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <map>
#include <vector>
#include <algorithm>

#include "bits.h"

#include "finite_automata.h"

int  encode ( char                                    const * raw_input
            , char                                          * encoded_output
            , std::map < char , std::vector < int > >       & huffman_code
            , std::size_t                                     num_char
            , std::size_t                                     num_bits
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
                    int ind_1 = get_index4(raw_input[k],0);
                    int ind_2 = get_index4(raw_input[k],4);
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
            , std::size_t                                     original_size
            , std::size_t                                     num_bits
            )
{
    
    std::vector<std::vector<int> > patterns;
    std::map<int,int> index_map;
    std::map < char , std::vector < int > > :: const_iterator it = huffman_code . begin ();
    std::map < int , std::vector < std::pair < char , std::vector < int > > > > sort_map;
    while(it!=huffman_code.end())
    {
        sort_map [ it->second . size () ] . push_back ( std::pair < char , std::vector < int > > ( it->first , it->second ) );
        ++it;
    }
    std::map < int , std::vector < std::pair < char , std::vector < int > > > > :: const_reverse_iterator sort_it = sort_map . rbegin ();
    index_map[-1] = -1;
    int ind = 0;
    while(sort_it!=sort_map.rend())
    {
        for ( std::size_t k(0)
            ; k < sort_it -> second . size ()
            ; ++k
            )
        {
            index_map[ind] = (int)sort_it -> second [ k ] . first;
            patterns . push_back ( sort_it -> second [ k ] . second );
            ++ind;
        }
        ++sort_it;
    }
    std::size_t output_index = 0;
    FiniteAutomata<2> FA( patterns );
    for ( std::size_t k(0)
        ; k < num_char
        && output_index/8 < original_size
        ; ++k
        )
    {
        int ind_1 = get_bit(encoded_input[k],0);
        int ind_2 = get_bit(encoded_input[k],1);
        int ind_3 = get_bit(encoded_input[k],2);
        int ind_4 = get_bit(encoded_input[k],3);
        int ind_5 = get_bit(encoded_input[k],4);
        int ind_6 = get_bit(encoded_input[k],5);
        int ind_7 = get_bit(encoded_input[k],6);
        int ind_8 = get_bit(encoded_input[k],7);
        int ret_1 = FA ( ind_1 );
        int ret_2 = FA ( ind_2 );
        int ret_3 = FA ( ind_3 );
        int ret_4 = FA ( ind_4 );
        int ret_5 = FA ( ind_5 );
        int ret_6 = FA ( ind_6 );
        int ret_7 = FA ( ind_7 );
        int ret_8 = FA ( ind_8 );
        switch ( num_bits )
        {
            case 2:
                {
                    if ( index_map [ ret_1 ] >= 0 ) 
                    { 

                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_1 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_2 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_2 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_3 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_3 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_4 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_4 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_5 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_5 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_6 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_6 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_7 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_7 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                    if ( index_map [ ret_8 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?((output_index%8>=6)?0xC0:0x30):((output_index%8>=2)?0x0C:0x03) 
                                 , flip2 ( index_map [ ret_8 ] ) 
                                 ) ; 
                        output_index += 2 ; 
                    };
                }
                break;
            case 4:
                {
                    if ( index_map [ ret_1 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_1 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_2 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_2 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_3 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_3 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_4 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_4 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_5 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_5 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_6 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_6 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_7 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_7 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                    if ( index_map [ ret_8 ] >= 0 ) 
                    { 
                        set_bits ( decoded_output[output_index/8] 
                                 , output_index%8 
                                 , (output_index%8>=4)?0xF0:0x0F 
                                 , flip4 ( index_map [ ret_8 ] ) 
                                 );
                        output_index += 4 ; 
                    };
                }
                break;
            default:
                break;
        }
    }
}

#endif 

