#pragma once
#include <string>

enum class ASTNodeType {
			// Basic types
			Number,
			String,
			Boolean,
			
			// Language constructs
			Variable,
			Conditional,
			Node,
			Edge,
			
			// Math
			MathOperator,
};


class ASTNode {
private:
  ASTNode* left{nullptr};
  ASTNode* right{nullptr};
public:
  std::string name;
  ASTNodeType type;
  
  ASTNode(std::string const &name, ASTNodeType type) : name{name}, type{type} {}
  ~ASTNode() {}
  void SetLeft(ASTNode& left);
  void SetRight(ASTNode& right);
  ASTNode* Next();
};

class Parser {
private:
  uint32_t line;
  uint32_t column;
  std::string::iterator position;
  std::string str;

  int8_t ParseDigit();
  // This should maybe take regex?
  bool TakeWhile(const char*);
  
public:
  Parser(std::string);
  //explicit Parser(std::string init) : line{1}, column{1}, str{init}, position{str.begin()} {}

  char Peek();
  char Current();
  void SkipWhitespace();
  bool IsEOF();
  void Advance(uint64_t);
  
  std::string ParseNumeric();
};
