package org.kohsuke.diyorb.impl;

import java.io.OutputStream;
import java.io.IOException;
import java.io.DataOutputStream;
import java.io.ByteArrayOutputStream;

/**
 * @author Kohsuke Kawaguchi
 */
public class Program {
    public final Statement body;

    public Program(Statement body) {
        this.body = body;
    }

    /**
     * Generates the binary.
     */
    public void writeTo(OutputStream out) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        body.writeTo(dos);
        dos.writeByte(3<<4);    // halt
        dos.close();

        dos = new DataOutputStream(out);
        dos.writeInt(-1);  // magic
        if(baos.size()>256)
            throw new IllegalArgumentException("Program is too large");
        dos.writeByte(baos.size());
        dos.write(baos.toByteArray());
        dos.close();
    }
}
