## Build Environment Setup

### Prerequisites

This project uses CMake to build an STM32F446ZE firmware with ROS 2 integration via Micro-ROS.

**System Requirements:**
- CMake >= 3.21
- ARM cross-compiler: `arm-none-eabi-gcc` (v14.2+)
- Python 3.9+
- Ninja or Make build tool (Ninja recommended)
- Docker (for Micro-ROS static library generation)

### Cloning the Repository

This project uses git submodules for external dependencies:
```bash
git clone https://github.com/yourusername/MoveoROS.git
cd MoveoROS
git submodule update --init --recursive
```

### Building Micro-ROS Static Library

Before building the firmware, generate the Micro-ROS static library for your STM32:
```bash
docker pull microros/micro_ros_static_library_builder:humble
docker run -it --rm -v $(pwd):/project \
  --env MICROROS_LIBRARY_FOLDER=external/micro_ros_stm32cubemx_utils/microros_static_library \
  microros/micro_ros_static_library_builder:humble
```

This generates `external/micro_ros_stm32cubemx_utils/microros_static_library/libmicroros/libmicroros.a`

### Building the Firmware
```bash
mkdir -p build/Debug
cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/gcc-arm-none-eabi.cmake ../..
cmake --build . -j$(nproc)
```

The compiled binary will be: `MoveoRos.elf`

### Flashing to Hardware

Using ST-Link:
```bash
st-flash write MoveoRos.bin 0x8000000
```

Or with OpenOCD:
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program MoveoRos.elf verify reset exit"
```

### Project Structure
```
MoveoROS/
├── Core/                          # STM32CubeMX generated code
├── Drivers/                       # STM32 HAL drivers
├── Middlewares/                   # FreeRTOS
├── external/
│   ├── arduino_compat/            # Arduino compatibility layer
│   ├── AccelStepper/              # Stepper motor library
│   ├── TMCStepper/                # TMC motor driver library
│   └── micro_ros_stm32cubemx_utils/  # Micro-ROS integration
├── cmake/                         # CMake build configuration
└── CMakeLists.txt
```

### Dependencies

- **AccelStepper**: Acceleration-controlled stepper motor library
- **TMCStepper**: Trinamic TMC stepper driver control library
- **Micro-ROS**: Lightweight ROS 2 for embedded systems
- **FreeRTOS**: Real-time operating system

### Troubleshooting

**CMake configuration fails:**
- Ensure `arm-none-eabi-gcc` is in your PATH: `which arm-none-eabi-gcc`
- Check that submodules are initialized: `git submodule status`

**Build fails with missing microros library:**
- Run the Docker build step above to generate `libmicroros.a`
- Verify the path: `ls external/micro_ros_stm32cubemx_utils/microros_static_library/libmicroros/libmicroros.a`

### Contributing

Contributions welcome! Please ensure:
- Code follows existing style
- All submodules remain unmodified
- CMake configuration is preserved

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

**Note:** This project incorporates AccelStepper (GPL v3), which requires any distributed derivative work to also be open-source under GPL v3 or compatible terms.

### Third-Party Libraries

This project uses the following open-source libraries with their respective licenses:

- **AccelStepper** - [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html)
- **TMCStepper** - [MIT License](https://opensource.org/licenses/MIT)
- **Micro-ROS STM32CubeMX Utils** - [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0)
- **FreeRTOS** - [MIT License](https://www.freertos.org/license.html)
- **STM32 HAL Drivers** - [BSD License](https://opensource.org/licenses/BSD-3-Clause)

Please refer to each library's repository for their complete license terms.

### License Compatibility Note

This project integrates GPL v3 licensed code (AccelStepper). If you distribute this firmware, you may need to make your source code available under compatible terms. See the [GPL v3 license](https://www.gnu.org/licenses/gpl-3.0.html) for details.