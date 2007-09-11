package org.kohsuke.diyorb.impl;

import java.io.DataOutput;
import java.io.IOException;
import java.io.DataOutputStream;
import java.io.ByteArrayOutputStream;

/**
 * @author Kohsuke Kawaguchi
 */
public class Loop implements Statement {
    /**
     * Loop count. 0 to indicate infinite loop.
     */
    public final int n;
    public final Statement body;

    public Loop(int n, Statement body) {
        if(n<0 || n>255)   throw new IllegalArgumentException("invalid loop count "+n);
        this.n = n;
        this.body = body;
    }

    public String toString() {
        if(n==0)
            return "loop "+body;
        else
            return "loop "+n+" "+body;
    }

    public void writeTo(DataOutput o) throws IOException {
        ByteArrayOutputStream child = new ByteArrayOutputStream();
        body.writeTo(new DataOutputStream(child));
        o.write(child.toByteArray());

        if(n>0) {
            o.writeShort(1<<12|child.size()); // instruction code: 1
            o.write(0);    // space for the counter
            o.write(n);    // repeat count
        } else {
            o.writeShort(2<<12|child.size());   // instruction code: 2
        }
    }
}
