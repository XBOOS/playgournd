module Parser where 
import Prelude hiding (return)

type Parser a = String-> [(a,String)]

item :: Parser Char
item = \input -> case input of 
  [] -> []
  (x:xs) -> [(x,xs)]

failure :: Parser a
failure = \input -> []

return :: a->Parser a
return v = \input -> [(v,input)]

(+++) :: Parser a ->Parser a->Parser a
p +++ q = \input -> case  parse p input of 
  [] -> parse q input
  [(v,out)] -> [(v,out)]


parse :: Parser a -> String -> [(a,String)]
parse p input = p input

---
p :: Parser (Char,Char)
p = do x <- item
       item
       y <- item
       return (x,y)
