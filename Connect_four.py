from random import randint
import copy

# <<< Exercise 1 >>>
example = [
    [' ', ' ', ' ', ' ', ' ', ' ', ' '], # 6
    [' ', ' ', ' ', ' ', ' ', ' ', ' '], # 5
    [' ', ' ', '*', 'o', ' ', ' ', ' '], # 4
    ['o', ' ', 'o', '*', ' ', ' ', ' '], # 3
    ['o', 'o', '', '', '', 'o', ''], # 2
    ['', '', 'o', '*', 'o', 'o', 'o']  # 1
]   # 1    2    3    4    5    6    7

# <<< Exercise 2 >>>
def create_grid():
    '''
        Creates an empty grid (or can be used to reset the existing one)
        return: list of 6 lists with length of 7
    '''
    return [[" " for _ in range(7)] for _ in range(6)]


# <<< Exercise 3 >>>
def display(grid):
    '''
        Displays the current state of grid
         : type grid: list
         : return: None (only prints)
    '''
    print('    1   2   3   4   5   6   7')
    for i in range(len(grid)):
        print(6 - i, '|', ' | '.join(grid[i]), '|', 6 - i)
    print('    1   2   3   4   5   6   7')


# <<< Exercise 4 >>>
def move_possible(col_index, grid):
    '''
        Determines if the move the player wants play is available
         : type grid: list
         : param column: the column player chose to drop the disc
         : type column: integer
         : return: row index (integer) or False (boolean) if no move is possible
    '''
    if 0 <= col_index <= 6:
        for row_index in range(5, -1, -1):  # go from bottom to top (row 5 to row 0)
            if grid[row_index][col_index] == ' ':
                return row_index  # return the row index where the disc can land
    return False  # return false if the column is full


# <<< Exercise 5 >>>
def drop_disc(grid, column, disc):
    '''
        Drops the disc into grid
         : type grid: list
         : param column: the number of the column player chose to drop the disc (not index)
         : type column: integer
         : param disc: the user's marker (either '*' or 'o') 
         : type disc: string
         : return: index of the row if move was possible or None otherwise
    '''
    col_index = column - 1  # adjust for 0-based index
    row_index = move_possible(col_index, grid)  # get the row index where the disc can drop

    if row_index is not False:
        grid[row_index][col_index] = disc  # place the disc in the correct row and column
        return row_index


# <<< Exercise 6 >>>
def length(box, grid):
    '''
        Indicates the length of the longest alignment of the disc in given position
         : param box: coordinates of the disc
         : type box: tuple; like (x, y) coordinates
         : type grid: list
         : return: natural number (that can be min 0 and max 7)
    '''
    x = 6 - box[0]  # since on display the indexes go from bottom to top, but in the matrix the other way around
    y = box[1] - 1  # indexes start from 0, but on display we start numbering from 1
    symbol = grid[x][y]
    if symbol == ' ':
        return 0
    
    lengths = {'row': 1, 'column': 1, 'diagonal1': 1, 'diagonal2': 1}

    # horizontal
    for direction in (-1, 1):  # -1 for left, +1 for right
        c = y  # we're gonna change this number, so we need a different variable for this loop
        while 0 <= c + direction < 7 and grid[x][c + direction] == symbol:
            lengths['row'] += 1
            c += direction

    # vertical
    for direction in (-1, 1):  # -1 for top, +1 for bottom
        r = x
        while 0 <= r + direction < 6 and grid[r + direction][y] == symbol:
            lengths['column'] += 1
            r += direction

    # diagonal1 (top-left to bottom-right)
    for direction in (-1, 1):  # -1 for top-left, +1 for bottom-right
        r, c = x, y
        while 0 <= r + direction < 6 and 0 <= c + direction < 7 and grid[r + direction][c + direction] == symbol:
            lengths["diagonal1"] += 1
            r += direction
            c += direction

    # diagonal2 (top-right to bottom-left)
    for direction in (-1, 1):  # -1 for top-left, +1 for bottom-right
        r, c = x, y
        while 0 <= r + direction < 6 and 0 <= c - direction < 7 and grid[r + direction][c - direction] == symbol:
            lengths["diagonal2"] += 1
            r += direction
            c -= direction
    
    return max(lengths.values())


