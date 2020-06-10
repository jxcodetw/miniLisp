package minilisp.ASTNode;

import minilisp.Atom.Atom;
import minilisp.Atom.Closure;
import minilisp.Environment;

import java.util.ArrayList;

public class Lambda extends ASTNode {
    ArrayList<String> args;
    ArrayList<ASTNode> body;

    public Lambda() {
        super();
        this.args = new ArrayList<>();
        this.body = new ArrayList<>();
    }

    public void addArg(String arg) {
        this.args.add(arg);
    }

    public void addBody(ASTNode node) {
        this.body.add(node);
    }

    @Override
    public Atom evaluate(Environment env) {
        return new Closure(this.args, this.body, env);
    }
}
