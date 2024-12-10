#!/bin/bash

# Name of the project executable
EXECUTABLE="./final_project"
PROJECT_DIR="/home/pi/471FinalProject"

# Go to project directory
cd $PROJECT_DIR

# Check if the executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Project '$EXECUTABLE' executable not found. Running make..."
    # Run make with all available processors
    make -j$(nproc)
    # Check if the build was successful
    if [ $? -ne 0 ]; then
        echo "Build failed. Exiting."
        exit 1
    fi
fi

# Run the executable
echo "Running '$EXECUTABLE'..."
"$EXECUTABLE"
if [ $? -ne 0 ]; then
    echo "Execution failed. Exiting."
    exit 1
fi
