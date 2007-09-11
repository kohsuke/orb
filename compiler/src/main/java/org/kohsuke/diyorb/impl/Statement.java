package org.kohsuke.diyorb.impl;

import java.io.DataOutput;
import java.io.IOException;

/**
 * @author Kohsuke Kawaguchi
 */
public interface Statement {
    /**
     * Generates the binary.
     */
    void writeTo(DataOutput o) throws IOException;
}
