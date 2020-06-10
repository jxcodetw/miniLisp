package minilisp.ASTNode;

import minilisp.Atom.*;
import minilisp.Atom.Integer;
import minilisp.Environment;
import minilisp.Lexer.TokenType;

public class Arithmetic extends ASTNode {
    TokenType operator;

    public Arithmetic(TokenType operator) {
        super();
        this.operator = operator;
    }

    @Override
    public Atom evaluate(Environment env) {
        Integer val = (Integer)this.child.get(0).evaluate(env);
        for(int i=1;i<this.child.size();++i) {
            val.apply(this.operator, (Integer)this.child.get(i).evaluate(env));
        }
        return val;
    }
}
