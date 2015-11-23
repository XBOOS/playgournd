module ListComprehension where
import Prelude
-- some utils to use list comprehensions
factors :: Int ->[Int]
factors n = 
  [x|x<-[1..n],n `mod` x==0]

prime :: Int -> Bool --to determine wether an int is prime nubmer
--the facotrs are only 1 and itself
prime n = factors n == [1,n]

primes :: Int -> [Int]
primes n = [x | x<- [2..n] , prime x]

--Testing
--facotrs 10
--prime 7
--primes 56

--using zip to mimic sliding window
pairs ::[a] ->[(a,a)]
pairs  xs = zip xs (tail xs)

sorted ::Ord a=>[a]->Bool
sorted xs = 
  and [x<=y| (x,y)<-pairs xs]
