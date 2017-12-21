package com.databasesandlife.explode;

import javax.annotation.Nonnull;

import static java.lang.Integer.MAX_VALUE;
import static java.lang.Integer.MIN_VALUE;

public enum Player {
    negative(-1) {
        public @Nonnull Player getOther() { return positive; }
    },
    positive(+1) {
        public @Nonnull Player getOther() { return negative; }
    };

    final int sign;

    Player(int s) { sign = s;}

    public int getMinimum() { return -sign * 10_000; }
    public int getMaximum() { return +sign * 10_000; }

    public abstract @Nonnull Player getOther();
}
