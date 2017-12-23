# Explode Game

## How to run

To run the C CLI

    gcc -O3 c/*.c -o explode && ./explode

To run the Java CLI, using Java 8: (Java 9 does not work)

    mvn package && java -jar target/explode-1.0-SNAPSHOT.jar

## Object of the game

The object of explode is to make all the bombs on the screen your colour.  This is done by exploding the other player's bombs so that they become your bombs.  The game is over when all the bombs are the same colour.  Each player takes it in turns to place bombs.
 
## Rules of the game

You can place a bomb where there is an empty space, or where there is a bomb of your colour.  If you click on one of your existing bombs, its count will increase by one.  If it is an empty space, a bomb of you colour with a count of one will appear.  When a bomb of a reaches a certain count, it will explode.  This count varies depending on where the bomb is.  In a corner a bomb will explode when the count is two, on the edge when it is three and in the middle when it is four.

When a bomb explodes, all the surrounding bomb's count will increase by one.  In addition if the bomb is the other player's, it will become your colour.  If a blank space is next to an exploding bomb, it will become a bomb of your colour with a count of one.

## Hints and tips

Starting in the corners is a good idea.  Make sure however, that the two sqaures next to the corner aren't empty when you explode the bomb in it.  This is because the other player can easily take the two bombs you create.

If you have hardly any bombs on the screen, put bombs in empty areas so that the other player cannot win easily.

Whenever you have the opportunity, try to change the colour of the other player's bombs.

## History

* 1994 - The original version was a Windows 3.11 program written by Robin (GUI) and Adrian (single-player algorithm) in C, while we were still at school
* 2017 - Adrian removed the Windows 3.11 code which could no longer be compiled, and replaced it with a CLI in C, and added a Java port also with a CLI.


