#include "sudoku.hh"

int main()
{
  int ith1 = 0;
  int ith2 = 0;
  int ith3 = 0;
  std::vector< std::vector<int> > sudoku1(9, std::vector<int>(9, 0));
  std::vector< std::vector<int> > sudoku2(9, std::vector<int>(9, 0));
  std::vector< std::vector<int> > sudoku3(9, std::vector<int>(9, 0));

  parseSudokuFile(sudoku1, "../problem.txt");
  solveSudoku3(sudoku1, ith1);
  std::cout << "Sudoku solved in " << ith1 << " iterations (recursive)." << std::endl;
  prettyPrintSudoku(sudoku1);

  parseSudokuFile(sudoku2, "../problem.txt");
  solveSudoku2(sudoku2, ith2);
  std::cout << "Sudoku solved in " << ith2 << " iterations (iterative)." << std::endl;
  prettyPrintSudoku(sudoku2);

  parseSudokuFile(sudoku3, "../problem.txt");
  prettyPrintSudoku(sudoku3);
  solveSudoku(sudoku3, ith3);
  std::cout << "Sudoku solved in " << ith3 << " iterations (mixed)." << std::endl;
  prettyPrintSudoku(sudoku3);

  return 0;
}

