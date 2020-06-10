package minilisp;

import minilisp.ASTNode.*;
import minilisp.Atom.Atom;
import minilisp.Lexer.Lexer;
import minilisp.Lexer.Token;
import minilisp.Lexer.TokenType;

public class Parser {
    Lexer lexer;

    public Parser(Lexer lexer) {
        this.lexer = lexer;
    }

    private ASTNode parseList() {
        ASTNode node;
        Token token = this.lexer.nextToken();
        switch(token.type) {
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case MOD:
                Arithmetic ath = new Arithmetic(token.type);
                while ((node = this.parse()) != null) {
                    ath.append(node);
                }
                return ath;
            case NOT:
            case AND:
            case OR:
                Logical lg = new Logical(token.type);
                while ((node = this.parse()) != null) {
                    lg.append(node);
                }
                return lg;
            case LE:
            case GE:
            case LT:
            case GT:
            case EQ:
                Compare cmp = new Compare(token.type);
                while ((node = this.parse()) != null) {
                    cmp.append(node);
                }
                return cmp;
            case IF:
                If myif = new If();
                myif.append(this.parse());
                myif.append(this.parse());
                myif.append(this.parse());
                return myif;
            case LET:
                token = this.lexer.nextToken();
                Let let = new Let(token.data, this.parse());
                return let;
            case LAMBDA:
                Lambda lambda = new Lambda();
                lexer.nextToken(); // trim (
                while((token = lexer.nextToken()) != null && token.type != TokenType.RB) {
                    lambda.addArg(token.data);
                }
                while ((node = this.parse()) != null) {
                    lambda.addBody(node);
                }
                return lambda;
            default:
                Func func;
                if (token.type == TokenType.LB) {
                    func = new Func(this.parseList());
                } else {
                    func = new Func(new Variable(token.data));
                }
                while ((node = this.parse()) != null) {
                    func.addArg(node);
                }
                return func;
        }
    }

    public ASTNode parse() {
        Token token = this.lexer.nextToken();
        switch(token.type) {
            case LB:
                return parseList();
            case NUMBER:
                Atom integer = new minilisp.Atom.Integer(token.data);
                return new Literal(integer);
            case BOOLEAN:
                Atom bool = new minilisp.Atom.Boolean(token.data);
                return new Literal(bool);
            case SYMBOL:
                return new Variable(token.data);
        }
        return null;
    }
}
