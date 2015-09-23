
--
-- lab.hs3
-- Matching strings against regular expressions.
--

import Data.List (sort)
import Data.Char (isAlpha)

type Matcher = RE -> String -> Bool

-- Test RE:
test_re :: RE
test_re = reparse "(a(b(aa)*b)*(a+b(aa)*ab)+b(a(bb)*a)*(b+a)(bb)*ba)*"

-- Problem 1: Implement the correct-but-slow regex matcher. This works by 
-- finding all possible "splits" for a concatenation and then trying the 
-- match against each one. The function `splits` is defined for you (below)
match_correct :: Matcher

--match_correct Empty s = null
--match_correct (Letter c) s = undefined
match_correct r s = case match r s of
                      Just "" -> True
                      Nothing ->False

  where
    match :: RE -> String -> Maybe String
    match Empty "" = (Just "")
    match (Letter a) (s:ss) = if (a == s)
                          then (Just "")
                          else (Nothing)
    match (Cat r1 r2) s = if and [((match_correct r1 s1) && (match_correct r2 s2)) | (s1,s2) <- (splits s)]
                          then (Just "")
                          else (Nothing)

    match (Union r1 r2) s = if or[((match r1 s1 == Just "") || (match r2 s2 == Just "")) | (s1,s2) <- (splits s)]
                            then (Just "")
                            else (Nothing)
    match (Star r) s = undefined
    match _ _ = Nothing
  --  match r s = undefined



-- Problem 2: Choose one of the other four matching algorithms:
--   * Incorrect matching against prefixes
--   * Correct matching by continuations
--   * Correct matching by enumeration
--   * Correct matching by derivaties
-- and implement it. In each case, we have provided you with a skeleton of
-- the solution, and all you have to do is write the internal `match` function. 

-- The prefix-based matcher works internally by returning either the remaining
-- string (on a successful match) or Nothing (indicating failure). 
match_prefixes :: Matcher
match_prefixes r s = case match r s of
                       Just "" -> True
                       Nothing -> False

  where
    match :: RE -> String -> Maybe String
    match r s = undefined


-- The continuation-based matcher works by matching a prefix of the string 
-- while at the same time, constructing a continuation that will only match
-- the corresponding suffix. 
type Continuation = String -> Bool
match_continuation :: Matcher
match_continuation r s = match r s (=="")
  where
    match :: RE -> String -> Continuation -> Bool
    match r s k = undefined


-- Enumeration-based matching works internally by checking to see whether the
-- string in question is an element of a possibly-infinite length-ordered list.
-- You will need to implement lenElem, the function that checks for language
-- membership, and enumerate, which outputs the list of strings in the language
-- defined by its input.
type LOL = [String] -- "Length-Ordered Language"
match_enumerate :: Matcher
match_enumerate r s = s `lenElem` (enumerate r)
  where
    -- lenElem returns True if s is an element of the length-ordered language
    -- l.
    lenElem :: String -> LOL -> Bool
    lenElem s l = undefined

    enumerate :: RE -> LOL
    enumerate r = undefined


-- The derivative-based matcher works by taking the derivative of the RE with
-- respect to the initial character in the input string. The result will either
-- be another RE (which can then be differentiated against the *second* char.,
-- and so forth) or an RE which is_empty. You will need the functions is_empty,
-- and has_epsilon from lab 2. 
match_derivative :: Matcher
match_derivative r s = derivStar r s
  where
    -- Repeatedly take the derivative of r with respect to the leading char.
    -- of the input string, until either the string is accepted or is_empty r.
    derivStar :: RE -> String -> Bool
    derivStar r s = undefined

    -- Take the derivative of an RE with respect to a character c.
    deriv :: Char -> RE -> RE
    deriv c r = undefined

    -- Delta is the delta-function defined in lecture and in the original 
    -- paper. You will need it in the implementation of deriv.
    delta :: RE -> RE
    delta r = undefined

    is_empty :: RE -> Bool
    is_empty r = undefined

    has_epsilon :: RE -> Bool
    has_epsilon r = undefined


-- Testing: Test your code against the first 10 strings of length 20; i.e.,
-- against
--   take 10 $ strings 20
-- Use the command
--   :set +s
-- in GHCi to record timing and memory usage. Record timing and memory usage
-- of both match_correct and your other matcher below
--
-- match_correct memory: 
-- match_correct time: 
--
-- match_prefixes memory:
-- match_prefixes time:
-- 
-- match_continuation memory:
-- match_continuation time:
--
-- match_enumerate memory:
-- match_enumerate time:
--
-- match_derivative memory:
-- match_derivative time:
--
-- Here is the correct output for match_correct, match_continuation, 
-- match_enumerate, and match_derivative:
-- ...
--
-- and here is the "correct" output for match_prefixes:
-- ...

-------------------------------------------------------------------------------
-- Begin provided code

-- Utility function for testing: generates a list of all strings of length
-- n, drawing from the alphabet sigma
strings :: Int -> [String]
strings 0 = [""]
strings n = concat [map (a:) strings' | a <- sigma]
  where
    strings' = strings (n-1)

-- Splits
-- Generates the lists of all prefixes and suffixes of the input
splits :: [a]  -> [([a],[a])]
splits [] = [([], [])]
splits w@(x:xs) = ([],w) : map (\(a,b) -> (x:a,b)) (splits xs)

distinct :: Ord a => [a] -> [a]
distinct = dedup . sort
    where
        dedup [] = []
        dedup [x] = [x]
        dedup (x1:x2:xs) | x1 == x2   = dedup (x2:xs)
                         | otherwise   = x1 : dedup (x2:xs)

type Language = [String]

sigma = ['a', 'b']

-------------------------------------------------------------------------------
-- Regular expressions
-- Here we define a datatype for regular expression, RE, as well as code to
-- print REs in the syntax we've used in class, and a function reparse
-- which will parse a string in the syntax into a RE. 

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