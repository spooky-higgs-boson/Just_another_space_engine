# Install dependencies
sudo apt update
sudo apt install build-essential cmake git
sudo apt install libglfw3-dev libglew-dev libglm-dev

# Create project directory
mkdir SpaceEngineClone
cd SpaceEngineClone

# Copy all the source files to their respective directories
# (src/, include/, shaders/)

# Build
mkdir build
cd build
cmake ..
make -j$(nproc)

# Run
./SpaceEngineClone
