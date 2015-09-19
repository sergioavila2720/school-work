{-# OPTIONS_GHC -fdefer-type-errors #-}
--
-- lab0.hs
-- CSci 119, Lab 0
-- The main purpose of this lab is to make sure that everyone knows how to
-- get into Haskell and work on the labs, both on the provided lab computers
-- and on your personal laptops. There are a few problems for you to do, just
-- to get you started. Here, as in all following labs, your main task is to
-- replace every `undefined` with the code necessary to complete the problem.
--

import Data.List

-------------------------------------------------------------------------------
-- Part I: Condescendingly Easy Problems

-- Fill in your name as a string
name = "sergio"

-- Fill in a greeting, for example, "Hello" (but be more creative than that)
greeting = "hello I'm sergio. still getting used to haskell again"

-- With the above to completed, you should be able to run `greet_me` in GHCi
-- to be "greeted" by Haskell. 
greet_me =  greeting ++ ", " ++ name ++ ", from Haskell!"

-- Complete the definition so as to compute the length, in characters of 
-- your greeting.
greeting_length = length greet_me

-------------------------------------------------------------------------------
-- Part II: Lists

{- Haskell has a lot of built-in functions for operating on lists. In the
   "Prelude" (the set of definitions that are automatically available), we have

   a:as          Infix "cons" operator (adds a to the head of the list as)
   as ++ bs      Infix list append operator (returns the result of appending as 
                 to bs).
   head l        Returns the head of a list l
   tail l        Returns the tail of a list l
   last l        Returns the last element of l
   init l        Returns everything *except* the last element of l
   take n l      Returns the list of the first n elements of l
   drop n l      Returns the list of everything except the first n elements of l
   l !! n        Returns the n-th element of the list l
   reverse l     Returns the elements of l in reverse order

   null l        Returns True if l is the empty list
   length l      Returns the length of l (number of elements)

   filter p l    Remove all elements of l that do not satisfy the predicate p
                 and return the result. E.g.
                    filter odd [1,2,3,4]   ====>   [1,3]
                    filter (=1) [1,2,1,3]  ====>   [1,1]
   map f l       Apply the function f to all the elements of l and return the 
                 result. E.g.
                    map (+1) [1,2,3,4]   ====>   [2,3,4,5]

   and l         Returns True if *all* elements of l are True
   or l          Returns True if *any* elements of l are True
   maximum l     Returns the largest element of l
   minimum l     Returns the smallest element of l
   sum l         Returns the sum of the elements of l
   product l     Returns the product of the elements of l

   concat l      "Flatens" a list-of-lists into a list. E.g.,
                    concat [[1,2], [3,4]]   ====>   [1,2,3,4] 

   repeat a      Returns the infinite list [a,a,a,a...]
   replicate n a Returns the list consisting of n copies of a

   elem a l      Returns True if a occurs in l
   notElem a l   Returns False is a occurs in l

   zip as bs     "Zips" as and bs by returning a list of pairs. E.g.,
                    zip [1,2] ['a','b']   ====>   [(1,'a'), (2,'b')]

   as == bs      Equality works for lists as you'd expect.

   (I've omitted all the `fold` related functions, as we'll talk about them
   later.)

   There are also some useful functions defined in the Data.List module, which
   we've imported above:

   intersperse a l   Intersperses a as every other element in l. E.g.
                     intersperse 1 [2,3,4]   ====>   [1,2,1,3,1,4]

   subsequences l    Returns the list of all subsequences (sublists) of l.
   permutations l    Returns the list of all permutations of l.

   sort l            Sorts the list l. Note that the elements of l must be 
                     comparable (must support Ord)!

   inits l           Returns the list of all prefixes of l
   tails l           Returns the list of all suffixes of l

   isPrefixOf as bs  Returns True if as is a prefix of bs (i.e., if as is an 
                     `elem` of `inits bs`).
   isSuffixOf as bs  Returns True if as is a suffix of bs (i.e., 
                     if `elem as (tails bs)`).

   as \\ bs          Gives the list (set-like) difference of as and bs, i.e.,
                     as with all the elements of bs removed.
   union as bs       Gives the set union of as and bs.
   intersect as bs   Gives the set intersection of as and bs.

   This next section will have you using some of these, and *reimplementing*
   some from scratch.
-}

-- Write the function `mylen` which takes a list and returns the number of
-- elements. Do not use the built-in length function. I've given you the two
-- cases for the empty and non-empty list. This function will be recursive
-- on the input list.
mylen :: [a] -> Int
mylen []     = 0
mylen (a:as) = 1 + mylen as 

-- Write the function `empty` which takes a list and returns True if it is
-- empty (has length 0) and False otherwise. Do not use the built-in `length`
-- or `null` functions, but you may use your `mylen` function.
empty :: [a] -> Bool
empty as = if (mylen as == 0)
           then True
           else False

-- Write the function `myappend` which takes two lists and appends them together.
-- Don't use the built-in ++ operator. This function will be recursive on its
-- first argument.
myappend :: [a] -> [a] -> [a]
myappend []     bs = bs
myappend (a:as) bs = a:myappend as bs

-------------------------------------------------------------------------------
-- Part III: Set operations
-- Although Data.List provides set-wise union and intersection, you are going
-- to re-implement them from scratch (i.e., without using their built-in 
-- equivalents). Note that you may find it easier to write these as list-
-- comprehensions rather than recursively.

-- Universe of discourse: Note that your functions should be able to work
-- correctly with *any* universe, not just this one
u = [1,2,3,4,5]

-- Duplicate-removal: In order to use sets as lists, we have to ensure that
-- they are duplicate-free. The function `dedup` removes duplicates from a list.
-- Note that there is a function `nub` in Data.List that does the same thing,
-- but dedup has two advantages over nub:
--  
--   * dedup is faster. Because nub does not assume its input is sortable, it
--     has to compare every element to every other element, so it runs in 
--     O(n^2) time. dedup works by sorting, so it runs in O(n log n) time.
--
--   * dedup gives sets a "canonical form". E.g., the sets {1,2,3} and 
--     {3,2,1} are equal, but as lists [1,2,3] and [3,2,1] they are not. By 
--     sorting the lists we ensure that equal sets are also ==.
dedup :: Ord a => [a] -> [a]
dedup = derun . sort
  where
    derun []         = []
    derun [a]        = [a]
    derun (a1:a2:as) | a1 == a2   = derun (a2:as)      -- Drop a1
                     | otherwise  = a1 : derun (a2:as) -- Keep a1

-- Note that while you *can* just use dedup to "brute-force" the removal of
-- duplicates, it's better to write your set functions so that they do not
-- produce duplicates in the first place (if this is possible).

-- Write the uni(on) function, which produces the union of two lists-as-sets.
-- Do not use the built-in `union` function; you may use ++, however.
-- Assumes: the inputs as and bs are proper duplicate-free sets.
-- Ensures: the output is a proper duplicate-free set, the union of the inputs 
uni :: Ord a => [a] -> [a] -> [a]
uni as bs = dedup(as ++ bs)

-- Write the inters(ect) function, which produces the intersection of two lists.
-- Do not use the built-in `intersect` function, or the \\ operator.
-- Assumes: both inputs are proper duplicate-free sets.
-- Ensures: the output is a proper duplicate-free set, the intersection of the
-- inputs.
inter :: Ord a => [a] -> [a] -> [a]
inter as bs = let ns = [ a | a <- as, elem a bs] in [ b | b <- bs, elem b ns]

-- Write the symmetric set diff(erence) function, which produces the symmetric
-- set difference of two lists (i.e., all the elements that are in as or bs but
-- not in both). 
-- Assumes: both inputs are proper duplicate-free sets.
-- Ensures: the output is a proper duplicate-free set, the symmetric set 
-- difference of the inputs.
diff :: Ord a => [a] -> [a] -> [a]
diff as bs = dedup (myappend[a |a <- as, notElem a bs] [b | b <- bs, notElem b as])