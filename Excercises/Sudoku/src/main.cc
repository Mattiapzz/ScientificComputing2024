#include "sudokuUtils.hh"

int main()
{
  std::vector< std::vector<int> > sudoku(9, std::vector<int>(9, 0));
  parseSudokuFile(sudoku, "Problem.txt");
  printSudoku(sudoku);
  prettyPrintSudoku(sudoku);
  return 0;}