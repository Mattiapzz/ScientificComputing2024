#include "include/sudokuUtils.hh"

bool 
parseSudokuFile(
  std::vector< std::vector<int> > & sudoku, 
  const std::string& filename) 
{
  bool isvalid = true;
  std::ifstream file(filename);
  std::string line;
  int row = 0;
  std::cout << "Reading Sudoku from " << filename << ":" << std::endl;
  try
  {
    std::cout << "Printing raw file content..." << std::endl;
    std::cout << "==========================" << std::endl;
    while (std::getline(file, line)) {
      std::cout << line << std::endl;
      if (line[0] == '+') continue; // ignore border lines
      std::istringstream iss(line);
      std::string block;
      int col = 0;
      while (std::getline(iss, block, '|')) {
        block.erase(remove(block.begin(), block.end(), ' '), block.end()); // remove spaces
        for (std::string::size_type i = 0; i < block.size(); ++i) {
          char c = block[i];
          if (c == '.') {
            sudoku[row][col++] = 0;
          } else if (isdigit(c)) {
            sudoku[row][col++] = c - '0';
          }
        }
      }
      row++;
    }
    std::cout << "==========================" << std::endl;
  }
  catch(const std::exception& e)
  {
    isvalid = false;
    fmt::print(fmt::fg(fmt::color::red) , "Something went wrong in the parsing. Sudoku may have an invalid format. Error: {}", e.what());
    std::cout << "\n";
  }
  return isvalid;
}

void printSudoku(const std::vector< std::vector<int> >& sudoku) {
  for (int i = 0; i < 9; ++i) {
    if (i % 3 == 0) {
        std::cout << "+-------+-------+-------+\n";
    }
    for (int j = 0; j < 9; ++j) {
      if (j % 3 == 0) {
        std::cout << "| ";
      }
      if (sudoku[i][j] == 0) {
        std::cout << ". ";
      } else {
        std::cout << sudoku[i][j] << " ";
      }
      //std::cout << (sudoku[i][j] == 0 ? "." : sudoku[i][j]) << " ";
    }
    std::cout << "|\n";
  }
  std::cout << "+-------+-------+-------+\n";
}


void prettyPrintSudoku(const std::vector< std::vector<int> >& sudoku){
  fmt::print(fmt::fg(fmt::color::blue) , "Pretty Printing Sudoku...\n");
  std::cout << "==========================" << std::endl;
  //
  fmt::print(fmt::bg(fmt::color::blue) , "{}{}{}{}{}{}{}",BUL,HL,BTU,HL,BTU,HL,BUR);
  std::cout << std::endl;
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      fmt::print(fmt::bg(fmt::color::blue) , "{}{}{}{}{}{}{}",BTL,HL,BC,HL,BC,HL,BTR);
      std::cout << std::endl;
    }
    for (int j = 0; j < 9; j++) {
      if (j % 3 == 0) {
        fmt::print(fmt::bg(fmt::color::blue) , "{}",BDV);
        // std::cout << "| ";
      }
      if (sudoku[i][j] == 0) {
        fmt::print(fmt::bg(fmt::color::red) , "{}{} ", (j%3==0?" ":""), WB);
        // std::cout << ". ";
      } else {
        fmt::print(fmt::bg(fmt::color::green) , "{}{} ",(j%3==0?" ":""), sudoku[i][j]);
        // std::cout << sudoku[i][j] << " ";
      }
    }
    fmt::print(fmt::bg(fmt::color::blue) , "{}",BDV);
    std::cout << std::endl;
    // std::cout << "|" << std::endl;
  }
  fmt::print(fmt::bg(fmt::color::blue) , "{}{}{}{}{}{}{}",BDL,HL,BTD,HL,BTD,HL,BDR);
  std::cout << std::endl;
  // std::cout << "+-------+-------+-------+" << std::endl;
  std::cout << "==========================" << std::endl;
}



bool
checkValidColumn(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col) {
  //std::cout << "Checking column " << idx_col << "..." << std::endl;
  bool isvalid = true;
  for (int i = 0; i < 9; i++) {
    if (i != idx_row) {
      if (sudoku[i][idx_col] == sudoku[idx_row][idx_col]) {
        isvalid = false;
        return isvalid;
      }
    }
  }
  return isvalid;
}

