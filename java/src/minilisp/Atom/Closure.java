package minilisp.Atom;

import minilisp.ASTNode.ASTNode;
import minilisp.Environment;

import java.util.ArrayList;

public class Closure extends Atom {
    Environment env;
    ArrayList<String> args;
    ArrayList<ASTNode> bodies;

    public Closure(ArrayList<String> args, ArrayList<ASTNode> bodies, Environment env) {
        this.args = args;
        this.bodies = bodies;
        this.env = new Environment(env);
    }

    public void extend(String name) {
        this.env.extend(name, this);
    }

    public Atom call(ArrayList<ASTNode> argvals, Environment env) {
        Atom result = null;
        for(int i=0;i<this.args.size();++i) {
            this.env.extend(this.args.get(i), argvals.get(i).evaluate(env));
        }
        for(ASTNode body : this.bodies) {
            result = body.evaluate(this.env);
        }
        return result;
    }

    @Override
    public String toString() {
        return "Closure";
    }
}
