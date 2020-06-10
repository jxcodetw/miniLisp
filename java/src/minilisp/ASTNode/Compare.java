package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Integer;
import minilisp.Environment;
import minilisp.Lexer.TokenType;

public class Compare extends ASTNode {
    TokenType operator;

    public Compare(TokenType operator) {
        super();
        this.operator = operator;
    }

    @Override
    public Atom evaluate(Environment env) {
        boolean result = true;
        Integer integer = (Integer)this.child.get(0).evaluate(env), tmp;
        for(int i=1;i<this.child.size();++i) {
            tmp = (Integer)this.child.get(i).evaluate(env);
            switch(this.operator) {
                case LE: result &= integer.getValue() <= tmp.getValue(); break;
                case GE: result &= integer.getValue() >= tmp.getValue(); break;
                case LT: result &= integer.getValue() <  tmp.getValue(); break;
                case GT: result &= integer.getValue() <  tmp.getValue(); break;
                case EQ: result &= integer.getValue() == tmp.getValue(); break;
            }
        }
        return new minilisp.Atom.Boolean(result);
    }
}
