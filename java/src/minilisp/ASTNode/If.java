package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Boolean;
import minilisp.Environment;

public class If extends ASTNode {
    @Override
    public Atom evaluate(Environment env) {
        Boolean choice = (Boolean)this.child.get(0).evaluate(env);
        if (choice.getValue()) {
            return this.child.get(1).evaluate(env);
        } else {
            return this.child.get(2).evaluate(env);
        }
    }
}
