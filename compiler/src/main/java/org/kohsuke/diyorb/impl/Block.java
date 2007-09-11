package org.kohsuke.diyorb.impl;

import java.util.ArrayList;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Compiled program.
 *
 * @author Kohsuke Kawaguchi
 */
public class Block extends ArrayList<Statement> implements Statement {
    public String toString() {
        StringBuilder buf = new StringBuilder();
        buf.append("{");
        for (Statement s : this) {
            buf.append(s);
        }
        buf.append("}");
        return buf.toString();
    }

    public void writeTo(DataOutput o) throws IOException {
        for (Statement s : this)
            s.writeTo(o);
    }
}
