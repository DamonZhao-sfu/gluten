import os
import re
import matplotlib.pyplot as plt

def extract_execution_time(file_path):
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
            if len(lines) >= 3:
                # Extract execution time from the third line
                try:
                    execution_time = float(lines[2].split(":")[-1].strip().split()[0])
                    return execution_time
                except ValueError:
                    print(f"Error extracting execution time from {file_path}")
    except Exception as e:
        print(f"Error reading file {file_path}: {e}")
    return None

def extract_numeric_part(query_number):
    match = re.search(r'\d+', query_number)
    return int(match.group()) if match else 0

def main():
    directory = "./"
    query_data = []

    total_velox_time = 0
    total_query_time = 0
    speedup_results = []
    num_speedup = 0
    num_slowdown = 0
    long_queries = 0
    long_queries_speedup = 0
    long_queries_slowdown = 0

    for filename in os.listdir(directory):
        if filename.startswith("velox_query_times_") and filename.endswith(".txt"):
            query_number = filename.split("_")[3].split(".")[0]
            corresponding_file = f"query_times_{query_number}.txt"
            velox_file_path = os.path.join(directory, filename)
            query_file_path = os.path.join(directory, corresponding_file)
            
            velox_execution_time = extract_execution_time(velox_file_path)
            query_execution_time = extract_execution_time(query_file_path)
            
            if velox_execution_time is not None and query_execution_time is not None:
                query_data.append((query_number, velox_execution_time, query_execution_time))
                total_velox_time += velox_execution_time
                total_query_time += query_execution_time

    query_data.sort(key=lambda x: extract_numeric_part(x[0]))

    sorted_query_numbers = [data[0] for data in query_data]
    velox_execution_times = [data[1] for data in query_data]
    query_execution_times = [data[2] for data in query_data]

    fig, ax = plt.subplots(figsize=(70, 50))  # Increase figure size for readability
    bar_width = 0.35  # Width of the bars

    index = range(len(sorted_query_numbers))

    # Plot bars for velox execution times
    bars1 = ax.bar(index, velox_execution_times, bar_width, label='Velox Execution Time', color='blue')

    # Plot bars for query execution times
    bars2 = ax.bar([i + bar_width for i in index], query_execution_times, bar_width, label='Query Execution Time', color='orange')

    ax.set_xlabel('Query Number')
    ax.set_ylabel('Execution Time (seconds)')
    ax.set_title('Execution Time Comparison for Queries')
    ax.set_xticks([i + bar_width / 2 for i in index])
    ax.set_xticklabels(sorted_query_numbers, rotation=45, ha='right')
    ax.legend()

    # Annotate each bar with its y-coordinate value
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width() / 2.0, height, round(height, 2), ha='center', va='bottom')

    for i in range(len(sorted_query_numbers)):
        speedup = query_execution_times[i] / velox_execution_times[i]  if velox_execution_times[i] > 0 else 0
        speedup_results.append((sorted_query_numbers[i], speedup, velox_execution_times[i], query_execution_times[i]))

        if speedup > 1:
            num_speedup += 1
        elif speedup < 1:
            num_slowdown += 1

        if query_execution_times[i] > 60:
            long_queries += 1
            if speedup > 1:
                long_queries_speedup += 1
            elif speedup < 1:
                long_queries_slowdown += 1


    plt.tight_layout()

    # Add legend for total execution times, number of speedup, and number of slowdown
    legend_text = (f'Total Velox Time: {total_velox_time:.2f}s\n'
                   f'Total Non-Native Spark Time: {total_query_time:.2f}s\n'
                   f'Number of Queries with Speedup: {num_speedup}\n'
                   f'Number of Queries with Slowdown: {num_slowdown}\n'
                   f'Number of Long Queries (>1min): {long_queries}\n'
                   f'Number of Long Queries with Speedup: {long_queries_speedup}\n'
                   f'Number of Long Queries with Slowdown: {long_queries_slowdown}')

    plt.figtext(0.5, 0.95, legend_text, wrap=True, horizontalalignment='center', fontsize=12)


    # Write speedup results to a text file
    with open("speedup_results.txt", "w") as f:
        f.write("Query Number\tSpeedup\tVelox Time\tQuery Time\n")
        for result in speedup_results:
            f.write(f"{result[0]}\t{result[1]:.2f}\t{result[2]:.2f}\t{result[3]:.2f}\n")


    plt.tight_layout()

    # Add legend for total execution times, number of speedup, and number of slowdown

    plt.savefig("velox_query_comparison.png")
    plt.show()

if __name__ == "__main__":
    main()
