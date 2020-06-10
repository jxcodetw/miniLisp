package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Closure;
import minilisp.Environment;

public class Let extends ASTNode {
    String name;
    ASTNode bind;

    public Let(String name, ASTNode bind) {
        this.name = name;
        this.bind = bind;
    }

    @Override
    public Atom evaluate(Environment env) {
        Atom eval = this.bind.evaluate(env);
        if (eval instanceof Closure) {
            ((Closure) eval).extend(this.name);
        }
        env.extend(this.name, eval);
        return null;
    }
}
