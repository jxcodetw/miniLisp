package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Closure;
import minilisp.Environment;

import java.util.ArrayList;

public class Func extends ASTNode {
    ASTNode func;
    ArrayList<ASTNode> args;

    public Func(ASTNode func) {
        this.func = func;
        this.args = new ArrayList<>();
    }

    public void addArg(ASTNode arg) {
        this.args.add(arg);
    }

    @Override
    public Atom evaluate(Environment env) {
        Closure closure = (Closure)func.evaluate(env);
        return closure.call(this.args, env);
    }
}
