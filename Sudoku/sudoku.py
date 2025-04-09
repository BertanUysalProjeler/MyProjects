import sys

# This function makes a list of lists that are containing numbers of rows
def create_grid(file):
    grid = [[],[],[],[],[],[],[],[],[]]              # Creates a list of empty lists
    with open(file, 'r') as text:                    # Opens file and checks every line(row) and puts them into lists
        line_num = 0
        for line in text:
            for num in line.split():
                grid[line_num].append(int(num))
            line_num += 1
    return grid

# This function finds the location of zero which means empty cell
def find_zero(grid):
    for row in range(9):                   # In this for iteration checks every cell if it is zero
        for column in range(9):
            if grid[row][column] == 0:
                return row, column
    return None                            # If there is no zero returns None

# This function looks if the given move is true or not
def is_true(grid, row, column, number):
    for c in range(9):                   # In this for iteration checks every column
        if grid[row][c] == number:
            return False

    for r in range(9):                   # In this for iteration checks every row
        if grid[r][column] == number:
            return False

    initial_row = 3 * (row // 3)
    initial_column = 3 * (column // 3)

    for r in range(3):                   # In this for iteration looks if the given number is in the 3*3 box
        for _ in range(3):
            if grid[initial_row + r][initial_column + _] == number:
                return False             # If is returns False

    return True                          # If the given number is not in the same row, same column, 3*3 box returns True

# This function looks how many possibilities there are for the given cell
def show_possibilities(grid, row, column):
    possibilities = {1, 2, 3, 4, 5, 6, 7, 8, 9}

    for c in range(9):                           # In this for iteration checks for row's every cell
        possibilities.discard(grid[row][c])

    for r in range(9):                           # In this for iteration checks for column's every cell
        possibilities.discard(grid[r][column])

    initial_row = 3 * (row // 3)
    initial_column = 3 * (column // 3)

    for r in range(3):                                                             # In this for iteration checks every 3*3 box
        for _ in range(3):
            possibilities.discard(grid[initial_row + r][initial_column + _])

    return list(possibilities)                                                     # Returns the list of possibilities for the given cell

# This function fills in cells with only one possibility until no further changes can be made
def fill(grid, steps, step_count):
    for row in range(9):
        for _ in range(9):
            if grid[row][_] == 0:              # Checks if the cell is empty
                possibilities_list = show_possibilities(grid, row, _)

                if len(possibilities_list) == 1:                      # If there is only one possiblity, fills in the cell and records the step
                    grid[row][_] = possibilities_list[0]
                    steps.append((copy_grid(grid), step_count, possibilities_list[0], row, _))
                    step_count += 1
                    return True, step_count                          # Updates step count here

    return False, step_count


def copy_grid(grid):
    # Manually creates a new copy of the grid
    return [row[:] for row in grid]

# This function recursively attempts to solve the Sudoku puzzle
def solving(grid, stages):
    stage_count = 1

    while True:                                      # Enters a while loop repeatedly calls the 'fill' function until no more changes can be made
        filled, stage_count = fill(grid, stages, stage_count)
        if not filled:                               # If the 'fill' function returns 'False', shows no changes were made
            break

    zero_location = find_zero(grid)                  # Looks for first empty cell

    if not zero_location:                            # If there are no empty cells, the sudoku is solved
        return True

    row, column = zero_location
    possibles = show_possibilities(grid, row, column)   # 'show_possibilities' function is called to get a list of possible values for the cell

    for number in possibles:                                                 # A loop iterates over each possible value for the epmty cell
        actual_value = grid[row][column]                                     # The current value of the cell is stored in 'actual_value'
        grid[row][column] = number                                           # The empty cell is filled with the current possible value
        stages.append((copy_grid(grid), stage_count, number, row, column))   # The current state of the grid is appended to the 'stages' list, along with needed info
        stage_count += 1                                                     # The 'solving' function is called recursively with the updated grid to continue the solving process

        if solving(grid, stages):                             # If the recursive call to 'solving' returns True, it means a solution is found
            return True                                       # and the function returns True to propagate the success upward through the recursive calls
                                                              # If the recursive call returns False it means no solution was found with the current value.The grid
        grid[row][column] = actual_value                      # is backtacked by restoring the original value to the empty cell

    return False                                              # If none of the possibilites lead to a solution, the function returns False to indicare failure

# Opens and prints everything wanted to the given output file
def print_to_output(steps, output):
    with open(output, 'w') as txt_file:
        for step in steps:
            txt_file.write('-' * 18 + '\n')
            txt_file.write(f"Step {step[1]} - {step[2]} @ R{step[3]+1}C{step[4]+1}\n")
            txt_file.write('-' * 18 + '\n')
            for row in step[0]:
                txt_file.write(' '.join(map(str, row)) + '\n')
        txt_file.write('-' * 18)

# Main function
def main():
    file = sys.argv[1]
    output = sys.argv[2]

    grid = create_grid(file)

    steps = []
    solving(grid, steps)

    print_to_output(steps, output)
# Triggers program
if __name__ == "__main__":
    main()
