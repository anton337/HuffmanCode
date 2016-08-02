#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <math.h>

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

////////////////////////////////////////
//                                    //
// Huffman Code                       //
//                                    //
////////////////////////////////////////
//                                    //
// 0                                  //
// 10                                 //
// 110                                //
// 111                                //
//                                    //
//             /\                     //
//            /  \                    //
//           0   /\                   //
//              /  \                  //
//             10  /\                 //
//                /  \                //
//              110  111              //
//                                    //
////////////////////////////////////////

struct HuffmanNode
{
    HuffmanNode * O;
    HuffmanNode * I;
    char c;
    float freq;
    HuffmanNode ( char _c , float _freq )
    : O    ( NULL   )
    , I    ( NULL   )
    , c    ( _c     )
    , freq ( 1.0/_freq )
    {

    }
    HuffmanNode ( HuffmanNode * o , HuffmanNode * i )
    : O ( o )
    , I ( i )
    , c ( '*' )
    , freq ( 1.0 / ( 1.0/o->freq + 1.0/i->freq ) )
    {

    }
};

struct HuffmanComparator
{
    bool operator () ( HuffmanNode const * a , HuffmanNode const * b )
    {
        return a->freq < b->freq;
    }
} huffmanComparator ;

void print_heap ( HuffmanNode const * N , std::string prefix = "" )
{
    if ( N -> I == NULL && N -> O == NULL )
    {
        std::cout << prefix << " - " << (int)N->c << " " << N->freq << std::endl;
    }
    else
    {
        print_heap ( N -> I , prefix+"1" );
        print_heap ( N -> O , prefix+"0" );
    }
}

void populate_huffman_code ( HuffmanNode const * N 
                           , std::map < char , std::vector < int > > & huffman_code 
                           , std::vector < int > address = std::vector < int > ()
                           )
{
    if ( N -> I == NULL && N -> O == NULL )
    {
        huffman_code [ N->c ] = address;
    }
    else
    {
        std::vector < int > address_1 = address;
        address_1 . push_back ( 1 );
        populate_huffman_code ( N -> I 
                              , huffman_code
                              , address_1
                              );
        std::vector < int > address_0 = address;
        address_0 . push_back ( 0 );
        populate_huffman_code ( N -> O 
                              , huffman_code
                              , address_0
                              );
    }
}

void construct_huffman_code ( std::map < char , long >                const & histogram 
                            , std::map < char , std::vector < int > >       & huffman_code
                            )
{
    std::vector < HuffmanNode * > nodes;
    std::map < char , long > :: const_iterator it = histogram . begin () ;
    while ( it != histogram . end () )
    {
        nodes . push_back ( new HuffmanNode ( it->first , it->second ) );
        ++it;
    }
    std::make_heap ( nodes . begin () , nodes . end () , huffmanComparator );
    while ( nodes . size () > 1 )
    {
        std::make_heap ( nodes . begin () , nodes . end () , huffmanComparator );
        HuffmanNode * I = nodes . front ();
        std::pop_heap ( nodes . begin () , nodes . end () , huffmanComparator );
        nodes . pop_back ();
        HuffmanNode * O = nodes . front ();
        std::pop_heap ( nodes . begin () , nodes . end () , huffmanComparator );
        nodes . pop_back ();
        nodes . push_back ( new HuffmanNode ( I , O ) );
        std::push_heap ( nodes . begin () , nodes . end () );
    }
    populate_huffman_code ( nodes[0] , huffman_code );
    print_heap ( nodes[0] );
}

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

#define NUM_TRANSITIONS 2
 
