#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int x = 6, y = 7;

int is_move_possible(char grid[6][7], int move){
    /*
    This function checks if the move is possible through that column
    Input:
        grid[6][7]: a matrix consisting of characters '*' or 'o'
        move: integer, a number between 1 and 7
    Output:
        is_possible: either 0 - move is not possible or 1 -move is possible
    */
    //invalid move that exceeds the row count
    if ((move < 0) || (move > 6)){
		return 0;
    }

    //if 6th row of the grid is empty
    if (grid[0][move] != ' '){
		return 0;
    }

    return 1;
}

int drop_disk(char grid[6][7], int move, char character){
    /*
    This function drops a disk into the aviable place in the grid if possible, if it's not possible it does nothing
    Input:
        grid: matrix consiting of characters
        move: integer, the column it will fall
        character: character, that basically shows whose turn it is
    Output:
        void: it drops a disk into the empty place
    */
    for(int i = 5; i >= 0; i--){
        if(grid[i][move] == ' '){
            grid[i][move] = character;
            return i;
        }
    }
}

int alignment(char list[6][7], char symbol){
	/*
	This function checks grid horizantally, vertically and diognally and returns longest same color sequence
	Input:
		list[6][7]: a 6x7 matrix with characters
		symbol: character
	Output:
		integer: longest aligned length of given color
	*/
	int i, j, align, longest_alignment = 0;

	// horizontal
	for(i = 0; i < 6; i++){
		align = 1;
		for(j = 0; j < 7 - 1; j++){
			if(list[i][j] == list[i][j + 1]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[i][j] == symbol)){
				longest_alignment = align;
			}
		}
	}
	// vertical
	for(j = 0; j < 7; j++){
		align = 1;
		for(i = 1; i < 6 - 1; i++){
			if(list[i][j] == list[i + 1][j]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[i][j] == symbol)){
				longest_alignment = align;
			}
		}
	}
	// diagonal left-to-right bottom 
	for(j = 0; j < 7 - 1; j++){
		align = 1;
		for(i = j; i < 6 - 1; i++){
			if(list[i][i - j] == list[i + 1][i - j + 1]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[i][i - j] == symbol)){
				longest_alignment = align;
			}
		}
	}

	// diagonal left-to-right top
	for(j = 6; j > 0; j--){
		align = 1;
		for(i = 6 - j; i < 6 - 1; i++){
			if(list[i][12 - i - j] == list[i + 1][12 - i - j - 1]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[i][12 - i - j] == symbol)){
				longest_alignment = align;
			}
		}
	}
	// diagonal right-to-left bottom 
	for(i = 1; i < 6; i++){
		align = 1;
		for(j = i; j < 7 - 1; j++){
			if((j - i) > 4) continue;
			if(list[j - i][j] == list[j - i + 1][j + 1]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[j - i][j] == symbol)){
				longest_alignment = align;
			}
		}
	}
	// diagonal right-to-left top
	for(i = 1; i < 6; i++){
		align = 1;
		for(j = 6 - i; j > 0; j--){
			if((6 - i - j) > 4) continue;
			if(list[6 - i - j][j] == list[6 - i - j + 1][j - 1]){
				align++;
			}
			else {
				align = 1;
			}
			if((align > longest_alignment) && (list[6 - i - j][j] == symbol)){
				longest_alignment = align;
			}
		}
	}
	return longest_alignment;
} // done

int random_advice(char grid[6][7]){
	/*
	This function gives user a random advice based on the move_possible functin, if the move is possible on that random given place it returns that column, if it's not it gives another random advice
	Input:
		grid[6][7]: a 6x7 matrix with characters
	Output:
		integer: randomly assigned advice
	*/
	int random_number = rand() % 7 + 1;
	int index = 0;
	while(1){
		if(is_move_possible(grid, index % 7)){
			random_number--;
			if(random_number <= 0){
				return index % 7;
			}
		}
		index++;
	}
}

