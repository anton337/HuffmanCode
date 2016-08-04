//***************************************************************************************//
//
//  Author:         Anton Kodochygov
//
//  File:           huffman.h
//
//  Description:    Implements Huffman Code.
//
//
//***************************************************************************************//

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

class HuffmanCode
{

    //***************************************************************************************//
    //  TypeDefs:
    //***************************************************************************************//
private:
    typedef                                             int                  integer_type     ;
    typedef                        std::map < char , long >                histogram_type     ; 
    typedef                                     std::size_t                     size_type     ;


public:

    //***************************************************************************************//
    //  Function:       HuffmanCode
    //  Description:    Constructor for this class
    //***************************************************************************************//
    HuffmanCode ( histogram_type                          const & histogram 
                , std::map < char , std::vector < int > >       & huffman_code
                )
    {
        _impl_construct_huffman_code_ ( histogram 
                                      , huffman_code
                                      );
    }

    //***************************************************************************************//
    //  Function:       ~HuffmanCode()
    //  Description:    Destructor for this class
    //***************************************************************************************//
    ~HuffmanCode()
    {

    }

private: 

    //***************************************************************************************//
    //  Struct:         HuffmanNode
    //  Description:    Tree node used for Huffman coding
    //***************************************************************************************//
    struct HuffmanNode
    {

    public:
        HuffmanNode * O;
        HuffmanNode * I;
        char c;
        float freq;

    public:

        //***************************************************************************************//
        //  Function:       HuffmanNode
        //  Description:    Constructor for this class
        //***************************************************************************************//
        HuffmanNode ( char _c , float _freq )
        : O    ( NULL   )
        , I    ( NULL   )
        , c    ( _c     )
        , freq ( 1.0/_freq )
        {
    
        }

        //***************************************************************************************//
        //  Function:       HuffmanNode
        //  Description:    Constructor for this class
        //***************************************************************************************//
        HuffmanNode ( HuffmanNode * o , HuffmanNode * i )
        : O ( o )
        , I ( i )
        , c ( '*' )
        , freq ( 1.0 / ( 1.0/o->freq + 1.0/i->freq ) )
        {
    
        }

    };
    
    //***************************************************************************************//
    //  Struct:         HuffmanComparator
    //  Description:    Comparator for Huffman nodes used for sorting
    //***************************************************************************************//
    struct HuffmanComparator
    {
        //***************************************************************************************//
        //  Function:       operator ()
        //  Description:    less than operator
        //***************************************************************************************//
        bool operator () ( HuffmanNode const * a , HuffmanNode const * b )
        {
            return a->freq < b->freq;
        }
    } huffmanComparator ;

private:
 
    //***************************************************************************************//
    //  Function:       _impl_populate_huffman_code_
    //  Description:    recursively populates the Huffman tree
    //***************************************************************************************//
    void _impl_populate_huffman_code_ ( HuffmanNode const * N 
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
            _impl_populate_huffman_code_ ( N -> I 
                                  , huffman_code
                                  , address_1
                                  );
            std::vector < int > address_0 = address;
            address_0 . push_back ( 0 );
            _impl_populate_huffman_code_ ( N -> O 
                                  , huffman_code
                                  , address_0
                                  );
        }
    }

    //***************************************************************************************//
    //  Function:       _impl_construct_huffman_code_
    //  Description:    constructs the Huffman tree
    //***************************************************************************************//
    void _impl_construct_huffman_code_ ( histogram_type                          const & histogram 
                                       , std::map < char , std::vector < int > >       & huffman_code
                                       )
    {
        std::vector < HuffmanNode * > nodes;
        histogram_type :: const_iterator it = histogram . begin () ;
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
        _impl_populate_huffman_code_ ( nodes[0] , huffman_code );
        print_heap ( nodes[0] );
    }

public:

    //***************************************************************************************//
    //  Function:       print_heap
    //  Description:    prints out the heap nodes along with normalized frequencies
    //***************************************************************************************//
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

};

#endif

