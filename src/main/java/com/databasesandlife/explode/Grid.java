package com.databasesandlife.explode;

import lombok.Getter;
import lombok.val;

import javax.annotation.CheckForNull;
import javax.annotation.Nonnull;
import java.util.ArrayList;
import java.util.List;

import static java.lang.Integer.signum;
import static java.lang.Math.abs;

class Grid implements Cloneable {

    public static final int width = 6;

    int[][] value = new int[width][width];

    void explode() {
        x: while (true) {
            if (hasWon() != null) return;
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < width; y++) {
                    boolean marginY = y == 0 || y == width - 1;
                    boolean marginX = x == 0 || x == width - 1;
                    int max = (marginX && marginY) ? 2 : (marginX || marginY) ? 3 : 4;
                    int val = value[x][y];
                    if (val == 0) continue;
                    int sign = signum(val);
                    if (val * sign >= max) {
                        if (x > 0) value[x - 1][y] = sign * (abs(value[x - 1][y]) + 1);
                        if (x < width - 1) value[x + 1][y] = sign * (abs(value[x + 1][y]) + 1);
                        if (y > 0) value[x][y - 1] = sign * (abs(value[x][y - 1]) + 1);
                        if (y < width - 1) value[x][y + 1] = sign * (abs(value[x][y + 1]) + 1);
                        value[x][y] -= sign * max;
                        continue x;
                    }
                }
            }
            break;
        }
    }

    public boolean isEmpty() {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < width; y++)
                if (value[x][y] != 0) return false;
        return true;
    }

    public @CheckForNull Player hasWon() {
        boolean negativeHasWon = true;
        boolean positiveHasWon = true;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < width; y++) {
                int val = value[x][y];
                if (val < 0) positiveHasWon = false;
                if (val > 0) negativeHasWon = false;
            }
        }
        if (negativeHasWon && positiveHasWon) return null;
        if (negativeHasWon) return Player.negative;
        if (positiveHasWon) return Player.positive;
        return null;
    }

    public @Nonnull Grid clone() {
        val result = new Grid();
        for (int x = 0; x < width; x++)
            System.arraycopy(value[x], 0, result.value[x], 0, width);
        return result;
    }

    @Override public @Nonnull String toString() {
        val result = new StringBuilder();
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < width; x++)
                result.append(String.format("%2d ", value[x][y]));
            result.append("\n");
        }
        return result.toString();
    }
}
