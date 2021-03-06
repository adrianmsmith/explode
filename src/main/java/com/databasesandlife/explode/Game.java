package com.databasesandlife.explode;

import lombok.val;

import java.util.Scanner;

public class Game {

    public static void main(String[] args) {
        val stdin = new Scanner(System.in);
        val grid = new Board();
        val humanPlayer = Player.positive;
        boolean firstMove = true;  // After one player has placed one move, they appear to have "won"

        System.out.print("Difficulty (1..5 or higher): ");
        int recursionDepth = stdin.nextInt();

        while (true) {
            System.out.print(grid.toString());

            if ( ! firstMove) {
                val won = grid.findWinningPlayerOrNull();
                if (won != null) {
                    System.out.println("Player " + won + " has won");
                    return;
                }
            }

            System.out.print("Enter move x (0..5): ");
            int x = stdin.nextInt();
            System.out.print("Enter move y (0..5): ");
            int y = stdin.nextInt();

            grid.value[x][y] += humanPlayer.sign;
            grid.explode();

            System.out.print(grid.toString());
            if ( ! firstMove) {
                val won = grid.findWinningPlayerOrNull();
                if (won != null) {
                    System.out.println("Player " + won + " has won");
                    return;
                }
            }
            Coordinates c = new Algorithm().play(grid, humanPlayer.getOther(), recursionDepth);
            System.out.println("Computer has chosen (" + c.x + "," + c.y + ")");

            grid.value[c.x][c.y]--;
            grid.explode();

            firstMove = false;
        }
    }
}
