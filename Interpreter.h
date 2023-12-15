#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

enum class TokenType {
    IDENTIFIER,
    LITERAL,
    OPERATOR,
    SEMICOLON,
    LPAREN,
    RPAREN,
    END
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// Node structure for the abstract syntax tree
struct Node {
    TokenType type;
    string value;
    vector<Node> children;
};

// Lexer function
vector<Token> lex(const string& input) {
    vector<Token> tokens;
    istringstream stream(input);
    char c;

    while (stream.get(c)) {
        if (isspace(c)) {
            continue; // Ignore whitespace
        }
        else if (isalpha(c) || c == '_') {
            string identifier;
            identifier += c;
            while (stream.get(c) && (isalnum(c) || c == '_')) {
                identifier += c;
            }
            stream.unget();
            tokens.push_back({ TokenType::IDENTIFIER, identifier });
        }
        else if (isdigit(c)) {
            string literal;
            literal += c;
            while (stream.get(c) && isdigit(c)) {
                literal += c;
            }
            stream.unget();
            tokens.push_back({ TokenType::LITERAL, literal });
        }
        else if (c == '+' || c == '-' || c == '*' || c == '=') {
            tokens.push_back({ TokenType::OPERATOR, string(1, c) });
        }
        else if (c == ';') {
            tokens.push_back({ TokenType::SEMICOLON, ";" });
        }
        else if (c == '(') {
            tokens.push_back({ TokenType::LPAREN, "(" });
        }
        else if (c == ')') {
            tokens.push_back({ TokenType::RPAREN, ")" });
        }
        else {
            throw invalid_argument("Invalid character: " + string(1, c));
        }
    }

    tokens.push_back({ TokenType::END, "" });
    return tokens;
}

// Parser functions
Node parseExp(vector<Token>& tokens);
Node parseTerm(vector<Token>& tokens);
Node parseFact(vector<Token>& tokens);
Node parseIdentifier(vector<Token>& tokens);
Node parseLiteral(vector<Token>& tokens);

Node parseExp(vector<Token>& tokens) {
    Node exp = parseTerm(tokens);

    while (tokens.front().type == TokenType::OPERATOR && (tokens.front().value == "+" || tokens.front().value == "-")) {
        Node opNode = { tokens.front().type, tokens.front().value };
        tokens.erase(tokens.begin());
        Node term = parseTerm(tokens);
        opNode.children = { exp, term };
        exp = opNode;
    }

    return exp;
}

Node parseTerm(vector<Token>& tokens) {
    Node term = parseFact(tokens);

    while (tokens.front().type == TokenType::OPERATOR && tokens.front().value == "*") {
        Node opNode = { tokens.front().type, tokens.front().value };
        tokens.erase(tokens.begin());
        Node fact = parseFact(tokens);
        opNode.children = { term, fact };
        term = opNode;
    }

    return term;
}

Node parseFact(vector<Token>& tokens) {
    if (tokens.front().type == TokenType::LPAREN) {
        tokens.erase(tokens.begin());
        Node exp = parseExp(tokens);
        if (tokens.front().type != TokenType::RPAREN) {
            throw invalid_argument("Expected closing parenthesis");
        }
        tokens.erase(tokens.begin());
        return exp;
    }
    else if (tokens.front().type == TokenType::OPERATOR && (tokens.front().value == "+" || tokens.front().value == "-")) {
        string unaryOperator = tokens.front().value;
        tokens.erase(tokens.begin());
        Node fact = parseFact(tokens);
        Node opNode = { TokenType::OPERATOR, unaryOperator };
        opNode.children = { fact };
        return opNode;
    }
    else if (tokens.front().type == TokenType::LITERAL) {
        Node literal = { tokens.front().type, tokens.front().value };
        tokens.erase(tokens.begin());
        return literal;
    }
    else if (tokens.front().type == TokenType::IDENTIFIER) {
        return parseIdentifier(tokens);
    }
    else {
        throw invalid_argument("Invalid token in fact");
    }
}

Node parseIdentifier(vector<Token>& tokens) {
    if (tokens.front().type == TokenType::IDENTIFIER) {
        Node identifier = { tokens.front().type, tokens.front().value };
        tokens.erase(tokens.begin());
        return identifier;
    }
    else {
        throw invalid_argument("Expected identifier");
    }
}

Node parseLiteral(vector<Token>& tokens) {
    if (tokens.front().type == TokenType::LITERAL) {
        Node literal = { tokens.front().type, tokens.front().value };
        tokens.erase(tokens.begin());
        return literal;
    }
    else {
        throw invalid_argument("Expected literal");
    }
}
class Interpreter
{
public:
    void interpret(const string& program) {
        vector<Token> tokens = lex(program);
        Node result;
        try {
            result = parseProgram(tokens);
            execute(result);
        }
        catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

private:
    unordered_map<string, int> variables;

    Node parseProgram(vector<Token>& tokens) {
        Node program;
        while (tokens.front().type != TokenType::END) {
            program.children.push_back(parseAssignment(tokens));
        }
        return program;
    }

    Node parseAssignment(vector<Token>& tokens) {
        Node assignment = { TokenType::OPERATOR, "=" };
        assignment.children.push_back(parseIdentifier(tokens));

        if (tokens.front().type == TokenType::OPERATOR && tokens.front().value == "=") {
            tokens.erase(tokens.begin());
        }
        else {
            throw invalid_argument("Expected assignment operator");
        }

        assignment.children.push_back(parseExp(tokens));

        if (tokens.front().type == TokenType::SEMICOLON) {
            tokens.erase(tokens.begin());
        }
        else {
            throw invalid_argument("Expected semicolon");
        }

        return assignment;
    }

    void execute(const Node& program) {
        for (const auto& assignment : program.children) {
            string identifier = assignment.children[0].value;
            int value = evaluate(assignment.children[1]);
            variables[identifier] = value;
        }

        for (const auto& entry : variables) {
            cout << entry.first << " = " << entry.second << endl;
        }
    }

    int evaluate(const Node& node) {
        if (node.type == TokenType::LITERAL) {
            return stoi(node.value);
        }
        else if (node.type == TokenType::IDENTIFIER) {
            if (variables.find(node.value) != variables.end()) {
                return variables[node.value];
            }
            else {
                throw invalid_argument("Variable '" + node.value + "' is uninitialized");
            }
        }
        else if (node.type == TokenType::OPERATOR) {
            if (node.value == "+") {
                return evaluate(node.children[0]) + evaluate(node.children[1]);
            }
            else if (node.value == "-") {
                if (node.children.size() == 1) {
                    return -evaluate(node.children[0]); // Unary minus
                }
                else if (node.children.size() == 2) {
                    return evaluate(node.children[0]) - evaluate(node.children[1]); //Binary minus
                }
                else {
                    throw invalid_argument("Invalid number of children for operator '-'.");
                }
            }
            else if (node.value == "*") {
                return evaluate(node.children[0]) * evaluate(node.children[1]);
            }
            else {
                throw invalid_argument("Invalid operator: " + node.value);
            }
        }
        else {
            throw invalid_argument("Invalid node type");
        }
    }
};

