import sys
import os

def write_to_file(file_number, lines):
    filename = f"{file_number:08d}.txt"
    with open(filename, 'w') as file:
        file.writelines(lines)

def main():
    file_size_limit = 1 * 1024 * 1024  # 1MB in bytes
    lines = []
    current_timestamp = None
    current_file_number = 1

    for line in sys.stdin:
        timestamp = line.split(' ')[0]

        if current_timestamp is None:
            current_timestamp = timestamp

        if current_timestamp != timestamp and sys.getsizeof(lines) > file_size_limit:
            write_to_file(current_file_number, lines)
            lines = []
            current_file_number += 1

        if current_timestamp != timestamp:
            current_timestamp = timestamp

        lines.append(line)

    # Write any remaining lines to the last file
    if lines:
        write_to_file(current_file_number, lines)

if __name__ == "__main__":
    main()
