//***************************************************************************************//
//
//  Author:         Anton Kodochygov
//
//  File:           finite_automata.h
//
//  Description:    Implements a finite automata based searcher for binary inputs.
//
//
//***************************************************************************************//

#ifndef FINITE_AUTOATA_H
#define FINITE_AUTOATA_H

#include <vector>

template < int NUM_TRANSITIONS=2 >
class FiniteAutomata
{

    //***************************************************************************************//
    //  TypeDefs:
    //***************************************************************************************//
private:
    typedef                                             int                  integer_type     ;
    typedef                       std::vector<integer_type>                  pattern_type     ;
    typedef                       std::vector<pattern_type>            pattern_array_type     ;
    typedef                                     std::size_t                     size_type     ;


private:
    //***************************************************************************************//
    //  Member variables:
    //  m_transition_function   -   represents the transition edges
    //  m_final_state           -   terminating state
    //  m_num_patterns          -   total number of patters
    //  m_patterns              -   binary patterns to match
    //
    //***************************************************************************************//
    integer_type                             ***                      m_transition_function   ;
    integer_type                             *                        m_final_state           ;
    integer_type                             *                        m_state                 ;
    integer_type                                                      m_num_patterns          ;
    pattern_array_type                                                m_patterns              ;

public:

    //***************************************************************************************//
    //  Function:       FiniteAutomata
    //  Description:    Constructor for this class
    //***************************************************************************************//
    FiniteAutomata ( pattern_array_type const & p_patterns )
    : m_patterns ( p_patterns )
    {
        _impl_initialize_search_();
    }

    //***************************************************************************************//
    //  Function:       ~FiniteAutomata()
    //  Description:    Destructor for this class
    //***************************************************************************************//
    ~FiniteAutomata()
    {

    }
 
private:

    //***************************************************************************************//
    //  Function:       _impl_get_next_state_
    //  Description:    Computes the next transition state.
    //***************************************************************************************//
    integer_type _impl_get_next_state_ ( pattern_type              const & p_pattern
                                       , integer_type                      p_final_state
                                       , integer_type                      p_state
                                       , integer_type                      p_transition
                                       )
    {
        if (p_state < p_final_state && p_transition == p_pattern[p_state])
        {
            return p_state+1;
        }
     
        integer_type c_state, i; 
     
        for (c_state = p_state; c_state > 0; c_state--)
        {
            if(p_pattern[c_state-1] == p_transition)
            {
                for(i = 0; i < c_state-1; i++)
                {
                    if (p_pattern[i] != p_pattern[p_state-c_state+1+i])
                        break;
                }
                if (i == c_state-1)
                    return c_state;
            }
        }
     
        return 0;
    }
     
    //***************************************************************************************//
    //  Function:       _impl_compute_transition_function_
    //  Description:    Computes the global transition function.
    //***************************************************************************************//
    void _impl_compute_transition_function_ ( pattern_type     const & p_pattern
                                            , integer_type             p_final_state
                                            , integer_type          ** p_transition_function
                                            )
    {
        integer_type p_state, p_transition;
        for (p_state = 0; p_state <= p_final_state; ++p_state)
            for (p_transition = 0; p_transition < NUM_TRANSITIONS; ++p_transition)
                p_transition_function[p_state][p_transition] = 
                    _impl_get_next_state_ ( p_pattern
                                          , p_final_state
                                          , p_state
                                          , p_transition
                                          );
    }

    //***************************************************************************************//
    //  Function:       _impl_reset_transition_state_
    //  Description:    Resets the transition state to the beginning.
    //***************************************************************************************//
    void _impl_reset_trasition_state_()
    {
        for ( integer_type k(0)
            ; k < m_num_patterns
            ; ++k
            )
        {
            m_state[k] = 0;
        }
    }

    //***************************************************************************************//
    //  Function:       _impl_initialize_search_
    //  Description:    Initialize this finite automata.
    //***************************************************************************************//
    void _impl_initialize_search_()
    {
    
        m_num_patterns = m_patterns.size();
        m_final_state = new integer_type[m_num_patterns];
        for ( integer_type k(0)
            ; k < m_num_patterns
            ; ++k
            )
        {
            m_final_state[k] = m_patterns[k].size();
        }
     
        m_transition_function = new integer_type**[m_num_patterns];
     
        for ( integer_type k(0)
            ; k < m_num_patterns
            ; ++k
            )
        {
            m_transition_function[k] = new integer_type*[m_final_state[k]+1];
            for ( integer_type i(0)
                ; i < m_final_state[k]+1
                ; ++i
                )
            {
                m_transition_function[k][i] = new integer_type[NUM_TRANSITIONS];
            }
            _impl_compute_transition_function_(m_patterns[k], m_final_state[k], m_transition_function[k]);
        }
     
        m_state = new integer_type[m_num_patterns];
        _impl_reset_trasition_state_();
    }

public:

    //***************************************************************************************//
    //  Function:       operator ()
    //  Description:    Advances the finite automata.
    //***************************************************************************************//
    integer_type operator () ( integer_type p_input )
    {
        for ( size_type k(0)
            ; k < m_patterns.size()
            ; ++k
            )
        {
            m_state[k] = m_transition_function[k][m_state[k]][p_input];
        }
        integer_type max_state = 0;
        for ( size_type k(0)
            ; k < m_patterns.size()
            ; ++k
            )
        {
            max_state = std::max ( m_state[k] , max_state );
        }
        for ( size_type k(0)
            ; k < m_patterns.size()
            ; ++k
            )
        {
            if (m_state[k] == m_final_state[k] && max_state <= m_final_state[k])
            {
                _impl_reset_trasition_state_();
                return k;
            }
        }
        return -1;
    }

};

#endif

