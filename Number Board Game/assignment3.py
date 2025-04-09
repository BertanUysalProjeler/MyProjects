import sys
import copy

def create_grid(input_file):
    with open(input_file, 'r') as text:
        grid = [[int(num) for num in line.split()] for line in text]
        return grid

def print_initial_board(grid):
    for lists in grid:
        for i in lists:
            print(i, end='\t')
        print()

    print('\n' + 'Your score is: 0')

def game_sequence(grid, score):
    guess_value, row_guess, column_guess, v = take_guess(grid, score)

    v, grid, guess_value = dfs(grid, row_guess, column_guess, v, guess_value)

    score = evaluate_score(v, guess_value, score)

    grid = erase_numbers(v, grid)

    org_grid = org_board(grid)

    grid = org_board_second(org_grid)

    print_new_board(grid, score)

    is_possible = evaluate_possibility(grid)

    if is_possible != 0:
        game_sequence(grid, score)
    else:
        print("Game over")

def dfs(grid, i, j, v, guess_value):
    guess_value_copy = guess_value
    grid_copy = grid

    dirs = [[0, -1], [-1, 0], [0, 1], [1, 0]]

    rows = len(grid_copy)
    cols = len(grid_copy[0])

    if i < 0 or i >= rows or j < 0 or j >= cols or grid_copy[i][j] != guess_value_copy or grid_copy[i][j] == '':
        return v, grid, guess_value

    grid_copy[i][j] = ''

    v.append((i, j))

    for dir in dirs:
        v, grid_copy, guess_value_copy = dfs(grid_copy, i + dir[0], j + dir[1], v, guess_value_copy)

    if len(v) == 1:
        # Restore the values back to the marked cell
        i, j = v[0]
        grid_copy[i][j] = guess_value

    return v, grid_copy, guess_value_copy

def evaluate_score(v, guess_value, score):
    if len(v) > 1:
        gained_score = len(v) * int(guess_value)
        score += gained_score
        return score
    else:
        return score

def take_guess(grid, score):
    try:
        guess = input("Your guess: ")
        guess_list = guess.split()
        row_guess, column_guess = int(guess_list[0]) - 1, int(guess_list[1]) - 1
        guess_value = int(grid[row_guess][column_guess])
        if row_guess < 0 or column_guess < 0:
            raise IndexError
        v = []
        return guess_value, row_guess, column_guess, v

    except ValueError:
        print("Please enter a correct size!")
        print_new_board(grid, score)
        game_sequence(grid, score)

    except IndexError:
        print("Please enter a correct size!")
        print_new_board(grid, score)
        game_sequence(grid, score)

def erase_numbers(v, grid):
    if len(v) > 1:
        for coordinate in v:
            row, column = coordinate
            grid[row][column] = ''
        return grid
    else:
        print("No movement happened; try again!")
        return grid

def org_board(grid):
    non_empty_rows = [row for row in grid if any(cell != '' for cell in row)]
    non_empty_columns = [col for col in zip(*non_empty_rows) if any(cell != '' for cell in col)]

    transposed = list(zip(*non_empty_columns))
    result_grid = [list(row) for row in transposed]

    return result_grid

def org_board_second(org_grid):
    moved = True

    while moved:
        moved = False

        for i in range(len(org_grid) - 1, 0, -1):
            for j in range(len(org_grid[0])):
                if org_grid[i][j] == '' and org_grid[i - 1][j] != '':
                    org_grid[i][j], org_grid[i - 1][j] = org_grid[i - 1][j], org_grid[i][j]
                    moved = True

    # Remove empty rows at the top of the grid
    org_grid = [row for row in org_grid if any(cell != '' for cell in row)]

    return org_grid

def print_new_board(grid, score):
    for row in grid:
        for cell in row:
            if cell == '':
                print(' ', end='\t')
            else:
                print(cell, end='\t')
        print()

    print('\n' + f"Your score is: {score}")

def evaluate_possibility(grid):
    possibility_count = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] != '':
                v = []
                guess_value = grid[i][j]
                grid_copy = copy.deepcopy(grid)
                dfs(grid_copy, i, j, v, guess_value)
                if len(v) >= 2:
                    possibility_count += 1

    return possibility_count

def main():
    input_file = sys.argv[1]

    grid = create_grid(input_file)

    print_initial_board(grid)

    score = 0
    try:
        game_sequence(grid, score)
    except TypeError:
        exit()
if __name__ == "__main__":
    main()
