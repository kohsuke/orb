// $ANTLR 2.7.4: "language.g" -> "ProgramParser.java"$

  package org.kohsuke.diyorb.impl;

import antlr.TokenBuffer;
import antlr.TokenStreamException;
import antlr.TokenStreamIOException;
import antlr.ANTLRException;
import antlr.LLkParser;
import antlr.Token;
import antlr.TokenStream;
import antlr.RecognitionException;
import antlr.NoViableAltException;
import antlr.MismatchedTokenException;
import antlr.SemanticException;
import antlr.ParserSharedInputState;
import antlr.collections.impl.BitSet;

public class ProgramParser extends BaseParser       implements ProgramParserTokenTypes
 {

protected ProgramParser(TokenBuffer tokenBuf, int k) {
  super(tokenBuf,k);
  tokenNames = _tokenNames;
}

public ProgramParser(TokenBuffer tokenBuf) {
  this(tokenBuf,1);
}

protected ProgramParser(TokenStream lexer, int k) {
  super(lexer,k);
  tokenNames = _tokenNames;
}

public ProgramParser(TokenStream lexer) {
  this(lexer,1);
}

public ProgramParser(ParserSharedInputState state) {
  super(state,1);
  tokenNames = _tokenNames;
}

	public final Program  program() throws RecognitionException, TokenStreamException, ANTLRException {
		Program p;
		
		Token  id = null;
		
		Statement s;
		int r,g,b;
		Block blk = new Block();
		p = new Program(blk);
		
		
		{
		_loop3:
		do {
			if ((LA(1)==TOKEN)) {
				id = LT(1);
				match(TOKEN);
				match(5);
				match(6);
				r=number();
				match(7);
				g=number();
				match(7);
				b=number();
				match(8);
				match(9);
				
				addNamedColor(id,new Color(r,g,b));
				
			}
			else {
				break _loop3;
			}
			
		} while (true);
		}
		{
		_loop5:
		do {
			if ((LA(1)==10||LA(1)==LITERAL_repeat||LA(1)==LITERAL_become)) {
				s=statement();
				blk.add(s);
			}
			else {
				break _loop5;
			}
			
		} while (true);
		}
		match(Token.EOF_TYPE);
		return p;
	}
	
	public final int  number() throws RecognitionException, TokenStreamException {
		int value;
		
		Token  n = null;
		
		n = LT(1);
		match(NUMBER);
		
		value = Integer.parseInt(n.getText());
		
		return value;
	}
	
	public final Statement  statement() throws RecognitionException, TokenStreamException {
		Statement s;
		
		
		Statement t;
		int n=0;
		Color c;
		
		
		switch ( LA(1)) {
		case 10:
		{
			match(10);
			
			Block b=new Block();
			s = b;
			
			{
			_loop8:
			do {
				if ((LA(1)==10||LA(1)==LITERAL_repeat||LA(1)==LITERAL_become)) {
					t=statement();
					b.add(t);
				}
				else {
					break _loop8;
				}
				
			} while (true);
			}
			match(11);
			break;
		}
		case LITERAL_repeat:
		{
			match(LITERAL_repeat);
			{
			switch ( LA(1)) {
			case NUMBER:
			{
				n=number();
				break;
			}
			case 10:
			case LITERAL_repeat:
			case LITERAL_become:
			{
				break;
			}
			default:
			{
				throw new NoViableAltException(LT(1), getFilename());
			}
			}
			}
			t=statement();
			
			s = new Loop(n,t);
			
			break;
		}
		case LITERAL_become:
		{
			match(LITERAL_become);
			c=color();
			match(LITERAL_in);
			n=number();
			match(9);
			
			s = new Become(c,n);
			
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		return s;
	}
	
	public final Color  color() throws RecognitionException, TokenStreamException {
		Color c;
		
		Token  t = null;
		
		int r,g,b;
		
		
		switch ( LA(1)) {
		case 6:
		{
			match(6);
			r=number();
			match(7);
			g=number();
			match(7);
			b=number();
			match(8);
			
			c = new Color(r,g,b);
			
			break;
		}
		case TOKEN:
		{
			t = LT(1);
			match(TOKEN);
			
			c = getNamedColor(t);
			
			break;
		}
		default:
		{
			throw new NoViableAltException(LT(1), getFilename());
		}
		}
		return c;
	}
	
	public final String  token() throws RecognitionException, TokenStreamException {
		String value;
		
		Token  t = null;
		
		t = LT(1);
		match(TOKEN);
		
		value = t.getText();
		
		return value;
	}
	
	
	public static final String[] _tokenNames = {
		"<0>",
		"EOF",
		"<2>",
		"NULL_TREE_LOOKAHEAD",
		"TOKEN",
		"\"=\"",
		"\"(\"",
		"\",\"",
		"\")\"",
		"\";\"",
		"\"{\"",
		"\"}\"",
		"\"repeat\"",
		"\"become\"",
		"\"in\"",
		"NUMBER",
		"WS",
		"MINUS",
		"STAR",
		"DIV",
		"OR",
		"EQUAL",
		"LPAREN",
		"RPAREN",
		"SEMI",
		"LBRA",
		"RBRA"
	};
	
	
	}
