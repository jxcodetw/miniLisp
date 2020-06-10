package minilisp.ASTNode;

import minilisp.Atom.*;
import minilisp.Environment;

public class Literal extends ASTNode {
    Atom atom;

    public Literal(Atom atom) {
        super();
        this.atom = atom;
    }

    public Atom evaluate(Environment env) {
        return this.atom;
    }
}
