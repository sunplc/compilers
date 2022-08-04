
/*
 * Deterministic finite automaton's state
 */

#ifndef _DFA_STATE_H_INCLUDED_
#define _DFA_STATE_H_INCLUDED_


enum dfa_state {
    state_initial,

    state_keyword_int1,
    state_keyword_int2,
    state_keyword_int3,

    state_id,

    state_int_literal,

    state_plus,
    state_minus,
    state_star,
    state_slash,
    state_assignment,

    state_left_paren,
    state_right_paren,

    state_semicolon,
};

#endif /* _DFA_STATE_H_INCLUDED_ */
