# M5Stack CoreS3 + LVGL9.2 + SquareLine Studio Template

This project provides a ready-to-use template for rapid prototyping using M5Stack CoreS3, LVGL 9.2, and SquareLine Studio. It includes basic setup for WiFi configuration, display initialization, and UI components.

## Features

- Pre-configured LVGL 9.2 setup for M5Stack CoreS3
- Integrated WiFi management with QR code configuration
- Basic UI components including:
  - Startup animation
  - WiFi configuration page
  - Status indicators
- Sound playback support
- SPIFFS file system integration

## Prerequisites

- [PlatformIO](https://platformio.org/) or Arduino IDE
- [SquareLine Studio](https://squareline.io/) (for UI design)
- [M5Stack CoreS3](https://shop.m5stack.com/products/m5stack-cores3-esp32s3-lotdevelopment-kit) development kit

## Project Structure

```
├── src/
│   ├── UI/                 # Generated UI files from SquareLine Studio
│   ├── main.cpp           # Main application entry
│   ├── wifi_setup.cpp     # WiFi configuration
│   ├── ui_cb.cpp          # UI callbacks
│   ├── util.cpp           # Utility functions
│   └── global.hpp         # Global definitions
```

## Getting Started

1. Clone this repository
2. Open the project in PlatformIO or Arduino IDE
3. Configure your WiFi AP credentials for WIFI Provisioning using tzapu/WiFiManager in `global.hpp`:
   ```cpp
   #define WIFI_SSID "M5STACK_S3"
   #define WIFI_PASS "45671234"
   ```
4. Build and upload to your M5Stack CoreS3

## WiFi Configuration

The template had integrated WiFiManger for WiFi configuration:

1. **WiFi Provisioning**: Easy WiFi Config through scanning QR code to enter config portal 
2. **WiFi Status**: Monitor WiFi Connection Status 

## UI Customization

1. Open SquareLine Studio
2. Import the UI project from the `SquareLine` directory
3. Modify the UI design
4. Export UI Files back to the src/UI directory

## Display Management

The template uses a dedicated core for LVGL tasks:
```cpp
#define LVGL_TASK_CORE 1
#define LVGL_TASK_PRIORITY 5
#define LVGL_STACK_SIZE 32768
```

## Event Handling

The template provides several event handlers:
- `PlayStartupMusic`: Plays startup sound
- `onWifiConnected`: Handles successful WiFi connection
- `onWifiConfig`: Manages WiFi configuration mode
- Screen load/unload events

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## License

This project is released under the MIT License.

## Acknowledgments

- [M5Stack](https://m5stack.com/) for the CoreS3 hardware
- [LVGL](https://lvgl.io/) for the graphics library
- [SquareLine Studio](https://squareline.io/) for the UI design tool

## Support

For questions and support:
- Create an issue in the repository
- Visit [M5Stack Forum](https://forum.m5stack.com/)
- Check [LVGL Documentation](https://docs.lvgl.io/)