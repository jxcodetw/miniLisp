package minilisp.Lexer;

public enum TokenType {

    LB("[(]"),
    RB("[)]"),
    NUMBER("[+-]?[0-9]+"),
    BOOLEAN("#[TtFf]"),
    ADD("\\+"),
    SUB("\\-"),
    MUL("\\*"),
    DIV("\\/"),
    MOD("\\%"),
    LE("<="),
    GE(">="),
    LT("<"),
    GT(">"),
    EQ("="),
    OR("or"),
    NOT("not"),
    AND("and"),
    IF("if"),
    LET("let"),
    LAMBDA("lambda"),
    SYMBOL("[a-zA-Z0-9_]+"),
    WHITESPACE("[ \t\f\r\n]+");

    public final String pattern;

    TokenType(String pattern) {
        this.pattern = pattern;
    }
}
