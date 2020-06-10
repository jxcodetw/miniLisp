package minilisp;

import minilisp.ASTNode.ASTNode;
import minilisp.Atom.Atom;
import minilisp.Lexer.Lexer;

import java.io.IOException;

public class Main {

    public static void interp(String input, Environment env) {
        Lexer lexer = new Lexer(input);
        Parser parser = new Parser(lexer);
        ASTNode parseTree = parser.parse();
        Atom result = parseTree.evaluate(env);
        if (result != null) {
            System.out.println(result);
        }
    }

    public static void main(String[] args) throws IOException {
        Environment env = new Environment();
        interp("(let add_x (lambda (x) (lambda (y) (+ x y))))", env);
        interp("(let z (add_x 10)", env);
        interp("(z 1)", env);
    }
}
