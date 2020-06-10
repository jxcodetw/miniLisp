package minilisp.Lexer;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Lexer {
    Matcher matcher;

    public Lexer(String input) {
        StringBuffer tokenPatternsBuffer = new StringBuffer();
        for (TokenType tokenType : TokenType.values()) {
            tokenPatternsBuffer.append(String.format("|(?<%s>%s)", tokenType.name(), tokenType.pattern));
        }
        Pattern tokenPatterns = Pattern.compile(new String(tokenPatternsBuffer.substring(1)));

        this.matcher = tokenPatterns.matcher(input);
    }

    public Token nextToken() {
        String matched;
        while (matcher.find()) {
            for (TokenType tokenType : TokenType.values()) {
                if (tokenType != TokenType.WHITESPACE) {
                    matched = matcher.group(tokenType.name());
                    if (matched != null) {
                        return new Token(tokenType, matched);
                    }
                }
            }
        }
        return null;
    }
}
