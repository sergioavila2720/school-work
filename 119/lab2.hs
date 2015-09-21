--
-- lab2.hs
-- Conversion of RE to regular languages via lang_of, properties of 
-- regular languages.
--

import Data.List (sort)
import Data.Char (isAlpha)

distinct :: Ord a => [a] -> [a]
distinct = dedup . sort
    where
        dedup [] = []
        dedup [x] = [x]
        dedup (x1:x2:xs) | x1 == x2   = dedup (x2:xs)
                         | otherwise   = x1 : dedup (x2:xs)

-------------------------------------------------------------------------------
-- Set operations
-- Here are the set operations you were asked to implement in lab 1.

type Language = [String]

sigma = ['a', 'b']

zero :: Language
zero = []

one :: Language
one = [""]

sym :: Char -> Language
sym = (:[]) . (:[])

uni :: Ord a => [a] -> [a] -> [a]
uni = (distinct .) . (++)

cat :: Language -> Language -> Language
cat l1 l2 = [s1 ++ s2 | s1 <- l1, s2 <- l2]

starn :: Int -> Language -> Language
starn n s = foldl uni one $ [selfcat i s | i <- [1..n]]
  where
    selfcat 0 _ = one
    selfcat n s = s `cat` (selfcat (n-1) s)

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

--------------------------------------------------------------------------------
-- Lab Begins Here

-- lang_of
-- lang_of implements L(re), the recursive definition of the language defined
-- by a regular expression. As in lab 1, convert the Kleene star to a bounded
-- star, with n=5.

lang_of :: RE -> Language
lang_of Empty = []
lang_of (Letter _) = [""]
lang_of (Cat r1 r2) = (lang_of r1) `cat` (lang_of r2) 
lang_of (Union r1 r2) = (lang_of r1) `uni` (lang_of r2)
lang_of (Star r1) = starn 5 (lang_of r1)
--lang_of Letter Char = one
--lang_of r = zero


-- Implement the (structurally recursive) functions defining the properties of
-- regular languages we've defined in class. Note that you must implement these
-- purely on the RE; *do not* use lang_of in any of your definitions.

-- is_empty 
-- True iff L(r) = {}
is_empty :: RE -> Bool
is_empty Empty = True
is_empty _ = False


-- is_one
-- True iff L(r) = {""}
is_one :: RE -> Bool
is_one (Letter one) = True
is_one _ = False

-- has_epsilon 
-- True iff "" is an element of L(r) 
has_epsilon :: RE -> Bool
has_epsilon Empty = False 
has_epsilon (Letter _) = False
has_epsilon (Cat r1 r2) = (is_one r1) || (is_one r2) || (has_epsilon r2)
has_epsilon (Union r1 r2) = (is_one r1) || (is_one r2) || (has_epsilon r2)
has_epsilon (Star r) = (is_one r)
--has_epsilon Empty = True 

-- is_infinite 
-- True iff L(r) is infinite
is_infinite :: RE -> Bool
is_infinite Empty = False
is_infinite (Letter _) = False
is_infinite (Star r1) = True
is_infinite (Cat r1 r2) = (is_infinite r1) || (is_infinite r2)
is_infinite (Union r1 r2) = (is_infinite r1) || (is_infinite r2)


-- rev
-- Converts r to r' such that L(r') = (L(r))^R, i.e., does language reversal,
-- but on the structure of the RE.
rev :: RE -> RE
rev Empty = Empty
rev (Letter a) = (Letter a)
rev (Star r1) = (Star (rev r1))
rev (Cat r1 r2) = ((rev r2) `Cat` (rev r1))
rev (Union r1 r2) = ((rev r2) `Union` (rev r1))
rev r = undefined

-- nepart
-- Converts r to r' such that "" is not an element of L(r'). I.e., if L(r)
-- would accept the empty string, then L(r') will *not* accept "". If L(r)
-- already would not accept "", then r' == r.
nepart :: RE -> RE
nepart (Union r1 r2) = if (is_one r1)
                       then (nepart r2)
                       else if (is_one r2)
                        then (nepart r1)
                        else (Union (nepart r1) (nepart r2))
nepart (Cat r1 r2) = if (is_one r1)
                     then  (nepart r2)
                     else if (is_one r2)
                      then (nepart r1)
                      else (Cat (nepart r1) (nepart r2))
nepart (Star r) = if (is_empty r)
                  then Empty
                  else (Cat r (Star r))
nepart r = undefined

-- Extra credit question!
-- simplify
-- "Simplify" an RE. You may define simplification however you like, but it 
-- should do at least the following:
-- * If any subexpression of r is empty, then it is replaced with 0.
-- * If any subexpression of r is one, then it is replaced with 0*
-- * r `uni` 0 ==> r    and    0 `uni` r ==> r
-- * r `cat` 1 ==> r    and    1 `cat` r ==> r
simplify :: RE -> RE
simplify r = undefined