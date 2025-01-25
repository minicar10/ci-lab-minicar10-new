#!/usr/bin/env bash

# Directories
TEST_DIR="testcases/week2"
INPUT_DIR="inputs"
OUTPUT_DIR="outputs"

# Create directories if they don't exist
mkdir -p "${INPUT_DIR}"
mkdir -p "${OUTPUT_DIR}"

# Loop through all `.s` files in the test directory
for TEST_FILE in ${TEST_DIR}/*.s; do
    TEST_NAME=$(basename "${TEST_FILE}" .s)

    echo "Testing: ${TEST_NAME}"

    # Save input file copy
    cp "${TEST_FILE}" "${INPUT_DIR}/${TEST_NAME}.s"

    # Run your program and save the output
    bin/ci -i "${TEST_FILE}" > "${OUTPUT_DIR}/${TEST_NAME}_my_output.txt"

    # Run the reference program and save the output
    bin/ci_reference -i "${TEST_FILE}" > "${OUTPUT_DIR}/${TEST_NAME}_ref_output.txt"

    # Compare outputs and save the diff
    if diff -u "${OUTPUT_DIR}/${TEST_NAME}_my_output.txt" "${OUTPUT_DIR}/${TEST_NAME}_ref_output.txt" > "${OUTPUT_DIR}/${TEST_NAME}_diff.txt"; then
        echo "✅ Passed: ${TEST_NAME}"
    else
        echo "❌ Failed: ${TEST_NAME} - See ${OUTPUT_DIR}/${TEST_NAME}_diff.txt"
    fi
done
