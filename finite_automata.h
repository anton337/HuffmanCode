#ifndef FINITE_AUTOMATA_H
#define FINITE_AUTOMATA_H

#include <vector>

template < int NUM_TRANSITIONS=2 >
class FiniteAutomata
{

private:
    int *** TF;
    int * M;
    int * state;
    int num_patterns;
    std::vector<std::vector<int> > patterns;

public:
    FiniteAutomata()
    {

    }

private:
    ~FiniteAutomata()
    {
        for ( int k(0)
            ; k < num_patterns
            ; ++k
            )
        {
            for ( int i(0)
                ; i < M[k]+1
                ; ++i
                )
            {
                delete [] TF[k][i];
            }
            delete [] TF[M[k]+1];
        }
        delete [] M;
        delete [] state;
    }
 
    int getNextnstate ( std::vector<int> const & pat
                     , int M
                     , int nstate
                     , int x
                     )
    {
        // If the character c is same as next character in pattern,
        // then simply increment state
        if (nstate < M && x == pat[nstate])
            return nstate+1;
     
        int ns, i;  // ns stores the result which is next nstate
     
        // ns finally contains the longest prefix which is also suffix
        // in "pat[0..state-1]c"
     
        // Start from the largest possible value and stop when you find
        // a prefix which is also suffix
        for (ns = nstate; ns > 0; ns--)
        {
            if(pat[ns-1] == x)
            {
                for(i = 0; i < ns-1; i++)
                {
                    if (pat[i] != pat[nstate-ns+1+i])
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
        int nstate, x;
        for (nstate = 0; nstate <= M; ++nstate)
            for (x = 0; x < NUM_TRANSITIONS; ++x)
               TF[nstate][x] = getNextnstate(pat, M,  nstate, x);
    }

public:
    void init_search ( std::vector<std::vector<int> > const & p_patterns
                     )
    {
  
        patterns = p_patterns;
        num_patterns = patterns.size();
        M = new int[num_patterns];
        for ( int k(0)
            ; k < num_patterns
            ; ++k
            )
        {
            M[k] = patterns[k].size();
        }
     
        TF = new int**[num_patterns];
     
        for ( int k(0)
            ; k < num_patterns
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
     
        state = new int[num_patterns];
        for ( int k(0)
            ; k < num_patterns
            ; ++k
            )
        {
            state[k] = 0;
        }
    }

    int operator () ( int input )
    {
        for ( int k(0)
            ; k < patterns.size()
            ; ++k
            )
        {
            state[k] = TF[k][state[k]][input];
            if (state[k] == M[k])
            {
                return k;
            }
        }
        return -1;
    }

};

#endif

