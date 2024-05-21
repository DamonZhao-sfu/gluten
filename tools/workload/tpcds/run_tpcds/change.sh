#!/bin/bash

# Directory containing the query_times files
DIRECTORY="./"

# Loop through all query_times*.txt files in the directory
for FILE in "$DIRECTORY"/query_times*.txt; do
  # Read the first line of the file to get the query path
  QUERY_PATH=$(head -n 1 "$FILE")
  
  # Extract the query identifier from the query path (allowing for numbers and letters)
  QUERY_IDENTIFIER=$(basename "$QUERY_PATH" | sed 's/\.sql//')
  
  # Construct the new filename
  NEW_FILENAME="${DIRECTORY}/query_times_${QUERY_IDENTIFIER}.txt"
  
  # Rename the file
  mv "$FILE" "$NEW_FILENAME"
  
  echo "Renamed $FILE to $NEW_FILENAME"
done

