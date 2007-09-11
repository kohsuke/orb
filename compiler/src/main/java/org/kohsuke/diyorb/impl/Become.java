package org.kohsuke.diyorb.impl;

import java.io.DataOutput;
import java.io.IOException;

/**
 * @author Kohsuke Kawaguchi
 */
public class Become implements Statement {
    public final Color c;
    public final int n;

    public Become(Color c, int n) {
        this.c = c;
        this.n = n;
    }

    public String toString() {
        return "become "+c+" in "+n+";";
    }

    public void writeTo(DataOutput o) throws IOException {
        o.write(pack(c.r));
        o.write(pack(c.g)<<4|pack(c.b));
        o.write(0xFF&n);
        o.write(0xFF&(n>>8));
    }

    private int pack(int x) {
        return (x/16)%16;
    }
}
