package com.databasesandlife.explode;

import javax.annotation.Nonnull;

import static java.lang.Integer.MAX_VALUE;
import static java.lang.Integer.MIN_VALUE;

public enum Player {
    negative(-1) {
        public @Nonnull Player getOther() { return positive; }
        public int getMinimum() { return MAX_VALUE; }
        public int getMaximum() { return MIN_VALUE; }
    },
    positive(+1) {
        public @Nonnull Player getOther() { return negative; }
        public int getMinimum() { return MIN_VALUE; }
        public int getMaximum() { return MAX_VALUE; }
    };

    final int sign;

    Player(int s) { sign = s; }

    public abstract @Nonnull Player getOther();
    public abstract int getMinimum();
    public abstract int getMaximum();
}