int advanced_advice(char grid[6][7], char player){
	/*
	This function gives advice to the users or to the bot, based on alignment function. We check each possible column and using alignment function we assign the column with longest possible alignment to a variable and return that column
	Input:
		grid[6][7]: a 6x7 matrix
		player: character
	Output:
		advancely recommended column returns best place to drop the disk
	*/
	int longestForPlayer[7], longestForOpponent[7];
	char opponent;
	int row;

	//we make sure of player an opponent's disk color
	if(player == '*')
		opponent = 'o';
	else 
		opponent = '*';

	//this loop determines move possibilities
	for(int i = 0; i < 7; i++){
		if(is_move_possible(grid, i)){
			row = drop_disk(grid, i, player);
			longestForPlayer[i] = alignment(grid, player);
			grid[row][i] = opponent;
			longestForOpponent[i] = alignment(grid, opponent);
			grid[row][i] = ' ';
		}
		else {
			longestForPlayer[i] = 0;
			longestForOpponent[i] = 0;
		}
	}

	//this loop determines max alignment for both players
	int maxPlayer = 0, maxOpponent = 0;
	for(int i = 0; i < 7; i++){
		if(longestForPlayer[i] > maxPlayer){
			maxPlayer = longestForPlayer[i];
		}
		if(longestForOpponent[i] > maxOpponent){
			maxOpponent = longestForOpponent[i];
		}
	}

	//we generate a random number to make different move choices
	int random_number = rand() % 7 + 1;
	int index = 0, reccomended;

	//finding best aligned column
	while(1){
		if(longestForPlayer[index % 7] == maxPlayer){
			random_number--;
			if(random_number <= 0){
				reccomended = index % 7;
				break;
			}
		}
		index++;
	}

	//if there's winning move we recommend that one
	if(maxPlayer >= 4){
		return reccomended;
	}
	//if player can't win they either block opponent's winning move or they play best choice 
	else if(maxOpponent >= 4){
		for(int i = 0; i < 7; i++){
			if(longestForOpponent[i] == maxOpponent){
				return i;
			}
		}
	}
	else
		return reccomended;
}

void createGrid(char grid[6][7]) {
	/*
	This function creates grid which is basically playing board of this game
	Input: 
		grid[6][7]: a 6x7 matrix consisting of characters
	Output: 
		void: it just creates a matrix it doesn't return anything
	*/

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			grid[i][j] = ' ';
		}
	}
}

void showGrid(char grid[6][7]) {
	/*
	This function shows the grid after each game, after each move
	Input: 
		grid[6][7]: a 6x7 matrix filled with characters
	Output: 
		void: it prints the grid after eachg move it doesn't return anything
	*/
	for (int i = -1; i < x + 1; i++)
	{
		for (int j = -1; j < y + 1; j++)
		{
			if (j == -1 || j == y) {
				if (i > -1 && i < x)
					printf("%d ", x - i);
				else
					printf("  ");
			}
			else if (i == -1 || i == x) {
				if (j > -1 && j < y)
					printf("%d ", j + 1);
				else
					printf("  ");
			}
			else if (i >= 0 && i < x && j >= 0 && j < y) {
				printf("%c ", grid[i][j]);
			}
		}
		printf("\n");
	}
}


// Function to check if it is draw
int is_grid_full(char grid[6][7]) {
    for (int col = 0; col < 7; col++) {
        if (is_move_possible(grid, col)) {
            return 0; // Grid is not full
        }
    }
    return 1; // Grid is full
}

