#include <iostream>
#include <fstream>
#include <sstream>

#include <fmt/core.h>
#include <fmt/color.h>

#define UL "\u300E"


#define BDH "\u2550"
#define BDV "\u2551"
#define BUL "\u2554"
#define BUR "\u2557"
#define BDL "\u255A"
#define BDR "\u255D"

#define BTL "\u2560"
#define BTR "\u2563"
#define BTU "\u2566"
#define BTD "\u2569"

#define BC "\u256C"

#define HL "\u2550\u2550\u2550\u2550\u2550\u2550\u2550"

#define WB "\u2610"


void 
parseSudokuFile(
  std::vector< std::vector<int> > & sudoku, 
  const std::string& filename);


void printSudoku(const std::vector< std::vector<int> >& sudoku);

void prettyPrintSudoku(const std::vector< std::vector<int> >& sudoku);

bool
checkValidColumn(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col);

bool 
checkValidRow(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col);

bool
checkValicBlock(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col);

bool 
checkValidity(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col);

bool 
checkValidity(const std::vector< std::vector<int> >& sudoku);

bool
solveSudoku(std::vector< std::vector<int> >& sudoku, int & ith);

bool
solveSudoku2(std::vector< std::vector<int> >& sudoku, int & ith);

bool
solveSudoku3(std::vector< std::vector<int> >& sudoku, int & ith);
