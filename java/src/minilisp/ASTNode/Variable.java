package minilisp.ASTNode;

import minilisp.Atom.*;
import minilisp.Environment;

public class Variable extends ASTNode {
    String name;

    public Variable(String name) {
        super();
        this.name = name;
    }

    public Atom evaluate(Environment env) {
        return (Atom)env.find(this.name).clone();
    }
}