# <<< Exercise 7 >>>
def recommend(grid):
    '''
        Recommends a random (but available) column to drop a disc
         : type grid: list
         : return: a string including the recommended column
    '''
    col = randint(1, 7)
    while move_possible(col, grid) == False:
        col = randint(1, 7) # first we recommended random column to the player, now this part is not useful
    return f"I recommend you to play at column {longest_alignment(grid, 'o')[0]}"


# <<< Exercise 8 >>>
def longest_alignment(grid, disc):
    '''
        Recommends the best possible move
         : type grid: list
         : param disc: the user's marker (either '*' or 'o') 
         : type disc: string
         : return: number of column which is the best move to play, and the length of alignment (tuple)
    '''
    best_move = 0
    grid_copy = copy.deepcopy(grid) # since grid is a list and list is areference type,
                                    # if we said grid_copy = grid, the changes we made on the copy would affect the initial one as well
                                    # so first we used .copy() function of list to create a variable with a different reference in the memory
                                    # but since the elements of the grid (the rows) are also lists, they had the same reference as the inital grid's rows
                                    # that's why we had to use deepcopy() function of copy module

    dict = {} # in this dictionary the keys are columns, and the values are the longest alignments we'll get if the disc drops to that column
    
    for col in range(1, 8):
        row_index = drop_disc(grid_copy, col, disc)    #creates copy of the grid to make move in that copy and see if it's the best
        
        if row_index != None:
            dict[col] = length((6 - row_index, col), grid_copy)
        else:
            dict[col] = 0

        grid_copy = copy.deepcopy(grid)
        
        if dict[col] == max(dict.values()):  # finds longest sequence and assigns it to best_move
            best_move = (col, dict[col])
    
    return best_move


# <<< Exercise 9 >>>
def human_turn(grid): 
    '''
        Handles the human player's move.
        Ensures that the input is valid and that the column isn't full before placing the disc.
         : type grid: list
         : return: True if the player wins, otherwise None
    '''
    row = 0
    while True:
        try:
            print('\nYour turn.', recommend(grid))
            column = int(input("Choose a column (1-7): "))
            row = 6 - drop_disc(grid, column, 'o')  #human drops a disc
            break
        except: # there could be 2 exceptions:
                    # 1. user input is not a number, and cannot be converted into int
                    # 2. drop_disc() returns None, (if number was >7 or <1)
            print("Column either full or invalid. Try again.")

    display(grid)
    if length((row, column), grid) >= 4:
        return True

def computer_turn(grid):  # finds the best move for the computer
    '''
        Handles the computer's move.
         : type grid: list
         : return: True if computer wins, otherwise None
    '''
    print("\nComputer's turn...")
    
    best_move = longest_alignment(grid, '*')
    player_best_move = longest_alignment(grid, 'o')
    
    if (not best_move[1] >= 4) and player_best_move[1] >= 4:
        best_move = player_best_move
    
    row = 6 - drop_disc(grid, best_move[0], '*')  # computer drops a disc

    display(grid)
    if length((row, best_move[0]), grid) >= 4:
        return True

def draw(grid):
    '''
        Checks if there are any moves left, else it's a draw
         : type grid: list
         : return: True or False (boolean)
    '''
    for row in grid:
        for col in row:
            if col == ' ':
                return False
    return True

def play_game(first_player, wins):  # checks if there is a winner
    '''
        Main game loop where the human and computer take turns.
        The game alternates between the human and the computer, starting with the human.
         : param first_player: indicating who starts ("human" or "computer")
         : type first_player: string
         : param wins: count of both computer's and human player's wins
         : type wins: dictionary
         : return: None
    '''
    grid = create_grid()  # create an empty grid
    display(grid)  # show the initial empty grid

    human = first_player == "human"  # True if human starts, False if computer starts

    while True:
        if human:  # Human's turn
            if human_turn(grid):
                print("Congratulations! You win!")
                wins['human'] += 1
                break
        else:  # Computer's turn
            if computer_turn(grid):
                print("Computer wins. Better luck next time!")
                wins['computer'] += 1
                break

        if draw(grid):
            break

        human = not human # Switch turns

wins = {'human': 0, 'computer': 0}
current_turn = "human"  # Initial starting player
while True:
    play_game(current_turn, wins)
    if input("\nDo you want to continue? If not input 0: ") == '0':
        break
    # Toggle the starting player
    current_turn = "computer" if current_turn == "human" else "human"

print('\nYou won', wins['human'], 'times, computer won', wins['computer'], 'times. See you next time!\n')
