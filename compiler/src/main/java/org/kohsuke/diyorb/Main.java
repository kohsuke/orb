package org.kohsuke.diyorb;

import org.kohsuke.diyorb.impl.Program;
import org.kohsuke.diyorb.impl.ProgramLexer;
import org.kohsuke.diyorb.impl.ProgramParser;

/**
 * @author Kohsuke Kawaguchi
 */
public class Main {
    public static void main(String[] args) throws Exception {
        ProgramParser pp = new ProgramParser(new ProgramLexer(System.in));
        Program p = pp.program();
        p.writeTo(System.out);
    }
}
