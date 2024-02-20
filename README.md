# Mobile Metherological Station
Organizing something outdoor near your home, with your friends and family can be somewhat frustating due to the unpredictability of the weather. What of a great opportunity to build a metherological station! \
Thanks to the versatility, portability and a budget-friendly price of embedded systems, you can build your own mobile station, put it in you yard and monitor the weather from your office with little money.
For this projecy we used the power of esp32, a microcontroller with an outstanding price/value ratio, capable of many things and buyable with as little as 12 euros. Then we opted for the espidf framework instead of the arduino one: this choice was made on the basis of having more control of the station and this pro outweighted the con of having far less documentation than the latter. \
Our ultimate purpose is to build a light and cheap metherological station, simple to maintain and transport but also as accurate as possible, in order to be reliable and useful. We also built and implemented an experimental AI model to being able to classify the clouds tipology, for the moment. \
Now let's dig deeper into this project!

## Project Structure
![Alt text](images/Workflow.png)

## Repository layout

![Alt text](images/undefined-3.jpg)
## Components and parts (hardware and software)
### Software
You can use any IDE which suits you better but we as a team recommend using VSCode with the platformIO extension installed.

<img width="298" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/f3d8d0d5-9367-47af-8279-133ea0da6c3e">

### Hardware
#### Pyrex bowl (to make the weather station waterproof)
![Alt text](images/pyrex.jpeg)
#### Esp32-Cam(with a fish-eye cam)  
![Alt text](images/ESP32-CAM.jpg)

The EspCam requires a power source given that it has not any usb ports and also a serial interface to program the microcontroller. \
We opted for an ARDUINO NANO which fits perfectly our purpose.

![Alt text](images/arduino_nano.jpeg)

#### Esp32-S3 eye 
![Alt text](images/ESP32-S3-EYE-ezgif.com-resize.jpg)

#### Sensors
BMP280 - temperature, pressure and altitude; \
DHT11 - Humidity; \
MQ135 - Smoke detector; \
Blue Led

## Getting Started
### Creating a Project
Having installed the PlatformIO IDE extension, we need to open our project: you can do it by clicking on the PlatformIO extension, going in the QUICK ACCESS section e go to Projects & Configuration 

<img width="349" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/4fe0d5c3-f4cd-46ec-b86a-ef2509abecb6">

Here we need to click on Create New Project 

<img width="663" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/9d4e5892-60d2-476f-921f-b359a668741e"> 

In the project wizard set these configurations and click finish 

<img width="600" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/3b57e867-f2db-4e3a-8541-1d8bb5be8ba7"> 

Now you are ready for the coding!
### Building and flashing the project into the microcontrollers
On VSCode, having the project open, you can see a bar below with many functions. \
The two functions we need are the verify and upload, respectively the "tick" and the "arrow" \

<img width="345" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/9b81150c-6a77-43d4-b8b0-d742a0a97a08"> \

## Code

### Cloud Detection

