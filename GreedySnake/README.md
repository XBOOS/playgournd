## Simple Version of greedy snake
### Implementation without OO
( credit to stanford version)
### Rules specification1. The snake moves by extending its head in the direction it's moving and pulling its tail in one space. 
2. The snake wins if it eats twenty pieces of food.3. The snake loses if it crashes into itself or into a wall.4. If the snake eats a piece of food, its length grows by one and a new piece of food is randomly placed. 
5. There is only one level, the starting level.


### World representation
+ A space character (' ') represents an empty tile.+ A pound sign ('#') represents a wall.+ A dollar sign ('$') represents food.+ An asterisk ('*') represents a tile occupied by a snake.