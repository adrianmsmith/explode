package com.databasesandlife.explode;

import lombok.val;

import java.util.Scanner;

public class Game {

    public static void main(String[] args) {
        val stdin = new Scanner(System.in);
        val grid = new Grid();
        val humanPlayer = Player.positive;
        boolean firstMove = true;  // After one player has placed one move, they appear to have "won"
        while (true) {
            System.out.print(grid.toString());

            if ( ! firstMove) {
                val won = grid.hasWon();
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
                val won2 = grid.hasWon();
                if (won2 != null) {
                    System.out.println("Player " + won2 + " has won");
                    return;
                }
            }
            Coordinates c = new Algorithm().play(grid, humanPlayer.getOther(), 5);
            System.out.println("Computer has chosen (" + c.x + "," + c.y + ")");

            grid.value[c.x][c.y]--;
            grid.explode();

            firstMove = false;
        }
    }
}