We decided to use the integrated camera of the ESP32cam in order to create a simple and lightweight algorithm for the analysis of the sky above the weather station. The program returns the percentage of Sun, Sky, Cloud, Error (things that don't belong to the sky).

First of all, we took many pictures of the sky using an OV2640 fish eye camera installed on the ESP32cam under different types of weather and light conditions. Then, using a python program and the library OpenCV for image processing we estimated the HSV range for Cloud, Sun and Sky.

We used the HSV color space since it is more robust towards external lighting changes and is often used in computer vision and image processing instead of RGB.

Once the program in python was completed we created a lightweight version in C without  using open CV (way too heavy for our project).
Particularly, the C program takes a picture and saves it in a RGB565 standard and returns a pointer to the frame buffer of the image. Then, we access pixel by pixel using the pointer, we convert it in RGB888 and then we convert it in HSV (using the same ranges of OpenCV H: 0-180 S: 0-255 V: 0-255).

Once we have the HSV value of the pixel we just classifie it using the HSV ranges discovered using the python program. Eventually, when all 320x240 pixels are classified we calculate the relative percentage.

![Alt text](images/CloudDetection.png)

### BMP280:

General Specifications:
Operating Voltage: 1.71V to 3.6V
Operating Temperature: -40°C to 85°C.
Operating Pressure: 300hPa to 1100hPa.

Communication Protocol ESP32cam ←→ BMP280:
We decided to use the I2C protocol since it uses less wires than the SPI and provides an optimal speed and reliability for our application.
Even more, ESP32cam has a really poor number of GPIO pins and the default I2C pins are used by the camera, so we used the pins 14 (SCL) and 15 (SDA) by properly enabling the pull-up resistors.

The altitude is derived from the pressure using the international barometric formula, so it should be considered approssimative.
H = 44330 * [1 - (P/p0)^(1/5.255) ]

H = altitude (m)
P = measured pressure (Pa) from the sensor
p0 = reference pressure at sea level (e.g. 1013.25hPa, ajust it based on your location for higher precision)

Drivers Used:
https://github.com/UncleRus/esp-idf-lib

### DHT11:

General Specification:
Operating Voltage: 3.3V
Humidity Range: 20% to 90%
Accuracy: +-5%

Communication Protocol ESP32cam ←→ DHT11:
Note:
A suitable pull-up resistor should be connected to the selected GPIO line (We have abilitated the internal Pull-up resistor of GPIO 2)

    __           ______          _______                              ___________________________
     \    A    /      \   C    /       \   DHT duration_data_low    /                           \
      \_______/   B    \______/    D    \__________________________/   DHT duration_data_high    \__
 
Initializing communications with the DHT requires four 'phases' as follows:
 
Phase A - MCU pulls signal low for at least 18000 us
Phase B - MCU allows signal to float back up and waits 20-40us for DHT to pull it low
Phase C - DHT pulls signal low for ~80us
Phase D - DHT lets signal float back up for ~80us
 
After this, the DHT transmits its first bit by holding the signal low for 50us
and then letting it float back high for a period of time that depends on the data bit.
duration_data_high is shorter than 50us for a logic '0' and longer than 50us for logic '1'.

There are a total of 40 data bits transmitted sequentially. These bits are read into a byte array
of length 5.  The first and third bytes are humidity (%) and temperature (C), respectively.  Bytes 2 and 4
are zero-filled and the fifth is a checksum such that:
 
byte_5 == (byte_1 + byte_2 + byte_3 + byte_4) & 0xFF

Drivers Used:
https://github.com/UncleRus/esp-idf-lib

### MhMq135

General Specification:
Operating voltage: 5 V
Detects: NH3, NOx, alcohol, Benzene, smoke, CO2, etc.

Communication Protocol:
The digital output of the MQ135 is setted to 1 when the number of toxic substance is below a threshold and 0 when is above. The threshold is determined by a potentiometer on chip.
Since the sensor need to be preheated for at least 1 minute, the first lecture should be done after at least that time.

## DEEP LEARNING ON EDGE DEVICE

### Introduction
The primary objective of this section is to explore the capabilities of the burgeoning field of AI within the constraints of small, efficient devices, similar to those found in embedded systems. Specifically, this involves utilizing an ESP32-EYE (ESP32S3, camera, LCD, and LED) to capture images of the surroundings and perform inference using the MobileNetv1 model.

------------------------------------------------------------------------------

### Hardware Requirements
To complete this project, you'll need the following components:

ESP32S3-EYE: This is the main hardware component, providing a complete ecosystem for your embedded system.
Power Bank (Optional): This can be used for powering your setup on the go.
Host with ThinkSpeak Support: You'll need a host that supports ThinkSpeak for data communication.
I opted for the ESP32S3 because of its compatibility with the TensorFlow Lite Micro library. It's one of the few microcontrollers that support this library, thanks to its ample storage and significant RAM capacity.

-------------------------------------------------------------------------------

### Software Requirements
The primary software used in this project includes:

#### ESP-IDF: This is the main software development framework. It can be used as a standalone tool or as an extension.
Visual Studio Code (Optional): This is a popular code editor. While optional, it can greatly enhance the development experience.
(Note: Installing ESP-IDF as an extension, especially in tools like Visual Studio and Eclipse, is easier.)

#### Installation and Configuration
Installation instructions can be found at https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/. I recommend installing it with Visual Studio Code, as all my code is based on this setup.

#### Libraries/Dependencies
There are two main ways to add libraries to your project:
TensorFlow
ESP Cam
ESP LCD

------------------------------------------------------------
### Weather Detection Model
The weather_detection_model.* file houses the TensorFlow Lite model converted into binaries and encapsulated within a C array. The conversion process is outlined in the file, detailing how the TensorFlow Lite model is prepared for integration into the project.

---------------------------------------------------------------------------------

### Model Settings
Within model_settings.*, essential constants utilized across the entire project are defined. These constants include the output labels of the model and the dimensions of the video feed from the camera.

--------------------------------------------------------------------
### Main Functions
In main_functions.*, the core implementation of the AI model is found.
model_setup_configuration(): This function aims to initialize the AI model, preparing it for inference on images captured by the ESP-Eye camera.
Utilizing tflite::GetModel(), the AI model is loaded into memory.
An important aspect is the initialization of the static tflite::MicroMutableOpResolver<5> micro_op_resolver;. Allocating the correct amount of memory to execute the model is crucial. Pay close attention to constexpr int kTensorArenaSize = 104 * 1024; to prevent errors during execution.

---------------------------------------------------------------------------
### image_provider.* 
This module comprises several functionalities crucial for image processing within our application:
Camera Initialization: app_camera_init initializes the camera to capture images.
Buffer Allocation: We allocate a buffer to store pixels for display on the LCD screen.
GetImage Function: This function, GetImage, holds significant importance as it determines which pixels are fed into the model and which are displayed on LCD.
The image format utilized is RGB 565. Consequently, the esp_camera_fb_get function returns a structure wherein the buf field contains the pixel data in 16-bit format.

To extract RGB information from these 16-bit pixels, we employ the following mask:

uint8_t hb = pixel & 0xFF;
uint8_t lb = pixel >> 8;
uint8_t r = (lb & 0x1F) << 3;
uint8_t g = ((hb & 0x07) << 5) | ((lb & 0xE0) >> 3);
uint8_t b = (hb & 0xF8);
These r, g, and b values are then stored within image_data, which serves as the input for the AI model.

------------------------------------------------------------------------

### detection_responder.*
The module contains the function responsible for post-inference tasks, such as printing the results of image classification.
This module:
Handles the processing of results obtained from the inference process.
Prints the classification results to an output stream or display.

--------------------------------------------------------------------------
### app_camera_esp.* 
These files serve as the cornerstone for configuring the camera pins and initializing the sensor.
These files encompass:
Camera Pin Configuration: Various pins related to the camera are configured here to ensure proper communication and functionality.
Sensor Initialization: The sensor is initialized within these files, ensuring that it is ready to capture images and interface with other components of the system

-------------------------------------------------------

### MODEL TRAINING
The main goal of this project was to put a machine learning model onto a small embedded system, rather than creating a fancy, complex AI model from scratch. I used a platform called EdgeImpulse for this task. It's great for quickly uploading labeled data and training models efficiently. I chose it over traditional Jupyter notebooks because it's much faster. Since I needed to try out several models due to memory constraints on the ESP32 devicE.

After training, I settled on using MobileNetV1, a well-known and reliable model known for its efficiency. It's particularly good for devices with limited resources.

Once the training was done, I downloaded the TensorFlow Lite version of the model, which is optimized and quantized for running on embedded platforms. To include it in my C project, I used a simple command to convert it into a C array:

xxd tf.tflite > tf.c
This made it easy to integrate the model into my C code for the embedded system.

## WiFi Connection with ESP32-S3 Eye

This project demonstrates how to establish a WiFi connection using an ESP32-S3 Eye microcontroller without the need for a web server. The code is written in C++ and utilizes the ESP-IDF framework.

### Prerequisites

- ESP-IDF development environment set up
- ESP32-S3 development board

### Getting Started

1. Clone this repository:

    ```bash
    git clone (https://github.com/0PkCk0/Mobile_Metherological_Station.git)
    ```

2. Navigate to the project directory:

    ```bash
    cd <project_directory>
    ```

3. Open the project in your preferred IDE.

4. Ensure that your ESP32-S3 board is connected to your development machine via USB.

5. Build and flash the project to your ESP32-S3 board.

6. Monitor the serial output to observe the WiFi connection process.

### WiFi Connection Process

The WiFi connection process involves the following steps:

1. Initialize the ESP-IDF framework and configure the WiFi connection parameters (SSID and password).

2. Set up event handlers to handle WiFi events such as connection status changes.

3. Start the WiFi connection process.

4. Wait for the ESP32-S3 to connect to the specified WiFi network.

5. Once connected, obtain the IP address assigned to the ESP32-S3.

6. Monitor the serial output for status updates and confirmation of successful WiFi connection.

### Code Structure

- `main.c`: Contains the main application code, including the initialization of the WiFi connection process.

- `wifi.h`: Header file containing the declaration of the `wifi_connection` function.

- `wifi.c`: Source file containing the implementation of the `wifi_connection` function.

### Usage

To use the WiFi connection functionality in your own project, follow these steps:

1. Copy the `wifi.h` and `wifi.c` files into your project directory.

2. Include the `wifi.h` header file in your main application file.

3. Call the `wifi_connection` function from your application code to initiate the WiFi connection process.

4. Monitor the serial output for WiFi connection status updates and IP address assignment.

### Troubleshooting

If you encounter any issues with the WiFi connection process, consider the following troubleshooting steps:

- Check the WiFi network credentials (SSID and password) configured in your code.

- Ensure that your ESP32-S3 board is within range of the WiFi network and that the network is operational.

- Verify that the ESP-IDF development environment is set up correctly and that the necessary components are installed.

- Check the serial output for error messages or status updates that may indicate issues with the WiFi connection process.

### Contributors

- [0PkCk0](https://github.com/0PkCk0)

### License

This project is licensed under the [MIT License](LICENSE).


## Video and presentation
### [Presentation](https://prezi.com/view/psKmNETbN1d7AcHLGIIs/)
### [Video](https://www.youtube.com/watch?v=L7zIJ5Sh3cw)
## Group members
Cerka Patrick(0PkCk0) : Wifi, ThingSpeak (API), Repository \
Gore Stefan(StefanGore) : AIMeteoDetection \
Mattei Filippo(FilippoMattei02) : Humidity, Smoke detector, Led Status  \
Pietri Francesco(FrancescoPietri) : Pressure, Temperature, Altitude, Cloud Detection
