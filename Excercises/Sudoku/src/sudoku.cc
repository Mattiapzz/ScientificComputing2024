#include "sudoku.hh"

int main()
{
  LOGO;
  LOGO2;
  bool isValid = false;
  int ith1 = 0;
  int ith2 = 0;
  int ith3 = 0;
  std::vector< std::vector<int> > sudoku1(9, std::vector<int>(9, 0));
  std::vector< std::vector<int> > sudoku2(9, std::vector<int>(9, 0));
  std::vector< std::vector<int> > sudoku3(9, std::vector<int>(9, 0));

  // Strategy 1. Brute force recursive

  isValid = parseSudokuFile(sudoku1, "../hard_problem.txt");
  ASSERT_SUDOKU(isValid, "Invalid sudoku file, fail to parse.");
  solveSudoku3(sudoku1, ith1);
  std::cout << "Sudoku solved in " << ith1 << " iterations (recursive)." << std::endl;
  prettyPrintSudoku(sudoku1);

  // Strategy 2. Smart iterative (Only work for simple to medium sudoku)

  isValid = parseSudokuFile(sudoku2, "../hard_problem.txt");
  ASSERT_SUDOKU(isValid, "Invalid sudoku file, fail to parse.");
  solveSudoku2(sudoku2, ith2);
  std::cout << "Sudoku solved in " << ith2 << " iterations (iterative)." << std::endl;
  prettyPrintSudoku(sudoku2);

  // Strategy 3. Mixed (first step iterative to fill in the easy ones, then recursive to solve the rest)

  isValid = parseSudokuFile(sudoku3, "../hard_problem.txt");
  ASSERT_SUDOKU(isValid, "Invalid sudoku file, fail to parse.");
  std::cout << "Pretty print unsolved sudoku:" << std::endl;
  prettyPrintSudoku(sudoku3);
  solveSudoku(sudoku3, ith3);
  std::cout << "Sudoku solved in " << ith3 << " iterations (mixed)." << std::endl;
  prettyPrintSudoku(sudoku3);

  return 0;
}

