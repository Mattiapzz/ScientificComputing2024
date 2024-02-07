#include "include/sudokuUtils.hh"

#define ASSERT_SUDOKU(cond,msg) \
  if (!(cond)) { \
    fmt::print(fmt::fg(fmt::color::red), "Sudoku is invalid: {}\n", msg); \
    return 1; \
  } 

#define LOGO fmt::print(fmt::fg(fmt::color::yellow), \
  "Sudoku Solver\n" \
  );

#define LOGO2 fmt::print(fmt::fg(fmt::color::yellow), \
  " ___ _   _ ___   ___  _  ___   _ \n/ __| | | |   \\ / _ \\| |/ / | | |\n\\__ \\ |_| | |) | (_) | ' <| |_| |\n|___/\\___/|___/ \\___/|_|\\_ \\___/ \n\n"\
  );
                                                            