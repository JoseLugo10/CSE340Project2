/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

struct REG
{
    struct REG_node *start;
    struct REG_node *accept;
};

struct REG_node
{
    struct REG_node *firstNeighbor;
    char first_label;
    struct REG_node *secondNeighbor;
    char second_label;
};

struct InputToken
{
    std::string name;
    struct REG *graph;
};

class Parser {
  public:
    void parseInput();
    void parseTokens_Section();
    void parseToken_List();
    void parseToken();
    struct REG* parseExpr();

  private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
};

class my_LexicalAnalyzer {
    int a;
    int b;
};

#endif

