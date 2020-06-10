package minilisp.Atom;

import minilisp.Lexer.TokenType;

public class Boolean extends Atom {
    boolean value;

    public Boolean(boolean value) {
        this.value = value;
    }

    public Boolean(String value) {
        if (value.charAt(1) == 'T' || value.charAt(1) == 't') {
            this.value = true;
        } else {
            this.value = false;
        }
    }

    @Override
    public String toString() {
        return String.valueOf(this.value);
    }

    public void negate() {
        this.value = !this.value;
    }

    public boolean getValue() {
        return this.value;
    }

    public void apply(TokenType operator, Boolean bool) {
        switch(operator) {
            case OR : this.value |= bool.value; break;
            case AND: this.value &= bool.value; break;
        }
    }
}
