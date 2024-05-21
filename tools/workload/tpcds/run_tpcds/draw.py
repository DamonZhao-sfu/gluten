import os
import re
import matplotlib.pyplot as plt

def extract_execution_time(file_path):
    # Read the third line of the file
    with open(file_path, 'r') as file:
        lines = file.readlines()
        if len(lines) >= 3:
            # Extract execution time from the third line (assuming it's the correct format)
            try:
                execution_time = float(lines[2].split(":")[-1].strip().split()[0])
                return execution_time
            except ValueError:
                print(f"Error extracting execution time from {file_path}")
    return None

def extract_numeric_part(query_number):
    # Extract numeric part from the query number using regular expressions
    numeric_part = re.search(r'\d+', query_number).group()
    return int(numeric_part) if numeric_part else 0

def main():
    # Directory containing the files
    directory = "./"

    # List to store query numbers and execution times
    query_data = []

    # Iterate over files in the directory
    for filename in os.listdir(directory):
        if filename.startswith("velox_query_times") and filename.endswith(".txt"):
            file_path = os.path.join(directory, filename)
            query_number = filename.split("_")[3].split(".")[0]  # Extract query number from filename
            execution_time = extract_execution_time(file_path)
            if execution_time is not None:
                query_data.append((query_number, execution_time))

    # Sort query data by the numeric part of query number
    query_data.sort(key=lambda x: extract_numeric_part(x[0]))

    # Extract sorted query numbers and execution times
    sorted_query_numbers = [data[0] for data in query_data]
    sorted_execution_times = [data[1] for data in query_data]

    # Plot the bar diagram
    plt.figure(figsize=(40, 30))  # Increase figure size
    bars = plt.bar(sorted_query_numbers, sorted_execution_times, color='skyblue')
    plt.xlabel('Query Number')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Execution Time for Query Numbers')
    plt.xticks(rotation=45, ha='right')  # Rotate x-axis labels for better readability

    # Annotate each bar with its y-coordinate value
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width() / 2.0, height, round(height, 2), ha='center', va='bottom')

    plt.tight_layout()

    # Save the plot locally
    plt.savefig("velox_result.png")

    # Show the plot
    plt.show()

if __name__ == "__main__":
    main()
