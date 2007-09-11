package org.kohsuke.diyorb.impl;

import antlr.Parser;
import antlr.SemanticException;
import antlr.Token;
import antlr.ParserSharedInputState;
import antlr.LLkParser;
import antlr.TokenBuffer;
import antlr.TokenStream;

import java.util.Map;
import java.util.HashMap;

/**
 * Defines methods to support {@link ProgramParser}.
 *
 * <p>
 * ... so that I don't have to write too much Java code in ANTLR source file.
 *
 * @author Kohsuke Kawaguchi
 */
abstract class BaseParser extends LLkParser {
    public final Map<String,Color> namedColors = new HashMap<String,Color>();

    protected BaseParser(int i) {
        super(i);
    }

    protected BaseParser(ParserSharedInputState parserSharedInputState, int i) {
        super(parserSharedInputState, i);
    }

    protected BaseParser(TokenBuffer tokenBuffer, int i) {
        super(tokenBuffer, i);
    }

    protected BaseParser(TokenStream tokenStream, int i) {
        super(tokenStream, i);
    }

    protected void addNamedColor(Token name,Color c) throws SemanticException {
        if(namedColors.containsKey(name.getText()))
            throw new SemanticException("Duplicate definition of color "+name.getText(),
                name.getFilename(), name.getLine(), name.getColumn());
        namedColors.put(name.getText(),c);
    }

    protected Color getNamedColor(Token name) throws SemanticException {
        Color c = namedColors.get(name.getText());
        if(c!=null)     return c;

        throw new SemanticException("Undefined color "+name.getText(),
            name.getFilename(), name.getLine(), name.getColumn());
    }
}
