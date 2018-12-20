#include <iostream>
#include <string>
#include <algorithm>
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

bool Parser::IsEOF() {
  return position >= (str.length());
}

std::string Parser::Peek() {
  if (IsEOF()) {
    return "\0";
  }

  return str.substr(position+1, 1);
}

std::string Parser::Current() {
  return str.substr(position, 1);
}

void Parser::SkipWhitespace() {
  while (Current() == " " || Current() == "\t") {
    position++;
  }
}

void Parser::Next() {
  position++;
}


std::string Parser::ParseNumeric() {
  std::string result;
  // TODO: Only make once, const on class?
  std::string valid{"0123456789XE."};

  while (!IsEOF()) {
    if (valid.find(Current()) == std::string::npos) {
      break;
    } else {
      result.append(Current());
    }
    position++;
  }

  std::replace(result.begin(), result.end(), 'X', 'A');
  std::replace(result.begin(), result.end(), 'E', 'B');
  return result;
}

int main() {
  /////////////////////////////////////////////////////////////
  // ASTNode one{ASTNode{"1", ASTNodeType::Number}};	     //
  // ASTNode plus{ASTNode{"+", ASTNodeType::MathOperation}}; //
  // ASTNode two{ASTNode{"2", ASTNodeType::Number}};	     //
  // 							     //
  // one.SetRight(plus);				     //
  // plus.SetRight(two);				     //
  // 							     //
  // ASTNode* current{&one};				     //
  // do {						     //
  //   std::cout << "Token: " << current->name << std::endl; //
  // } while ((current = current->Next())); 		     //
  /////////////////////////////////////////////////////////////

  Parser parser{Parser{"X+1+2-72+E"}};
  ASTNode *initial = nullptr;
  // Okay cool. Lets do some parsing into the AST.
  ASTNode *current = nullptr;
  while (!parser.IsEOF()) {
    ASTNode *token;
    switch (parser.Current()[0]) {
    case '+':
      token = new ASTNode{"+", ASTNodeType::MathOperator};
      parser.Next();
      break;
    case '-':
      token = new ASTNode{"-", ASTNodeType::MathOperator};
      parser.Next();
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

  std::cout << std::stoi(initial->name, 0, 12) << std::endl;
  
  return 0;
}
