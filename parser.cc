/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include <vector>

using namespace std;

vector<InputToken> tokenList;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing
void Parser::parseInput()
{
    Token token;
    parseTokens_Section();
    token = expect(INPUT_TEXT);
}

void Parser::parseTokens_Section()
{
    Token token;
    parseToken_List();
    token = expect(HASH);
}

void Parser::parseToken_List()
{
    Token token;

    parseToken();

    Token t1 = lexer.peek(1);

    if(t1.token_type == COMMA)
    {
        token = expect(COMMA);
        parseToken_List();
    }
    else if(t1.token_type == HASH)
    {
        return;
    }
    else
    {
        syntax_error();
    }
}

void Parser::parseToken()
{
    Token token;

    Token t1 = lexer.peek(1);

    if(t1.token_type == ID)
    {
        InputToken newToken;

        token = expect(ID);
        newToken.name = token.lexeme;

        REG *R = new REG();
        R = parseExpr();
        newToken.graph = R;

        tokenList.push_back(newToken);
    }
    else
    {
        syntax_error();
    }
}

struct REG* Parser::parseExpr()
{
    Token token;
    REG *R1 = new REG();

    Token t1 = lexer.peek(1);

    if(t1.token_type == CHAR)
    {
        token = expect(CHAR);

        // The REG and REG_node objects are created
        REG_node *n1 = new REG_node();
        REG_node *n2 = new REG_node();

        // We set the values for the two nodes in the regular expression
        n1->firstNeighbor = n2;
        n1->first_label = token.lexeme[0];
        n1->secondNeighbor = NULL;
        n2->firstNeighbor = NULL;
        n2->first_label = '_';
        n2->secondNeighbor = NULL;

        // We set the starting node and accepting node of the REG that is being returned
        R1->start = n1;
        R1->accept = n2;

        return R1;
    }
    else if(t1.token_type == LPAREN)
    {
        token = expect(LPAREN);
        R1 = parseExpr();
        token = expect(RPAREN);

        Token t2 = lexer.peek(1);

        if(t2.token_type == DOT)
        {
            token = expect(DOT);
            token = expect(LPAREN);
            REG *R2 = new REG();
            R2 = parseExpr();
            token = expect(RPAREN);

            R1->accept->firstNeighbor = R2->start;
            R1->accept = R2->accept;

            return R1;
        }
        else if(t2.token_type == OR)
        {
            token = expect(OR);
            token = expect(LPAREN);
            REG *R2 = new REG();
            R2 = parseExpr();
            token = expect(RPAREN);

            REG_node *n1 = new REG_node();
            REG_node *n2 = new REG_node();

            n1->firstNeighbor = R1->start;
            n1->first_label = '_';
            n1->secondNeighbor = R2->start;
            n1->second_label = '_';

            n2->firstNeighbor = NULL;
            n2->first_label = '_';
            n2->secondNeighbor = NULL;

            R1->start = n1;
            R1->accept = n2;

            return R1;
        }
        else if(t2.token_type == STAR)
        {
            token = expect(STAR);

            REG_node *n1 = new REG_node();
            REG_node *n2 = new REG_node();

            n1->firstNeighbor = R1->accept;
            n1->first_label = '_';
            n1->secondNeighbor = n2;
            n1->second_label = '_';

            n2->firstNeighbor = NULL;
            n2->first_label = '_';
            n2->secondNeighbor = NULL;

            R1->accept->secondNeighbor = R1->start;
            R1->accept->second_label = '_';

            R1->start = n1;
            R1->accept = n2;

            return R1;
        }

        return R1;
    }
    else
    {
        syntax_error();
    }

    return R1;
}

int main()
{
	// note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    Parser parser;

	parser.parseInput();

	cout << tokenList[0].name << endl;
}
