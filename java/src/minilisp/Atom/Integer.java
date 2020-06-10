package minilisp.Atom;

import minilisp.Lexer.TokenType;

public class Integer extends Atom {
    int value;

    public Integer(String value) {
        this.value = java.lang.Integer.valueOf(value);
    }

    public int getValue() {
        return this.value;
    }

    @Override
    public String toString() {
        return String.valueOf(this.value);
    }

    public void apply(TokenType operator, Integer integer) {
        switch(operator) {
            case ADD: this.value += integer.value; break;
            case SUB: this.value -= integer.value; break;
            case MUL: this.value *= integer.value; break;
            case DIV: this.value /= integer.value; break;
            case MOD: this.value %= integer.value; break;
        }
    }
}
