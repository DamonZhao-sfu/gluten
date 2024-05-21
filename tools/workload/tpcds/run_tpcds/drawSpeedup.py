import matplotlib.pyplot as plt

# Read the speedup results from the text file
speedup_data = []
with open("speedup_results.txt", "r") as file:
    # Skip the header line
    next(file)
    for line in file:
        parts = line.strip().split("\t")
        query_number = parts[0]
        speedup = float(parts[1])
        speedup_data.append((query_number, speedup))

# Extract query numbers and speedup values
query_numbers = [data[0] for data in speedup_data]
speedup_values = [data[1] for data in speedup_data]

# Plot the bar diagram
plt.figure(figsize=(20, 10))  # Adjust figure size if needed
bars = plt.bar(query_numbers, speedup_values, color='skyblue')
plt.xlabel('Query Number')
plt.ylabel('Speedup')
plt.title('Speedup Comparison for Queries')
plt.xticks(rotation=45, ha='right')  # Rotate x-axis labels for better readability

# Add horizontal line at y = 1
plt.axhline(y=1, color='red', linestyle='--')

plt.tight_layout()

# Annotate each bar with its speedup value
for bar, speedup in zip(bars, speedup_values):
    plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.01, f'{speedup:.2f}', ha='center', va='bottom')

# Show the plot
plt.show()


# Save the plot locally
plt.savefig("speedup_result.png")

# Show the plot
plt.show()
