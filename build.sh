#!/bin/bash

# Remove the build directory if it exists
if [ -d "build" ]; then
  echo "Removing existing build directory..."
  rm -rf build
fi

# Create new build directory
echo "Creating build directory..."
mkdir build
cd build

# Run CMake configuration
echo "Running CMake configuration..."
cmake ..

# Build the project
echo "Building the project..."
make -j$(nproc)


echo "Build completed!"
echo ""
echo "To run the application:"
echo "  build/examples/sprayer"
echo "  ./SprayerExample"
