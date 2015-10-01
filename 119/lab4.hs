--
-- lab4.hs
-- Implementing (deterministic) finite state machines in Haskell
--

import Data.List
import Data.List (sort)

-- Finite state machines where the states are of type a. For this lab, this
-- will be Int, but in the future we will want to construct (e.g.) the Union
-- machine, where the states will be pairs of states.
--             Q,  q0,  F,  delta
type FSM a = ([a], a,  [a], [(a,Char,a)])

-- Some helper functions for extracting the parts of a machine (although 
-- usually it will be better to use pattern matching for this).
states :: FSM a -> [a]
states (qs,_,_,_) = qs

start :: FSM a -> a
start (_,q0,_,_) = q0

finals :: FSM a -> [a]
finals (_,_,fs,_) = fs

delta :: FSM a -> [(a,Char,a)]
delta (_,_,_,ds) = ds

distinct :: Ord a => [a] -> [a]
distinct = dedup . sort
    where
        dedup [] = []
        dedup [x] = [x]
        dedup (x1:x2:xs) | x1 == x2   = dedup (x2:xs)
                         | otherwise   = x1 : dedup (x2:xs)

within:: Ord a => a -> [a] -> Bool
within x [] = False
within x (a:as) = if (x == a)
				  then True
				  else within x as
				   

unique :: (Ord a) => [a] -> Bool
unique as = (distinct as) == as

-------------------------------------------------------------------------------
-- Begin lab

-- lookup M q a
-- In a general finite state machine, the transition function can actually 
-- return a *set* of states. For a DFA, this set will always have size 1, but
-- we'll need the general version later, so we write it now. `lookup M q a`
-- returns the list of states that we would move to if we were in state q, and
-- we read a character `a`. Thus, lookup will need to search through the 
-- delta component of the machine for all triples that have `q` as its first 
-- element and `a` as its second, and then return the list of all their
-- third elements. 
lookupAll :: (Eq a) => (a,Char) -> [(a,Char,a)] -> [a]
lookupAll (q,a) dlt = [s2 | (s1,c,s2) <- dlt, q == s1, c == a]

-- Checks whether a finite state machine (qs, s, fs, ts) is correct/complete:
-- (1) States qs are unique (no duplicates, qs must be a set)
-- (2) Start state is a state (s is in qs)
-- (3) Final states are states (fs is a subset of qs)
-- (4) Transition relation is a function from qs and sigma to qs
checkFSM :: (Ord a, Eq a) => [Char] -> FSM a -> Bool
checkFSM sigma (q, q0, f, ds) = (unique q) && (within q0 q) && (and[within x q | x <- f] && (and [(within s1 q) && (within s2 q) && (within c sigma) | (s1,c,s2) <- ds]))

-- dlt M q a == q'
-- dlt implements the delta function of M: given state q and input symbol a, 
-- it returns the new state q'.
dlt :: (Eq a) => FSM a -> a -> Char -> a
dlt fsm q a = undefined

-- deltaStar M q s
-- deltaStar implements the delta* function, which returns the state the machine
-- will be in after processing an entire string. 
deltaStar :: (Eq a) => FSM a -> a -> String -> a
deltaStar fsm q s = undefined

-- accept1 M s
-- accept1 defines string acceptance using deltaStar.
accept1 :: (Eq a) => FSM a -> String -> Bool
accept1 fsm s = undefined

-- accept2 M s
-- accept2 defines string acceptance using the Lq(M) construct: starting the
-- machine in an arbitrary state q, instead of just its start state.
accept2 :: (Eq a) => FSM a -> String -> Bool
accept2 fsm@(qs,st,_,_) s = accept_q fsm st s
  where
    accept_q :: (Eq a) => FSM a -> a -> String -> Bool
    accept_q fsm q s = undefined

----------------------------------- Part 2 -------------------------------------

-- Implement the following machines, and test them using accept1 and accept2
-- using some acceptable strings (the `strings` function is again provided
-- for you, if you want to use it):
--
-- * The machine that accepts all strings in (ab)*
-- * The machine that accepts all strings in (ab)(ab)* (i.e., (ab)+)
-- * The machine over sigma=['0','1'] that accepts strings which, when 
--   interpreted as base-2 numbers, are divisible by 3. Assume that the 
--   empty string is equivalent to 0. 
--
-- Use Ints for your states.

machine_ab_star :: FSM Int
machine_ab_star = ([undefined],  -- List of states
                   undefined,    -- Start state
                   [undefined],  -- list of final states
                   [(undefined,undefined,undefined)]) -- delta function

machine_ab_plus :: FSM Int
machine_ab_plus = undefined

machine_div_3 :: FSM Int
machine_div_3 = undefined
