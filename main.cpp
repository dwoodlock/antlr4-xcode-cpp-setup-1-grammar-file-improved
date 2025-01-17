/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <iostream>
#include <fstream>  

#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"

using namespace antlrcpptest;
using namespace antlr4;

int main(int , const char **) {
        
    std::string filename = "../../input.txt";
    
    // Open the file
    std::ifstream fileStream(filename);

    // Check if the file was opened successfully
    if (!fileStream) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;  // Exit with error code
    }
    
  ANTLRInputStream input(fileStream);
  ExprLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }

  ExprParser parser(&tokens);
  tree::ParseTree *tree = parser.prog();

  std::cout << tree->toStringTree(&parser) << std::endl;

  return 0;
}
