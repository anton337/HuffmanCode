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
    typedef         std::map < char , std::vector < int > >             huffman_code_type     ;


public:

    //***************************************************************************************//
    //  Function:       HuffmanCode
    //  Description:    Constructor for this class
    //***************************************************************************************//
    HuffmanCode (    histogram_type      const & p_histogram 
                , huffman_code_type            & p_huffman_code
                )
    {
        _impl_construct_huffman_code_ ( p_histogram 
                                      , p_huffman_code
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
        HuffmanNode * m_O_node      ;
        HuffmanNode * m_I_node      ;
        char          m_value       ;
        float         m_frequency   ;

    public:

        //***************************************************************************************//
        //  Function:       HuffmanNode
        //  Description:    Constructor for this class
        //***************************************************************************************//
        HuffmanNode ( char      p_value 
                    , float     p_frequency 
                    )
        : m_O_node    ( NULL                )
        , m_I_node    ( NULL                )
        , m_value     ( p_value             )
        , m_frequency ( 1.0 / p_frequency   )
        {
    
        }

        //***************************************************************************************//
        //  Function:       HuffmanNode
        //  Description:    Constructor for this class
        //***************************************************************************************//
        HuffmanNode ( HuffmanNode * p_O_node 
                    , HuffmanNode * p_I_node 
                    )
        : m_O_node      ( p_O_node      )
        , m_I_node      ( p_I_node      )
        , m_value       ( '*'           )
        , m_frequency   ( 1.0 / ( 1.0 / m_O_node->m_frequency + 1.0 / m_I_node->m_frequency ) )
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
        bool operator () ( HuffmanNode const * p_node_A
                         , HuffmanNode const * p_node_B
                         )
        {
            return p_node_A->m_frequency < p_node_B->m_frequency ;
        }
    } huffmanComparator ;

private:
 
    //***************************************************************************************//
    //  Function:       _impl_populate_huffman_code_
    //  Description:    recursively populates the Huffman tree
    //***************************************************************************************//
    void _impl_populate_huffman_code_ ( HuffmanNode          const * p_parent_node 
                                      , huffman_code_type          & p_huffman_code 
                                      , std::vector < int >          p_address = std::vector < int > ()
                                      )
    {
        if ( p_parent_node -> m_I_node == NULL && p_parent_node -> m_O_node == NULL )
        {
            p_huffman_code [ p_parent_node->m_value ] = p_address;
        }
        else
        {
            std::vector < int > c_address_1 = p_address;
            c_address_1 . push_back ( 1 );
            _impl_populate_huffman_code_ ( p_parent_node -> m_I_node
                                         , p_huffman_code
                                         , c_address_1
                                         );
            std::vector < int > c_address_0 = p_address;
            c_address_0 . push_back ( 0 );
            _impl_populate_huffman_code_ ( p_parent_node -> m_O_node 
                                         , p_huffman_code
                                         , c_address_0
                                         );
        }
    }

    //***************************************************************************************//
    //  Function:       _impl_construct_huffman_code_
    //  Description:    constructs the Huffman tree
    //***************************************************************************************//
    void _impl_construct_huffman_code_ ( histogram_type          const & p_histogram 
                                       , huffman_code_type             & p_huffman_code
                                       )
    {
        std::vector < HuffmanNode * > c_nodes;
        histogram_type :: const_iterator it = p_histogram . begin () ;
        while ( it != p_histogram . end () )
        {
            c_nodes . push_back ( new HuffmanNode ( it->first , it->second ) );
            ++it;
        }
        std::make_heap ( c_nodes . begin () 
                       , c_nodes . end () 
                       , huffmanComparator 
                       );
        while ( c_nodes . size () > 1 )
        {
            std::make_heap ( c_nodes . begin () 
                           , c_nodes . end   () 
                           , huffmanComparator 
                           );
            HuffmanNode * I = c_nodes . front ();
            std::pop_heap ( c_nodes . begin () 
                          , c_nodes . end   () 
                          , huffmanComparator 
                          );
            c_nodes . pop_back ();
            HuffmanNode * O = c_nodes . front ();
            std::pop_heap ( c_nodes . begin () 
                          , c_nodes . end   () 
                          , huffmanComparator 
                          );
            c_nodes . pop_back ();
            c_nodes . push_back ( new HuffmanNode ( I , O ) );
            std::push_heap ( c_nodes . begin () 
                           , c_nodes . end   () 
                           );
        }
        _impl_populate_huffman_code_ ( c_nodes[0] , p_huffman_code );
        print_heap ( c_nodes[0] );
    }

public:

    //***************************************************************************************//
    //  Function:       print_heap
    //  Description:    prints out the heap nodes along with normalized frequencies
    //***************************************************************************************//
    void print_heap ( HuffmanNode const * p_node 
                    , std::string         p_prefix = "" 
                    )
    {
        if ( p_node -> m_I_node == NULL && p_node -> m_O_node == NULL )
        {
            std::cout << p_prefix << " - " << (int)p_node->m_value << " " << p_node->m_frequency << std::endl;
        }
        else
        {
            print_heap ( p_node -> m_I_node , p_prefix + "1" );
            print_heap ( p_node -> m_O_node , p_prefix + "0" );
        }
    }

};

#endif

