cmake_minimum_required(VERSION 3.15)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compiler code/main.cpp code/Parser.cpp code/Parser.h code/basics.h code/Token.cpp code/Token.h code/Lexer.cpp code/Lexer.h code/Error.cpp code/Error.h code/Assert.cpp code/Assert.h code/Mips.cpp code/Mips.h code/SymbolMap.cpp code/SymbolMap.h code/babasics.h)

