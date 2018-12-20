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
  uint64_t position;
  std::string const &str;

  int8_t ParseDigit();
  
public:
  explicit Parser(std::string const &str) : line{1}, column{1}, position{0}, str{str} {}

  std::string Peek();
  std::string Current();
  void SkipWhitespace();
  bool IsEOF();
  void Next();
  
  std::string ParseNumeric();
};
