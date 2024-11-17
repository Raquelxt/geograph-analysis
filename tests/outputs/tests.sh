#!/bin/bash

# Directory containing test input files
INPUT_DIR="tests/inputs"

# Directory containing expected output files
EXPECTED_DIR="tests/outputs"


PROGRAM="./bin/tp1"

TEMP_OUTPUT="temp_output.txt"

total_tests=0
failed_tests=0

check_dependencies() {
    if ! command -v bc &> /dev/null; then
        echo "Error: 'bc' is not installed. Please install it to proceed."
        exit 1
    fi
}

if [ ! -d "$INPUT_DIR" ]; then
    echo "Error: Input directory '$INPUT_DIR' does not exist."
    exit 1
fi

if [ ! -d "$EXPECTED_DIR" ]; then
    echo "Error: Expected output directory '$EXPECTED_DIR' does not exist."
    exit 1
fi

if [ ! -x "$PROGRAM" ]; then
    echo "Error: Program '$PROGRAM' does not exist or is not executable."
    exit 1
fi

check_dependencies

for input_file in "$INPUT_DIR"/*.txt; do
    # Check if there are no .txt files
    if [ ! -e "$input_file" ]; then
        echo "No .txt test files found in '$INPUT_DIR'."
        break
    fi

    filename=$(basename "$input_file")

    expected_output_file="$EXPECTED_DIR/$filename"

    if [ ! -f "$expected_output_file" ]; then
        echo "Warning: Expected output file '$expected_output_file' does not exist. Skipping test '$filename'."
        continue
    fi

    total_tests=$((total_tests + 1))

    "$PROGRAM" < "$input_file" > "$TEMP_OUTPUT"

    if ! diff -q "$TEMP_OUTPUT" "$expected_output_file" > /dev/null; then
        echo "Test '$filename': FAILED"
        failed_tests=$((failed_tests + 1))
        
        echo "Differences for '$filename':"
        diff -u "$expected_output_file" "$TEMP_OUTPUT"
        echo "-------------------------------------------------"
    else
        echo "Test '$filename': PASSED"
    fi
done

rm -f "$TEMP_OUTPUT"

if [ "$total_tests" -gt 0 ]; then
    fail_percent=$(printf "%.2f" "$(echo "scale=4; ($failed_tests / $total_tests) * 100" | bc)")
    echo "================================================="
    echo "Summary"
    echo "================================================="
    echo "Total Tests: $total_tests"
    echo "Failed Tests: $failed_tests"
    echo "Failure Rate: $fail_percent%"
else
    echo "No tests were executed."
fi
