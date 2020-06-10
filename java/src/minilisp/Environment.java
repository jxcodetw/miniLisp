package minilisp;

import minilisp.Atom.*;

import java.util.HashMap;

public class Environment {
    HashMap<String, Atom> env;

    public Environment() {
        this.env = new HashMap<>();
    }

    public Environment(Environment oldenv) {
        this.env = new HashMap<>(oldenv.env);
    }

    public void extend(String name, Atom value) {
        this.env.put(name, value);
    }

    public Atom find(String name) {
        return this.env.get(name);
    }
}
