import sys

# Function to write lines to a file with a given file number
def write_to_file(file_number, lines):
    filename = f"{file_number:08d}.txt"
    with open(filename, 'w') as file:
        file.writelines(lines)

# Main function
def main():
    # Set the maximum file size limit in bytes (100 KB in this example)
    file_size_limit = 100 * 1000
    lines = []                  # Initialize an empty list to store lines
    current_timestamp = None    # Initialize a variable to track the current timestamp
    current_file_number = 0     # Initialize the current file number

    # Read lines from standard input (stdin) using a loop
    for line in sys.stdin:
        timestamp = line.split(' ')[0]  # Extract the timestamp from the line

        if current_timestamp is None:
            current_timestamp = timestamp  # Set the current timestamp if it's not set

        # Check if the timestamp changes and the current lines exceed the file size limit
        if current_timestamp != timestamp and len(lines) > file_size_limit:
            write_to_file(current_file_number, lines)  # Write lines to a file
            lines = []                # Clear the lines list for the next file
            current_file_number += 1  # Increment the file number

        if current_timestamp != timestamp:
            current_timestamp = timestamp  # Update the current timestamp

        lines.append(line)  # Add the current line to the lines list

    # Write any remaining lines to the last file
    if lines:
        write_to_file(current_file_number, lines)

# Entry point of the script
if __name__ == "__main__":
    main()  # Call the main function if the script is executed
