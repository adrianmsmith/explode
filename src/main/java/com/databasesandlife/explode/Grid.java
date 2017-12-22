package com.databasesandlife.explode;

import lombok.val;

import javax.annotation.CheckForNull;
import javax.annotation.Nonnull;

import static java.lang.Integer.signum;
import static java.lang.Math.abs;

class Grid implements Cloneable {

    public static final int width = 6;

    int[][] value = new int[width][width];

    void explode() {
        x: while (true) {
            if (findWinningPlayerOrNull() != null) return;
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

    public @CheckForNull Player findWinningPlayerOrNull() {
        boolean negativeIsPresent = false;
        boolean positiveIsPresent = false;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < width; y++) {
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

    public @Nonnull Grid clone() {
        val result = new Grid();
        for (int x = 0; x < width; x++)
            System.arraycopy(value[x], 0, result.value[x], 0, width);
        return result;
    }

    @Override public @Nonnull String toString() {
        val result = new StringBuilder();
        result.append("\n");
        result.append("    |");
        for (int x = 0; x < width; x++)
            result.append(String.format("%2d ", x));
        result.append("\n");
        result.append("----+");
        for (int x = 0; x < width; x++)
            result.append("---");
        result.append("\n");
        for (int y = 0; y < width; y++) {
            result.append(String.format("%3d |", y));
            for (int x = 0; x < width; x++)
                result.append(String.format("%2d ", value[x][y]));
            result.append("\n");
        }
        result.append("\n");
        return result.toString();
    }
}
