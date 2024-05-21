import os
import re

# Function to rename files
def rename_files():
    # List all files in the current directory
    files = os.listdir('.')
    
    # Regex pattern to match the files
    pattern = re.compile(r"^velox_query_timesq(\w+)\.txt$")
    
    for filename in files:
        # Check if the filename matches the pattern
        match = pattern.match(filename)
        if match:
            # Create the new filename by adding the underscore
            new_filename = f"velox_query_times_q{match.group(1)}.txt"
            # Rename the file
            os.rename(filename, new_filename)
            print(f"Renamed: {filename} -> {new_filename}")

# Run the rename function
if __name__ == "__main__":
    rename_files()

