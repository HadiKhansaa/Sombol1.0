#include <iostream>
#include <vector>

using namespace std;

pair<vector<vector<int>>, string> eat_piece_if_possible(vector<vector<int>>& board_layout, int row, int col, int r, int c, int color) {

  string eat_direction = "";

  if (color == 1 || color == 2) {
    int sumRowOver2 = (row + r) / 2;
    int sumColOver2 = (col + c) / 2;
    if (c == col) {
      if (r > row) {
        eat_direction = 'd';
      } else {
        eat_direction = 'u';
      }
    } else {
      if (c > col) {
        eat_direction = 'r';
      } else {
        eat_direction = 'l';
      }
    }

    if (color == 1) {
      if (board_layout[sumRowOver2][col] == 2 || board_layout[sumRowOver2][col] == 4) {
        board_layout[sumRowOver2][col] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      } else if (board_layout[row][sumColOver2] == 2 || board_layout[row][sumColOver2] == 4) {
        board_layout[row][sumColOver2] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      }
    } else if (color == 2) {
      if (board_layout[sumRowOver2][col] == 1 || board_layout[sumRowOver2][col] == 3) {
        board_layout[sumRowOver2][col] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      } else if (board_layout[row][sumColOver2] == 1 || board_layout[row][sumColOver2] == 3) {
        board_layout[row][sumColOver2] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      }
    }
  } else if (color == 3) {
    if (c == col) {
      if (r > row) {
        eat_direction = 'd';
      } else {
        eat_direction = 'u';
      }
    } else {
      if (c > col) {
        eat_direction = 'r';
      } else {
        eat_direction = 'l';
      }
    }

    for (int k = row; k <= r; k++) {
      if (board_layout[k][col] == 2 || board_layout[k][col] == 4) {
        board_layout[k][col] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      }
    }
  } else if (color == 4) {
    if (c == col) {
      if (r > row) {
        eat_direction = 'd';
      } else {
        eat_direction = 'u';
      }
    } else {
      if (c > col) {
        eat_direction = 'r';
      } else {
        eat_direction = 'l';
      }
    }

    for (int k = row; k <= r; k++) {
      if (board_layout[k][col] == 1 || board_layout[k][col] == 3) {
        board_layout[k][col] = 0;
        //eatSound.play();
        return {board_layout, eat_direction};
      }
    }
  }

  return {board_layout, eat_direction};
}

char get_eat_direction(int row1, int col1, int row2, int col2)
{
    char eat_direction = 0;
    if (col1==col2)
    {
        if (row1>row2)
            eat_direction = 'd';
        else
            eat_direction = 'u';
    }
    else
    {
        if (col1>col2)
            eat_direction = 'r';
        else
            eat_direction = 'l';
    }
    return eat_direction;
}

vector<vector<int>> deepcopy2(vector<vector<int>> board_layout) {
  vector<vector<int>> new_board_layout(board_layout.size());
  for (int i = 0; i < board_layout.size(); i++) {
    new_board_layout[i] = board_layout[i];
  }
  return new_board_layout;
}


int main(){
    vector<vector<int>> test_board = {
                                      {0, 0, 0, 0, 0, 0, 0},
                                      {1, 1, 1, 1, 1, 1, 1},
                                      {1, 1, 1, 1, 1, 1, 1},
                                      {0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0},
                                      {2, 2, 2, 2, 2, 2, 2},
                                      {2, 2, 2, 2, 2, 2, 2},
                                      {0, 0, 0, 0, 0, 0, 0},
                                    };


    double time_spent = 0.0;
    clock_t begin = clock();
    //time code
    for (int i = 0; i < 100000; i++)
    {
        deepcopy2(test_board);
    }
    
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<< time_spent;
    return 0;
}