{-# OPTIONS_GHC -fdefer-type-errors #-}
--
-- lab5.hs
-- Converting regular languages/expressions to machines
--

import Data.List
import Data.Char
sigma = ['a', 'b']

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

-------------------------------------------------------------------------------
-- Begin assignment

-- Write functions that will construct the Empty, Symbol, Union, Cat, and Star
-- machines. Then use these functions to write a function re_to_fsm that
-- converts a regular expression to a finite state machine.
--
-- 
-- 0 is your list of states 
-- q0 your starting state
-- F is your final states
lookupAll :: (Eq a) => (a,Char) -> [(a,Char,a)] -> [a]
lookupAll (q,a) dlt = [s2 | (s1,c,s2) <- dlt, q == s1, c == a]

dlt :: (Eq a) => FSM a -> a -> Char -> a
dlt fsm q a = head(lookupAll (q,a) (delta fsm) )

distinct :: Ord a => [a] -> [a]
distinct = dedup . sort
    where
        dedup [] = []
        dedup [x] = [x]
        dedup (x1:x2:xs) | x1 == x2   = dedup (x2:xs)
                         | otherwise   = x1 : dedup (x2:xs)


-- Construct the Empty machine, the machine that accepts no strings.
m_empty :: FSM Int
m_empty = ([0],0,[],[(0,c,0) | c <- sigma])

-- Construct the Symbol machine, the machine that accepts strings consisting
-- of the single character c.
m_symbol :: Char -> FSM Int
m_symbol c = ([0,1,-1],0,[1], [(0,c,1)]++[(0,a,-1) | a <- sigma, c/= a]++[(1,a,-1) | a<- sigma]++[(-1,a,-1) | a <- sigma])

-- Construct the Union machine, the machine that accepts strings in the union
-- of L(m1) and L(m2)
m_union :: (Eq a, Eq b) => FSM a -> FSM b -> FSM (a,b) 
m_union m1@(q1,s1,f1,d1) m2@(q2,s2,f2,d2) = (_Q,_q0,_f,_dlt)
    where
      _Q = [(qa,qb)  | qa <- q1, qb <- q2 ]
      _q0 = (s1,s2)
      _f = [(qa, qb) | qa <- q1, qb <- q2, ((qa `elem` f1) || (qb `elem` f2)) ]
      _dlt = [( (qa,qb) , a, ((dlt m1 qa a), (dlt m2 qb a)) )| a <- sigma, qa <- q1, qb <- q2]

-- Construct the Cat machine, the machine that accepts strings in the 
-- concatenation of L(m1) and L(m2).
correction :: (Eq a, Eq b) => (a,[b]) -> [a] -> b -> (a,[b])
correction (q,x) f1 q0 = if (q `elem` f1)
                         then (q, x ++ [q0])
                         else (q,x)

{-
m_cat :: (Eq a, Eq b) => FSM a -> FSM b -> FSM (a,[b])
m_cat m1@(q1,s1,f1,d1) m2@(q2,s2,f2,d2) = (_Q,_q0,_f,_dlt) 
    where
      _Q = [ correction (qa, x2) f1 _q0 | qa <- q1, x2 <- subsequences(q2)]
      _q0 = correction (_q0,[]) f1 _q0
      _f = [(q,x) | (q,x) <- _Q, (x `intersect` f2) /= []]
      _dlt = [( ql,c, correction((dlt m1 q c), ([(dlt m2 _X c) | _X <- x]) ) f1 s2 ) | c <- sigma, ql@(q,x) <- _Q]
-}
-- Construct the Star machine, the machine that accepts strings in the 
-- Kleene start of L(m).

correction_star :: Eq a => [a]
correction_star x f q0 = if( (x `intersect` f ) /= [] )
                         then (x ++ [q0])
                         else (x)

m_star :: Eq a => FSM a -> FSM [a]
m_star m@(q,s,f,d) = (_Q,_q0,_f,_dlt)
    where
      _Q = [correction_star (x) f _q0 | x <- subsequences(q)]
      _q0 = []
      _f = [] ++ [x | x <- _Q, (x `intersect` f) /= [] ]
      _dlt = --[(l,c,r)] 

-- Using the above, write the function re_to_fsm that converts a RE into a
-- FSM that accepts the same language.
re_to_fsm :: Eq a => RE -> FSM a
re_to_fsm Empty = undefined
re_to_fsm (Letter c) = undefined
re_to_fsm (Cat r1 r2) = undefined
re_to_fsm (Union r1 r2) = undefined
re_to_fsm (Star r) = undefined



-- Extra-credit question: Implement reachability
-- reachable m returns m' such that only the reachable states of m are in m'.
reachable :: Eq a => FSM a -> FSM a
reachable m = undefined


-- Test. Convert some REs to FSMs. How many states do they have? What about
-- our test re:
-- Test RE:
test_re :: RE
test_re = reparse "(a(b(aa)*b)*(a+b(aa)*ab)+b(a(bb)*a)*(b+a)(bb)*ba)*"
-- Can you convert it to an FSM? 


-------------------------------------------------------------------------------
-- Regular expressions
-- Here's all the usual RE stuff.

data RE = Empty
        | Letter Char
        | Cat RE RE
        | Union RE RE
        | Star RE

-- We make RE an instance of Show so that REs can be converted to strings. GHCi
-- will implciitly do this when printing values of type RE.
instance Show RE where

  showsPrec _ Empty       = showString "0"
  showsPrec _ (Letter c)  = showString [c]
  showsPrec d (Cat r1 r2) = showParen (d > up_prec) $
                              showsPrec (up_prec+1) r1 .
                              showsPrec (up_prec+1) r2
    where
      up_prec = 5

  showsPrec d (Union r1 r2) = showParen (d > up_prec) $
                                showsPrec (up_prec+1) r1 .
                                showString "+" .
                                showsPrec (up_prec+1) r2
    where
      up_prec = 3

  showsPrec d (Star r)    = showsPrec 10 r . showString "*"

{------------------------------------------------------------------------------
   Parsing regular expressions
   ---------------------------

   An (ambiguous) grammar for RE:
   
   <RE> ::= <RE><RE>
   <RE> ::= <RE> '*'
   <RE> ::= <RE> '+' <RE>
   <RE> ::= '(' <RE> ')'
   <RE> ::= SYMBOL
   <RE> ::= '1'
   <RE> ::= '0'

   An ambiguous grammar doesn't do us much good, so we'll encode the precedence
   (* has highest precedence, followed by concatenation, followed by +) in the
   grammar:

   <RE>   ::= <Plus>
   <Plus> ::= <Cat> ('+' <Plus>)*
   <Cat>  ::= <Star> <Cat>*
   <Star> ::= <Unit> '*'*
   <Unit> ::= '(' <Plus> ')' 
            | SYMBOL
            | '1'
            | '0'

   Note that in this definition, both <Plus> and <Cat> will be *right* 
   associative; this doesn't matter in this case, since both are associative
   operators, but it's something you should bear in mind when constructing
   grammars with left-associative operators.

   The parser below implements a functional PEG parser. For large inputs the
   performance will be exponential, but this hopefully won't be a problem for
   the REs we'll be accepting.
-}
type ParseResult = Maybe (String,RE)
type Rule = String -> ParseResult

reparse :: String -> RE
reparse s = let Just ("",result) = plus $ filter (/=' ') s in result
  where

    -- <Plus> ::= <Cat> ('+' <Plus>)*
    plus :: Rule
    plus ss = case cat ss of
                Just (ss',re) -> build Union $ (Just (ss',re)) : rept2 plus '+' ss'
                Nothing       -> Nothing

    -- <Cat> ::= <Star> <Cat>*
    cat :: Rule
    cat ss = case star ss of
              Just (ss',re) -> build Cat $ (Just (ss',re)) : rept cat ss'
              Nothing       -> Nothing

    -- <Star> ::= <Unit> '*'*
    star :: Rule
    star ss = case unit ss of
                Just ('*':ss', re) -> Just (consume '*' ss', Star re)
                Just (ss,re)       -> Just (ss,re)
                Nothing            -> Nothing

    -- <Unit> ::= '(' <Plus> ')' | SYMBOL | '1' | '0'
    unit :: Rule
    unit = alt [paren, symbol, epsilon, zero]

    epsilon :: Rule
    epsilon ('1':ss) = Just (ss, Star Empty)
    epsilon _        = Nothing 

    zero :: Rule
    zero ('0':ss) = Just (ss, Empty)
    zero _        = Nothing

    symbol :: Rule
    symbol (s:ss) | isAlpha s    = Just (ss, Letter s)
    symbol _ = Nothing

    paren :: Rule
    paren ('(':ss) = case plus ss of
                        Just (')':ss',re) -> Just (ss',re)
                        _                 -> Nothing
    paren _ = Nothing

    -------------------------------------
    -- Helper functions implementing the
    -- internals of a PEG parser.
    alt :: [Rule] -> Rule
    alt [] _ = Nothing
    alt (r:rs) s = case r s of
                     Just result -> Just result -- Return first successful match
                     Nothing -> alt rs s        -- Failed; try next alternative

    -- Eat up all the c's at the beginning of the string s
    consume :: Char -> String -> String
    consume _ "" = ""
    consume c s@(c':cs) | c == c'   = consume c cs
                        | otherwise = s

    -- Recognize zero or more Rules
    rept :: Rule -> String -> [ParseResult]
    rept r s = case r s of
                Just (ss,re) -> (Just (ss,re)) : rept r ss
                Nothing      -> []

    -- A variant of rept that recognizes ('c' <r>)* and constructs the list
    -- of results by discarding all the c's.
    rept2 :: Rule -> Char -> String -> [ParseResult]
    rept2 r c (c':s) | c == c'   = case r s of
                                    Just (ss,re) -> Just (ss,re) : rept2 r c ss
                                    Nothing      -> []
    rept2 _ _ _ = []

    -- Converts a list of ParseResults (as returned by rept) into a single
    -- parse result, by applying the RE constructor cons right-associatively.
    build :: (RE -> RE -> RE) -> [ParseResult] -> ParseResult
    build _    []    = Nothing
    build _    [res] = res
    build cons ((Just (ss,re)):rs) = let (Just (ss',re')) = build cons rs
                                     in
                                       Just (ss', cons re re')
