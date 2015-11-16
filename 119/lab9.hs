{-# OPTIONS_GHC -fdefer-type-errors #-}
--
-- lab10.hs
-- Parsing expression grammars
--

-- A "matcher" is a function from strings to Maybe strings. If the matcher can
-- match some prefix of the string, it should return the remaining suffix. If
-- it cannot match at all, it should return Nothing.
type Matcher = String -> Maybe String

-- Matcher combinators. For each of the primitive PEG operators, there is a 
-- corresponding matcher combinator. I've implemented the more complex ones,
-- but it's up to you to implement the easier ones.

-- Sequencing: m1 m2 .. mn
seqn :: [Matcher] -> Matcher
seqn [] s = Just s
seqn (m:ms) s = case m s of
                    Just s' -> seqn ms s'
                    Nothing -> Nothing

-- Ordered choice: m1 / m2 / .. / mn
alt :: [Matcher] -> Matcher
alt [] _ = Nothing
alt (m:ms) s = case m s of
                  Just s' -> Just s'
                  Nothing -> alt ms s

-- Zero-or-more (greedy) m*
star :: Matcher -> Matcher
star m s = case m s of
              Just s' -> star m s
              Nothing -> Just s

-- One-or-more (greedy) m+
plus :: Matcher -> Matcher
plus m = seqn [m, star m]


--------------------------------------------------------------------------------
-- Implement these:

-- Zero-or-one, optional (m?)
opt :: Matcher -> Matcher
opt m s = undefined

-- Positive lookahead (&m)
pos :: Matcher -> Matcher
pos m s = undefined

-- Negative lookahead (!m)
neg :: Matcher -> Matcher
neg m s = undefined

-- Single character matcher (c). Note that if the string is empty this should
-- return Nothing.
chr :: Char -> String -> Maybe String
chr c s = undefined

-- Any character matcher (.). Again, if the string is empty this should return
-- Nothing.
dot :: Matcher
dot s = undefined

-- Optional: string matcher, match (and consume) u as a prefix of s. You may
-- find it helpful to implement this, when implementing your grammar, but it is
-- not required.
str :: String -> Matcher
str u s = undefined


-- Implement a PEG parser for your grammar. Note that this may require some
-- changes to your grammar in order to make it parse correctly as a PEG! You
-- should implement start as your start variable, and then any other matchers
-- as needed for the other variables.
parse :: Matcher
parse s = start (unspace s) 
    where
        -- Filter out spaces, for convenience.
        unspace s = filter (/=' ') s

        start :: Matcher
        start = undefined

-- Example: As an example to help get you started, here's a parser for 
-- regular expressions. Note that this merely accepts or rejects a RE, as your
-- parser above will do for your grammar. We're not worried about *constructing*
-- either a RE or an expression tree.
parse_re :: String -> Bool
parse_re s = case start (unspace s) of
              Just _  -> True
              Nothing -> False
    where
        unspace s = filter (/=' ') s

        -- RE <- Cat ('|' Cat)*
        start = seqn [conc, star (seqn [chr '|', conc])]

        -- Cat <- Star+
        conc = plus starred

        -- Star <- Elem ('*')*
        starred = seqn [element, star $ chr '*']

        -- Element <- '(' RE ')' 
        --          / Letter
        --          / 0
        --          / 1
        element = alt [
                seqn [chr '(', start, chr ')'],
                chr '0',
                chr '1'
            ]

