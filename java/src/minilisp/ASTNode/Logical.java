package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Boolean;
import minilisp.Environment;
import minilisp.Lexer.TokenType;

public class Logical extends ASTNode {
    TokenType operator;

    public Logical(TokenType operator) {
        super();
        this.operator = operator;
    }

    @Override
    public Atom evaluate(Environment env) {
        Boolean val = (Boolean)this.child.get(0).evaluate(env);
        if (operator == TokenType.NOT) {
            val.negate();
            return val;
        }
        for(int i=1;i<this.child.size();++i) {
            val.apply(this.operator, (Boolean) this.child.get(i).evaluate(env));
        }
        return val;
    }

}
