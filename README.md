# Breaking-1357
A proof for the solution of the game 1357 and a bot for perfect play

## Game Breakdown
The 1357 game starts with 4 rows each with 1, 3, 5, and 7 sticks as shown below:
```
   |
  |||
 |||||
|||||||
```
(In reality, the sticks are usually sugar packets taken from a condiment holder while waiting for your food)

The game has two players, player 1 and player 2.
For a players move, they choose any of the 4 rows and can remove any amount of sticks from that row alone.

✔ Valid Move:
```
   |             |
  |||    -->    |||
 |||||   -->    |||
|||||||       |||||||
```
✖ Invalid Move:
```
   |             |
  |||    -->     |
 |||||   -->    |||
|||||||       ||||||| 
```

The goal of the game is to make the other player take the last stick.
In the state below, the player whose turn it is will be forced to lose because they only have one option.
```
   |    
   -   
   -  
   -
```
Dashes (-) represent empty rows

In this other state, the player whose turn it is will be able to force the other player to lose on their next turn.
```
   |            |
   -    -->     -
   -    -->     -
 |||||          - 
```

## Proof

The proof is a program that checks every state the game can be in and
backtracks to see if it is possible for either player 1 or player 2 to win
every time with perfect play.

It starts by creating a tree and a map.

It begins with the initial game state, represented by the string "13571" (the
first 4 characters represent the number of sticks in each row, and the final
character represents which players turn it is, 1 or 2)

A node is created with this game state inside, it also has two vectors of node pointers, parents and children.

This node is placed into the map with the string representation as the key.

Then the child states are created, then added to the children vector and the map.

Then, a recursive function is called on each of the children individually.
This function will create all the children of the inputted state, and iterate over them.
It checks if a child is in the map,
    If it IS NOT in the map, then a new state node is created, it is added to the children vector of its parent, and the function is called on it.
    If it IS in the map, then the node pointer inside the map is added to the children vector of the parent, and the loop continues.

There is now a tree of every state and two-way edges between every parent and child node.
There is also a map that contains every unique state node pointer with the state's string representation as the key.

Then, starting with the winning state ("0000X" where X is the winning player), a set is created.
The set is filled with states that the player can guarantee lead to their own winning state.
If a solution set for either player contains the initial position ("13571"), then that player can always force a win with perfect play.

## Conclusion

As it turns out, player 2 can always force a win in this game.
Although, the size of the solution sets for either player are very similar in size.
this means that a mistake from player 2 could result in the current state of
the game to be in the solution set of player 1, meaning player 1 can guarantee
a win with perfect play.

## Bot

Using the solution sets, I have created a bot that plays the game perfectly.
Given the current state of the game, it checks if that state is in its solution set.
    If it is, then it iterates over the current state's children until it finds
    one that is also in the solution set. Due to the way the set works, every state
    in the solution set (except for the winning state) will always have at least
    one child in the set as well.
    If it is not, then it iterates over the current state's children and finds
    the one with the highest percentage of children that are in the solution set.
    This is so that the player (the person playing against the bot) has the highest
    chance of making a move that puts the current state into the bot's solution
    set. 


To use the bot, run `game [player number]` where `player number` is either 1 or 2. \
    If you choose 1, then you will make the first move (You will lose). \
    If you choose 2, then the bot will make the first move (good luck 😅).