bool 
checkValidRow(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col) {
  //std::cout << "Checking row " << idx_row << "..." << std::endl;
  bool isvalid = true;
  for (int i = 0; i < 9; i++) {
    if (i != idx_col) {
      if (sudoku[idx_row][i] == sudoku[idx_row][idx_col]) {
        isvalid = false;
        return isvalid;
      }
    }
  }
  return isvalid;
}

bool
checkValicBlock(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col) {
  //std::cout << "Checking block " << idx_row << "..." << std::endl;
  bool isvalid = true;
  int start_row = (idx_row / 3) * 3;
  int start_col = (idx_col / 3) * 3;
  for (int i = start_row; i < start_row + 3; i++) {
    for (int j = start_col; j < start_col + 3; j++) {
      if (i != idx_row && j != idx_col) {
        if (sudoku[i][j] == sudoku[idx_row][idx_col]) {
          isvalid = false;
          return isvalid;
        }
      }
    }
  }
  return isvalid;
}

bool 
checkValidity(const std::vector< std::vector<int> >& sudoku, int idx_row, int idx_col) {
  return ( checkValicBlock(sudoku, idx_row, idx_col) && checkValidRow(sudoku, idx_row, idx_col) && checkValidColumn(sudoku, idx_row, idx_col) );
}

bool 
checkValidity(const std::vector< std::vector<int> >& sudoku) {
  //std::cout << "Checking Sudoku validity..." << std::endl;
  bool isvalid = true;

  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      isvalid = isvalid && checkValicBlock(sudoku, i, j) && checkValidRow(sudoku, i, j) && checkValidColumn(sudoku, i, j);
    }
  }
  return isvalid;
}


bool
solveSudoku(std::vector< std::vector<int> >& sudoku, int & ith) {
  //std::cout << "Solving Sudoku..." << std::endl;
  bool solved = true;
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      solved = false;
      if (sudoku[i][j] == 0) {
        for (int k = 1; k <= 9; k++) {
          ith++;
          sudoku[i][j] = k;
          if (checkValidity(sudoku, i, j)) {
            // std::cout << "Valid Iteration: " << ith << " Editing item (" << i << ", " << j << ") = " << k << std::endl;
            // // sleep for 1 second
            // printSudoku(sudoku);
            // std::this_thread::sleep_for(std::chrono::microseconds(100));
            solved = solveSudoku(sudoku, ith);
          } else {
            sudoku[i][j] = 0;
          }
          if (solved) {
            return true;
          } else {
            sudoku[i][j] = 0;
          }
          if (ith > 1e6) { return false; }
        }
        return false;
      }
    }
  }
  return true;
}

bool
solveSudoku2(std::vector< std::vector<int> >& sudoku, int & ith) {
  bool solved = false;
  // vector of cvarctor of vectors of possibility all from 1 to 9
  std::vector<std::vector<std::vector<int>>> possibilities(9, std::vector<std::vector<int>>(9, std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));
  int num_zero = 0;
  
  while (!solved && ith < 1e5) {
    // ith++;
    num_zero = 0;
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (sudoku[i][j] == 0) {
          num_zero++;
          if (possibilities[i][j].size() == 1) {
            sudoku[i][j] = possibilities[i][j][0];
          } else {
            for (unsigned k = 0; k < possibilities[i][j].size(); k++) {
              sudoku[i][j] = possibilities[i][j][k];
              ith++;
              if(!checkValidity(sudoku, i, j)){
                possibilities[i][j].erase(std::remove(possibilities[i][j].begin(), possibilities[i][j].end(), possibilities[i][j][k]), possibilities[i][j].end());
                k--;
              }
            }
            sudoku[i][j] = 0;
          }
        }
      }
    }
    // printSudoku(sudoku);
    if (num_zero > 0) {
      solved = false;
    } else {
     solved = true;
    }
  }
  return solved;
}

bool
solveSudoku3(std::vector< std::vector<int> >& sudoku, int & ith) {
  bool solved = false;
  solved = solveSudoku2(sudoku, ith);
  if( solved ) { return true; }
  solved = solveSudoku(sudoku, ith);
  return solved;
}

