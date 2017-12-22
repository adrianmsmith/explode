package com.databasesandlife.explode;

import lombok.AllArgsConstructor;
import lombok.val;

import javax.annotation.Nonnull;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import static com.databasesandlife.explode.Board.size;
import static java.lang.Math.abs;

public class Algorithm {

    @AllArgsConstructor
    class Result {
        @Nonnull Coordinates c;
        int score;
    }

    protected @Nonnull List<Coordinates> getCoordinatesToAttempt(@Nonnull Board board, @Nonnull Player player) {
        val result = new ArrayList<Coordinates>();

        // Try corners first
        result.add(new Coordinates(0, 0));
        result.add(new Coordinates(0, size -1));
        result.add(new Coordinates(size -1, 0));
        result.add(new Coordinates(size -1, size -1));

        // Try edges next
        for (int i = 1; i < size -1; i++) {
            result.add(new Coordinates(i, 0));
            result.add(new Coordinates(i, size -1));
            result.add(new Coordinates(0, i));
            result.add(new Coordinates(size -1, i));
        }

        // Try squares we already have pieces
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                if (player.sign * board.value[x][y] > 0)
                    result.add(new Coordinates(x, y));

        // Try empty squares
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                if (board.value[x][y] == 0)
                    result.add(new Coordinates(x, y));

        return result;
    }

    protected @Nonnull Result staticAnalysis(@Nonnull Board board) {
        int score = 0;
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                score += board.value[x][y];
        return new Result(new Coordinates(0,0), score);
    }

    protected @Nonnull Result tryAllMovesAndSelectBest(
        @Nonnull Stack<Board> stack, int recursionDepth, int threshold, @Nonnull Player player
    ) {
        val grid = stack.peek();

        // One party has won?
        if (stack.size() > 1) {
            val winner = grid.findWinningPlayerOrNull();
            if (winner != null) return new Result(new Coordinates(0, 0), winner.getMaximum());
        }

        // No more recursion left? Do static analysis
        if (stack.size() == recursionDepth+1) return staticAnalysis(grid);

        // Recurse through list of squares to try, most likely to least likely
        Result best = null;
        for (val c : getCoordinatesToAttempt(grid, player)) {

            // Other player owns this square? We can't go here
            if (player.sign*grid.value[c.x][c.y] < 0) continue;

            int bestScore = best == null ? player.getMinimum() : best.score;

            val newGrid = grid.clone();
            newGrid.value[c.x][c.y] += player.sign;
            newGrid.explode();

            stack.push(newGrid);
            val resultOfOtherPlayer = tryAllMovesAndSelectBest(stack, recursionDepth, bestScore, player.getOther());
            stack.pop();

            if (best == null || player.sign*resultOfOtherPlayer.score > player.sign*best.score)
                best = new Result(c, resultOfOtherPlayer.score);

            // E.g. caller is maximizing and already has 12 from another square,
            // we are minimizing so as soon as we hit 12 we know we will return 12 or less.
            // If we return 12 or less, we know that the maximizing caller will choose their other square, so don't continue.
            if (player.sign*best.score >= player.sign*threshold) break;
        }
        if (best == null) throw new RuntimeException("Unreachable");
        return best;
    }

    protected @Nonnull Coordinates play(@Nonnull Board board, @Nonnull Player player, int recursionDepth) {
        val stack = new Stack<Board>();
        stack.push(board);

        val result = tryAllMovesAndSelectBest(stack, recursionDepth, player.getOther().getMinimum(), player);
        return result.c;
    }
}