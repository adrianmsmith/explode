package com.databasesandlife.explode;

import lombok.val;

import javax.annotation.CheckForNull;
import javax.annotation.Nonnull;

import static java.lang.Integer.signum;
import static java.lang.Math.abs;

class Board implements Cloneable {

    public static final int size = 6;

    int[][] value = new int[size][size];

    void explode() {
        x: while (true) {
            if (findWinningPlayerOrNull() != null) return;
            for (int x = 0; x < size; x++) {
                for (int y = 0; y < size; y++) {
                    boolean marginY = y == 0 || y == size - 1;
                    boolean marginX = x == 0 || x == size - 1;
                    int max = (marginX && marginY) ? 2 : (marginX || marginY) ? 3 : 4;
                    int val = value[x][y];
                    if (val == 0) continue;
                    int sign = signum(val);
                    if (val * sign >= max) {
                        if (x > 0) value[x - 1][y] = sign * (abs(value[x - 1][y]) + 1);
                        if (x < size - 1) value[x + 1][y] = sign * (abs(value[x + 1][y]) + 1);
                        if (y > 0) value[x][y - 1] = sign * (abs(value[x][y - 1]) + 1);
                        if (y < size - 1) value[x][y + 1] = sign * (abs(value[x][y + 1]) + 1);
                        value[x][y] -= sign * max;
                        continue x;
                    }
                }
            }
            break;
        }
    }

    public boolean isEmpty() {
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                if (value[x][y] != 0) return false;
        return true;
    }

    public @CheckForNull Player findWinningPlayerOrNull() {
        boolean negativeIsPresent = false;
        boolean positiveIsPresent = false;
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                int val = value[x][y];
                if (val < 0) negativeIsPresent = true;
                if (val > 0) positiveIsPresent = true;
            }
        }
        if (negativeIsPresent && positiveIsPresent) return null;
        if (negativeIsPresent) return Player.negative;
        if (positiveIsPresent) return Player.positive;
        return null;
    }

    public @Nonnull Board clone() {
        val result = new Board();
        for (int x = 0; x < size; x++)
            System.arraycopy(value[x], 0, result.value[x], 0, size);
        return result;
    }

    @Override public @Nonnull String toString() {
        val result = new StringBuilder();
        result.append("\n");
        result.append("    |");
        for (int x = 0; x < size; x++)
            result.append(String.format("%2d ", x));
        result.append("\n");
        result.append("----+");
        for (int x = 0; x < size; x++)
            result.append("---");
        result.append("\n");
        for (int y = 0; y < size; y++) {
            result.append(String.format("%3d |", y));
            for (int x = 0; x < size; x++)
                result.append(String.format("%2d ", value[x][y]));
            result.append("\n");
        }
        result.append("\n");
        return result.toString();
    }
}