int getNextState ( std::vector<int> const & pat
                 , int M
                 , int state
                 , int x
                 )
{
    // If the character c is same as next character in pattern,
    // then simply increment state
    if (state < M && x == pat[state])
        return state+1;
 
    int ns, i;  // ns stores the result which is next state
 
    // ns finally contains the longest prefix which is also suffix
    // in "pat[0..state-1]c"
 
    // Start from the largest possible value and stop when you find
    // a prefix which is also suffix
    for (ns = state; ns > 0; ns--)
    {
        if(pat[ns-1] == x)
        {
            for(i = 0; i < ns-1; i++)
            {
                if (pat[i] != pat[state-ns+1+i])
                    break;
            }
            if (i == ns-1)
                return ns;
        }
    }
 
    return 0;
}
 
/* This function builds the TF table which represents Finite Automata for a
   given pattern  */
void computeTF ( std::vector<int> const & pat
               , int M
               , int ** TF
               )
{
    int state, x;
    for (state = 0; state <= M; ++state)
        for (x = 0; x < NUM_TRANSITIONS; ++x)
           TF[state][x] = getNextState(pat, M,  state, x);
}
 
/* Prints all occurrences of pat in txt */
void search ( std::vector<std::vector<int> > const & patterns
            , std::vector<int>               const & txt
            )
{
    int N = txt.size();

    int M[patterns.size()];
    for ( int k(0)
        ; k < patterns.size()
        ; ++k
        )
    {
        M[k] = patterns[k].size();
    }
 
    int *** TF = new int**[patterns.size()];
 
    for ( int k(0)
        ; k < patterns.size()
        ; ++k
        )
    {
        TF[k] = new int*[M[k]+1];
        for ( int i(0)
            ; i < M[k]+1
            ; ++i
            )
        {
            TF[k][i] = new int[NUM_TRANSITIONS];
        }
        computeTF(patterns[k], M[k], TF[k]);
    }
 
    // Process txt over FA.
    int state[patterns.size()];
    for ( int k(0)
        ; k < patterns.size()
        ; ++k
        )
    {
        state[k] = 0;
    }
    for ( int i(0)
        ; i < N
        ; i++
        )
    {
        for ( int k(0)
            ; k < patterns.size()
            ; ++k
            )
        {
            state[k] = TF[k][state[k]][txt[i]];
            if (state[k] == M[k])
            {
                printf ("pattern %d found at index %d\n", k, i-M[k]+1);
            }
        }
    }
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
                for ( int k(0)
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
        case 3:
            {
                data.resize((data.size()/3+1)*3);
                void* array = &data[0];
                char* carray = reinterpret_cast<char*>(array);
                for ( int b(0)
                    ; b < bits
                    ; ++b
                    )
                {
                    histogram[(char)b] = 1;
                }
                for ( int k(0)
                    ; k*3 < data.size()
                    ; ++k
                    )
                {
                    for ( int i(0)
                        ; i < 24
                        ; i += 3
                        )
                    {
                        {
                            int ind1   = get_index3(carray[k*24  +i],0);
                            int ind2   = get_index3(carray[k*24  +i],3);
                            int ind3_a = get_index2(carray[k*24  +i],6);
                            int ind3_b = get_index1(carray[k*24+1+i],0);
                            int ind3 = (ind3_a << 1) + ind3_b;
                            histogram[(char)ind1]++;
                            histogram[(char)ind2]++;
                            histogram[(char)ind3]++;
                        }
                        {
                            int ind1   = get_index3(carray[k*24+1+i],1);
                            int ind2   = get_index3(carray[k*24+1+i],4);
                            int ind3_a = get_index1(carray[k*24+1+i],7);
                            int ind3_b = get_index2(carray[k*24+2+i],0);
                            int ind3 = (ind3_a << 2) + ind3_b;
                            histogram[(char)ind1]++;
                            histogram[(char)ind2]++;
                            histogram[(char)ind3]++;
                        }
                        {
                            int ind1   = get_index3(carray[k*24+2+i],2);
                            int ind2   = get_index3(carray[k*24+2+i],5);
                            histogram[(char)ind1]++;
                            histogram[(char)ind2]++;
                        }
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
                for ( int k(0)
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

