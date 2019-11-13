cmake_minimum_required(VERSION 3.15)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)
add_executable(Compiler main.cpp frontend/Parser.cpp frontend/Parser.h frontend/basics.h frontend/Token.cpp frontend/Token.h frontend/Lexer.cpp frontend/Lexer.h frontend/Error.cpp frontend/Error.h frontend/Assert.cpp frontend/Assert.h backend/Mips.cpp backend/Mips.h backend/SymbolMap.cpp backend/SymbolMap.h)