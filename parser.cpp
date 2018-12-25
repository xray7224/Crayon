#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include "parser.h"

void ASTNode::SetLeft(ASTNode& l) {
  left = &l;
}

void ASTNode::SetRight(ASTNode& r) {
    right = &r;
    // Is this the instance?
    r.SetLeft(*this);
  }

ASTNode* ASTNode::Next() {
  return right;
}

Parser::Parser(std::string init) {
  line = 1;
  column = 1;
  str = init;
  position = str.begin();
}

bool Parser::IsEOF() {
  return position >= str.end();
}

char Parser::Peek() {
  if (IsEOF()) {
    return '\0';
  }

  return *position;
}

char Parser::Current() {
  return *position;
}

void Parser::SkipWhitespace() {
  while (Current() == ' ' || Current() == ' ') {
    position++;
  }
}

void Parser::Advance(uint64_t amount) {
  for (; amount > 0; amount--) { 
    if (*position == '\n') {
      column = 1;
      line++;
    } else {
      column++;
    }
    position++;
  }
}


std::string Parser::ParseNumeric() {
  // This matches:
  // - hex (base 16, e.g. 0x123456789ABCDEF)
  // - decimal (prefixed with a 0, e.g. 0123456789)
  // - dozenal (no prefix e.g. 123456789XE) (X = 010, E = 011)
  std::regex valid("0x[0-9|A-F]+|0[0-9]+|[1-9XE]+[0-9XE]*",
		   std::regex_constants::ECMAScript);

  auto first = std::sregex_iterator(position, str.end(), valid);
  auto empty = std::sregex_iterator();

  // Have we found a number?
  if (std::distance(first, empty) == 0) {
    return std::string();
  }

  std::smatch number = *first;
  position += number.length();
  std::string result = number.str();
  
  // Convert our notation for dozenal numbers to the standard.
  // Note this will break hex numbers. We should fix it.
  if (result.substr(0, 1) != "0") {
    // It's a dozenal number.
    std::replace(result.begin(), result.end(), 'X', 'A');
    std::replace(result.begin(), result.end(), 'E', 'B');
  }
  return result;
}

int main() {
  Parser parser{Parser{"X72E0+010-0xFF*0x22/X"}};
  ASTNode *initial = nullptr;
  // Okay cool. Lets do some parsing into the AST.
  ASTNode *current = nullptr;
  while (!parser.IsEOF()) {
    ASTNode *token;
    switch (parser.Current()) {
    case '+':
      token = new ASTNode{"+", ASTNodeType::MathOperator};
      parser.Advance(1);
      break;
    case '-':
      token = new ASTNode{"-", ASTNodeType::MathOperator};
      parser.Advance(1);
      break;
    case '*':
      token = new ASTNode{"*", ASTNodeType::MathOperator};
      parser.Advance(1);
      break;
    case '/':
      token = new ASTNode{"/", ASTNodeType::MathOperator};
      parser.Advance(1);
      break;
    default:
      token = new ASTNode{parser.ParseNumeric(), ASTNodeType::Number};      
    }

    if (initial == nullptr) {
      initial = token;
    } else {
      current->SetRight(*token);
    }
    current = token;
  }

  current = initial;
  do {
    std::cout << "Token: " << current->name << std::endl;
  } while ((current = current->Next()));

  return 0;
}
