import sys


def read_file(argument_file1):
    with open(argument_file1, 'r') as input_file:
        lines = [[char for char in line.split()] for line in input_file.readlines()]
        constraints = lines[0:4]
        puzzle_board = lines[4:]
        initial_puzzle_board = lines[4:]
    return constraints, puzzle_board, initial_puzzle_board


def create_grids(puzzle_board):
    grids = []
    searched = []
    for row in range(len(puzzle_board)):
        for col in range(len(puzzle_board[0])):
            cell = (row, col)
            cell_value = puzzle_board[row][col]
            if cell not in searched:
                if cell_value == "L":
                    grid = []
                    grid.append((row, col))
                    grid.append((row, col + 1))
                    grids.append(grid)
                    searched.append((row, col + 1))
                elif cell_value == "U":
                    grid = []
                    grid.append((row, col))
                    grid.append((row + 1, col))
                    grids.append(grid)
                    searched.append((row + 1, col))

    return grids


def find_empty_grid(puzzle_board, coordinate, grids):
    for grid in grids:
        row = grid[0][0]
        col = grid[0][1]
        if puzzle_board[row][col] == "L" or puzzle_board[row][col] == "U":
            coordinate[0] = row
            coordinate[1] = col
            return True

    return False


def is_valid(puzzle_board, x, y, tile):
    if tile[0] == "N":
        return True

    elif puzzle_board[x][y] == "L":
        if y-1 >= 0:
            if puzzle_board[x][y - 1] == tile[0]:
                return False
        if x-1 >= 0:
            if puzzle_board[x-1][y] == tile[0]:
                return False
        if x+1 <= len(puzzle_board)-1:
            if puzzle_board[x + 1][y] == tile[0]:
                return False
        if y+2 <= len(puzzle_board[0])-1:
            if puzzle_board[x][y+2] == tile[1]:
                return False
        if x-1 >= 0 and y+1 <= len(puzzle_board[0])-1:
            if puzzle_board[x-1][y+1] == tile[1]:
                return False
        if x+1 <= len(puzzle_board)-1 and y+1 <= len(puzzle_board[0])-1:
            if puzzle_board[x+1][y+1] == tile[1]:
                return False

    elif puzzle_board[x][y] == "U":
        if y-1 >= 0:
            if puzzle_board[x][y - 1] == tile[0]:
                return False
        if y+1 <= len(puzzle_board[0])-1:
            if puzzle_board[x][y + 1] == tile[0]:
                return False
        if x-1 >= 0:
            if puzzle_board[x - 1][y] == tile[0]:
                return False
        if x+2 <= len(puzzle_board)-1:
            if puzzle_board[x + 2][y] == tile[1]:
                return False
        if y-1 >= 0 and x+1 <= len(puzzle_board)-1:
            if puzzle_board[x+1][y - 1] == tile[1]:
                return False
        if x+1 <= len(puzzle_board)-1 and y+1 <= len(puzzle_board[0])-1:
            if puzzle_board[x + 1][y + 1] == tile[1]:
                return False

    return True


def constraint_looker(puzzle_board, constraints):

    for x in range(len(puzzle_board)):
        num_of_h, num_of_b = 0, 0
        for y in range(len(puzzle_board[0])):
            if puzzle_board[x][y] == "H":
                num_of_h += 1
            elif puzzle_board[x][y] == "B":
                num_of_b += 1

        if int(constraints[0][x]) > -1:
            condition1 = num_of_h == int(constraints[0][x])

            if (not condition1):
                return False

        if int(constraints[1][x]) > -1:
            condition2 = num_of_b == int(constraints[1][x])

            if (not condition2):
                return False

    for y in range(len(puzzle_board[0])):
        num_of_h, num_of_b = 0, 0
        for x in range(len(puzzle_board)):
            if puzzle_board[x][y] == "H":
                num_of_h += 1
            elif puzzle_board[x][y] == "B":
                num_of_b += 1

        if int(constraints[2][y]) > -1:
            condition3 = num_of_h == int(constraints[2][y])

            if (not condition3):
                return False

        if int(constraints[3][y]) > -1:
            condition4 = num_of_b == int(constraints[3][y])

            if (not condition4):
                return False

    return True


def solver(puzzle_board, constraints, grids):
    coordinate = [0, 0]

    if (not find_empty_grid(puzzle_board, coordinate, grids)):
        if constraint_looker(puzzle_board, constraints):
            return True

    row = coordinate[0]
    col = coordinate[1]

    tiles = [("H", "B"), ("B", "H"), ("N", "N")]

    for tile in tiles:
        if is_valid(puzzle_board, row, col, tile):
            if puzzle_board[row][col] == "L":
                if tile == ("H", "B"):
                    puzzle_board[row][col] = "H"
                    puzzle_board[row][col + 1] = "B"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "L"
                    puzzle_board[row][col + 1] = "R"

                elif tile == ("B", "H"):
                    puzzle_board[row][col] = "B"
                    puzzle_board[row][col + 1] = "H"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "L"
                    puzzle_board[row][col + 1] = "R"

                elif tile == ("N", "N"):
                    puzzle_board[row][col] = "N"
                    puzzle_board[row][col + 1] = "N"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "L"
                    puzzle_board[row][col + 1] = "R"

            if puzzle_board[row][col] == "U":
                if tile == ("H", "B"):
                    puzzle_board[row][col] = "H"
                    puzzle_board[row + 1][col] = "B"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "U"
                    puzzle_board[row + 1][col] = "D"

                elif tile == ("B", "H"):
                    puzzle_board[row][col] = "B"
                    puzzle_board[row + 1][col] = "H"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "U"
                    puzzle_board[row + 1][col] = "D"

                elif tile == ("N", "N"):
                    puzzle_board[row][col] = "N"
                    puzzle_board[row + 1][col] = "N"

                    if solver(puzzle_board, constraints, grids):
                        return True

                    puzzle_board[row][col] = "U"
                    puzzle_board[row + 1][col] = "D"


    return False


def write_board(puzzle_board, argument_file2, constraints, grids):
    if solver(puzzle_board, constraints, grids):
        with open(argument_file2, 'w') as output_file:
            for row in puzzle_board:
                for number in row:
                    output_file.write(str(number) + ' ')
                output_file.write('\n')

    else:
        with open(argument_file2, 'w') as output_file:
            output_file.write("No solution")


def main():
    argument_file1 = sys.argv[1]
    argument_file2 = sys.argv[2]
    constraints, puzzle_board, initial_puzzle_board = read_file(argument_file1)
    grids = create_grids(puzzle_board)
    write_board(puzzle_board, argument_file2, constraints, grids)


if __name__ == "__main__":
    main()