char game(char player){
	/*
	This function is crated for player vs player interactions. each player gets a turn and they play until one of them creates 4 disks alligned
	Input: 
		player: character
	Output: 
		charcter: the symbol(disk color) of next player
	*/
	srand(time(NULL));
	char initialSymbol = player;
	char grid[6][7];
	createGrid(grid);

	char run = 1;
	int move;
	while(run && !is_grid_full(grid)){
		
		//H cursors comes to the beginning, 2J terminal gets erased
		printf("\x1b[H\x1b[2J");
		showGrid(grid);
		
		//user gets advice from the computer
		if(is_grid_full(grid) != 1){
			printf("You can play on %d\n", advanced_advice(grid, player) + 1);
			printf("\nPlayer %c's move: ", player);
			scanf("%d", &move);

			//if user's move is possible we drop the disk and count length of alignment
			if(is_move_possible(grid, move - 1)){
				drop_disk(grid, move - 1, player);
				
				//if user wins it gives a message
				if(alignment(grid, player) >= 4){
					printf("\x1b[H\x1b[2J");
					showGrid(grid);
					printf(" -- Player %c won the game --\n", player);
					run = 0;
				}

				//next player's turn
				if(player == '*')
					player = 'o';
				else
					player = '*';
			}
		}else{
			break;
		}
	}
	if(is_grid_full(grid) == 1){
		printf("It's a draw!\n");
	}

	if(initialSymbol == '*')
		return 'o';
	return '*';
}

void game_vs_ai(char player){
	/*
	This function is made for player vs ai interactions. Each time it gives user a game and considers a next move based on the users move
	Input: 
		player: character
	Output: 
		void: it doesn't return a value
	*/
	srand(time(NULL));
	char ai;
	char grid[6][7];
	createGrid(grid);
	if(player == '*'){
		ai = 'o';
	}else{
		ai = '*';
	}
	char run = 1;
	int move;
	while(run && !is_grid_full(grid)){
		printf("\x1b[H\x1b[2J");
		showGrid(grid);

		if(is_grid_full(grid) != 1){
			printf("\nYour turn: ", player);
			scanf("%d", &move);

			if(is_move_possible(grid, move - 1)){
				drop_disk(grid, move - 1, player);

				if(alignment(grid, player) >= 4){
					printf("\x1b[H\x1b[2J");
					showGrid(grid);
					printf(" -- You won the game --\n");
					run = 0;
					break;
				}
			}

			drop_disk(grid, advanced_advice(grid, ai), ai);
			if(alignment(grid, ai) >= 4){
				printf("\x1b[H\x1b[2J");
				showGrid(grid);
				printf(" -- Ai won the game --\n");
				run = 0;
			}
		}
	}
	if(is_grid_full(grid) == 1){
		printf("It's a draw!");
	}
}

int main() {
	char run = 1;
	int choice = 3;
	char player_char = '*';

	//this loop opens the menu of the game and gives user options
	while(run){
		printf("+------ Connect 4 ------+\n"
			   "|      (1) Start        |\n"
			   "|      (2) About        |\n"
			   "|      (0) Exit         |\n"
			   "+-----------------------+\n"
			   "Choose option: ");
		scanf("%d", &choice);
		switch(choice){
			case 1:
				printf(
			   "+----------------------------+\n"
			   "|   (1) Player vs Player     |\n"
			   "|   (2) Player vs Computer   |\n"
			   "|   (0) Back                 |\n"
			   "+----------------------------+\n"
			   "Choose option: ");
				scanf("%d", &choice);
				switch(choice){
					case 1:
						player_char = game(player_char);
						break;
					case 2:
						game_vs_ai(player_char);
						break;
					case 0:
						break;
					default:
						printf("\n =>> Error. Undefined option <<==\n");
				}
				break;
			case 2:
				printf("\nThis program is created by\n"
				"Ayhan Safarov, Tural Haji-Nabili, Ilaha Algayeva and Zaur Valiyev\n\n"
				"While making of this project, even though caused problems, GitHub was used\n"
				"Program was made to work in both Windows and Linux terminals\n"
				"Please support us on our patreon...\n\n");
				break;
			case 0:
				run = 0;
				break;
			default:
				printf("\n =>> Error. Undefined option <<==\n");
		}
	}

	return 0;
}
