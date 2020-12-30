"""
Sergio Avila
Computer science 164 Artificial Intelligence
Project: Connect four

In this project I will be implementing a really simple
Connect four game. I won't be using any graphics because the 
main point of this program is to create an intelligent AI.
For this project I decided to use python3 in order to use some built-in functions.

Environment: I am using Linux operating system
             running this through the terminal

run: 
$> python3 connect4.py

"""

import random, sys, copy


#globals to set board parameters
Board_width = 7
Board_height = 6

def createBoard(board):
	print("\n")
	print(' ', end='')
	for i in range(1, Board_width + 1):
		print(' %s  ' % i, end='')
	print()
	print('+---+' + ('---+' * (Board_width - 1)))
	for j in range(Board_height):
		print('|', end='')
		for i in range(Board_width):
			print(' %s |' % board[i][j], end='')
		print()
		print('+---+' + ('---+' * (Board_width - 1)))

#getNewBoard
def getboard():
	board = []
	for i in range(Board_width):
		board.append([' '] * Board_height)
	return board

#enterHumanTile
def chooseColor():
	#colors is going to refer to x = red and o = black
	color = ''
	while not(color == 'X' or color == 'O'):
		print("Color Red = X & Color Black = O")
		print("Choose a color")
		color = input().upper()

	if color == 'X':
		return['X', 'O']
	else:
		return['O', 'X']
#getHumanMove
def playerMove(board):
	while True:
		print("Make a move. select 1-7")
		move = input()
		if not move.isdigit():
			continue
		move = int(move) - 1
		if isValidMove(board, move):
			return move

def compMove(board, computerColor):
    potentialMoves = getmoves(board, computerColor, 2)
    bestMoveScore = max([potentialMoves[i] for i in range(Board_width) if isValidMove(board, i)])
    bestMoves = []
    for i in range(len(potentialMoves)):
        if potentialMoves[i] == bestMoveScore:
            bestMoves.append(i)
    return random.choice(bestMoves)

# getPotentialMoves
def getmoves(board, human, lookahead):
	if lookahead == 0:
		return [0] * Board_width
	potentialMoves = []

	if human == 'X':
		enemy = 'O'
	else:
		enemy = 'X'

	if full(board):
		return [0] * Board_width

	potentialMoves = [0] * Board_width
	for playerMove in range(Board_width):
		dupeBoard = copy.deepcopy(board)
		if not isValidMove(dupeBoard, playerMove):
			continue
		makeMove(dupeBoard, human, playerMove)
		if isWinner(dupeBoard, human):
			potentialMoves[playerMove] = 1
			break
		else:
			if full(dupeBoard):
				potentialMoves[playerMove] = 0
			else:
				for enemyMove in range(Board_width):
					dupeBoard2 = copy.deepcopy(dupeBoard)
					if not isValidMove(dupeBoard2, enemyMove):
						continue
					makeMove(dupeBoard2, enemy, enemyMove)
					if isWinner(dupeBoard2, enemy):
						potentialMoves[playerMove] = -1
						break
					else:
						results = getmoves(dupeBoard2, human, lookahead - 1)
						potentialMoves[playerMove] += (sum(results) / Board_width) / Board_width
	return potentialMoves

#isBoardFull
def full(board):
    for i in range(Board_width):
        for j in range(Board_height):
            if board[i][j] == ' ':
                return False
    return True

def whoGoesFirst():
    if random.randint(0, 1) == 0:
        return 'computer'
    else:
        return 'human'


def makeMove(board, player, column):
    for i in range(Board_height-1, -1, -1):
        if board[column][i] == ' ':
            board[column][i] = player
            return


def isValidMove(board, move):
    if move < 0 or move >= (Board_width):
        return False

    if board[move][0] != ' ':
        return False

    return True

def isWinner(board, tile):
    for y in range(Board_height):
        for x in range(Board_width - 3):
            if board[x][y] == tile and board[x+1][y] == tile and board[x+2][y] == tile and board[x+3][y] == tile:
                return True

    for x in range(Board_width):
        for y in range(Board_height - 3):
            if board[x][y] == tile and board[x][y+1] == tile and board[x][y+2] == tile and board[x][y+3] == tile:
                return True

    for x in range(Board_width - 3):
        for y in range(3, Board_height):
            if board[x][y] == tile and board[x+1][y-1] == tile and board[x+2][y-2] == tile and board[x+3][y-3] == tile:
                return True

    for x in range(Board_width - 3):
        for y in range(Board_height - 3):
            if board[x][y] == tile and board[x+1][y+1] == tile and board[x+2][y+2] == tile and board[x+3][y+3] == tile:
                return True

    return False


def main():

    while True:
        humanTile, computerTile = chooseColor()
        turn = whoGoesFirst()
        print('The %s player will go first.' % (turn))
        mainBoard = getboard()

        while True:
            if turn == 'human':
                createBoard(mainBoard)
                move = playerMove(mainBoard)
                makeMove(mainBoard, humanTile, move)
                if isWinner(mainBoard, humanTile):
                    winner = 'human'
                    break
                turn = 'computer'
            else:
                createBoard(mainBoard)
                print("Computer's turn ")
                move = compMove(mainBoard, computerTile)
                makeMove(mainBoard, computerTile, move)
                if isWinner(mainBoard, computerTile):
                    winner = 'computer'
                    break
                turn = 'human'

            if full(mainBoard):
                winner = 'tie'
                break

        createBoard(mainBoard)
        print('Winner is: %s' % winner)


if __name__ == '__main__':
    main()