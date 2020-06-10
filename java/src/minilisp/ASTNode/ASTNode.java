package minilisp.ASTNode;

import minilisp.Atom.*;
import minilisp.Environment;

import java.util.ArrayList;

public abstract class ASTNode {
    ArrayList<ASTNode> child;

    ASTNode() {
        this.child = new ArrayList<>();
    }

    @Override
    public String toString() {
        return "just an ASTNode";
    }

    public void append(ASTNode node) {
        if (node != null) {
            this.child.add(node);
        }
    }

    public abstract Atom evaluate(Environment env);

}
