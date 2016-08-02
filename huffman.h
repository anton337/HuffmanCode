#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

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

#endif

