header {
  package org.kohsuke.diyorb.impl;
}

class ProgramParser extends Parser("BaseParser");
options {
  defaultErrorHandler=false;
}

program
returns [Program p]
throws ANTLRException
{
  Statement s;
  int r,g,b;
  Block blk = new Block();
  p = new Program(blk);
}
  : (// named color declaration
      id:TOKEN "=" "(" r=number "," g=number "," b=number ")" ";"
      {
        addNamedColor(id,new Color(r,g,b));
      }
    )*
    (// program body
      s=statement {blk.add(s);}
    )*
    EOF
  ;

statement
returns [Statement s]
{
  Statement t;
  int n=0;
  Color c;
}
  // block
  : "{"
    {
      Block b=new Block();
      s = b;
    }
    ( t=statement {b.add(t);} )* "}"
  // loop
  | "repeat" (n=number)? t=statement
    {
      s = new Loop(n,t);
    }
  // set color
  | "become" c=color "in" n=number ";"
    {
      s = new Become(c,n);
    }
  ;

token
returns [String value]
  : t:TOKEN
  {
    value = t.getText();
  }
  ;

number
returns [int value]
  : n:NUMBER
  {
    value = Integer.parseInt(n.getText());
  }
  ;

color
returns [Color c]
{
  int r,g,b;
}
  : "(" r=number "," g=number "," b=number ")"
    {
      c = new Color(r,g,b);
    }
  | t:TOKEN
    {
      c = getNamedColor(t);
    }
  ;

class ProgramLexer extends Lexer;
options {
  // defaultErrorHandler=false;
}

TOKEN
  : ('A'..'Z'|'a'..'z') ('A'..'Z'|'a'..'z'|'0'..'9')+
  ;

NUMBER
  : ('0'..'9')+
  ;

WS
  : ( ' '
    | '\t'
    | '\r' '\n' { newline(); }
    | '\n'      { newline(); }
    )
    { $setType(Token.SKIP); } 
  ;


MINUS:  '-';
STAR: '*';
DIV:  '/';
OR:   ',';
EQUAL:  '=';
LPAREN: '(';
RPAREN: ')';
SEMI: ';';
LBRA: '{';
RBRA: '}';