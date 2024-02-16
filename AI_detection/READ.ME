--DEEP_LEARNING_ON_EDGE_DEVICE--

------------------------------------------------------------------------------

Introduction
The primary objective of this section is to explore the capabilities of the burgeoning field of AI within the constraints of small, efficient devices, similar to those found in embedded systems. Specifically, this involves utilizing an ESP32-EYE (ESP32S3, camera, LCD, and LED) to capture images of the surroundings and perform inference using the MobileNetv1 model.

------------------------------------------------------------------------------

Hardware Requirements
To complete this project, you'll need the following components:

ESP32S3-EYE: This is the main hardware component, providing a complete ecosystem for your embedded system.
Power Bank (Optional): This can be used for powering your setup on the go.
Host with ThinkSpeak Support: You'll need a host that supports ThinkSpeak for data communication.
I opted for the ESP32S3 because of its compatibility with the TensorFlow Lite Micro library. It's one of the few microcontrollers that support this library, thanks to its ample storage and significant RAM capacity.

Software Requirements
The primary software used in this project includes:

ESP-IDF: This is the main software development framework. It can be used as a standalone tool or as an extension.
Visual Studio Code (Optional): This is a popular code editor. While optional, it can greatly enhance the development experience.
(Note: Installing ESP-IDF as an extension, especially in tools like Visual Studio and Eclipse, is easier.)

Installation and Configuration
Installation instructions can be found at [URL]. I recommend installing it with Visual Studio Code, as all my documentation is based on this setup.
Once installed, follow these steps:

Create a new project, ensuring that the target device is set to ESP32S3 via USB to avoid potential errors.
Choose a simple "hello world" project template from the ESP repositories and modify it to suit your needs.

Libraries/Dependencies
There are two main ways to add libraries to your project:

Direct Download: You can directly download the required libraries using the following command in the ESP-IDF terminal:
idf.py download ...
Or write inside the file sdkbuild...

The dependencies for this project include:
TensorFlow
ESP Cam
ESP LCD


------------------------------------------------------------
PART OF THE PROJECT LAYOUT
(Here i will link all the folders and importnat files (not all))

------------------------------------------------------------
Weather Detection Model
The weather_detection_model.* file houses the TensorFlow Lite model converted into binaries and encapsulated within a C array. The conversion process is outlined in the file, detailing how the TensorFlow Lite model is prepared for integration into the project.

---------------------------------------------------------------------------------
Model Settings
Within model_settings.*, essential constants utilized across the entire project are defined. These constants include the output labels of the model and the dimensions of the video feed from the camera.
--------------------------------------------------------------------
Main Functions
In main_functions.*, the core implementation of the AI model is found.
model_setup_configuration(): This function aims to initialize the AI model, preparing it for inference on images captured by the ESP-Eye camera.
Utilizing tflite::GetModel(), the AI model is loaded into memory.
An important aspect is the initialization of the static tflite::MicroMutableOpResolver<5> micro_op_resolver;. Allocating the correct amount of memory to execute the model is crucial. Pay close attention to constexpr int kTensorArenaSize = 104 * 1024; to prevent errors during execution.

---------------------------------------------------------------------------
The image_provider.* module comprises several functionalities crucial for image processing within our application:

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

The detection_responder.* module contains the function responsible for post-inference tasks, such as printing the results of image classification.
This module:
Handles the processing of results obtained from the inference process.
Prints the classification results to an output stream or display.

--------------------------------------------------------------------------
The app_camera_esp.* files serve as the cornerstone for configuring the camera pins and initializing the sensor.
These files encompass:
Camera Pin Configuration: Various pins related to the camera are configured here to ensure proper communication and functionality.
Sensor Initialization: The sensor is initialized within these files, ensuring that it is ready to capture images and interface with other components of the system
-------------------------------------------------------

MODEL TRAINING
The main goal of this project was to put a machine learning model onto a small embedded system, rather than creating a fancy, complex AI model from scratch. I used a platform called EdgeImpulse for this task. It's great for quickly uploading labeled data and training models efficiently. I chose it over traditional Jupyter notebooks because it's much faster. Since I needed to try out several models due to memory constraints on the ESP32 devicE.

After training, I settled on using MobileNetV1, a well-known and reliable model known for its efficiency. It's particularly good for devices with limited resources.

Once the training was done, I downloaded the TensorFlow Lite version of the model, which is optimized and quantized for running on embedded platforms. To include it in my C project, I used a simple command to convert it into a C array:

xxd tf.tflite > tf.c
This made it easy to integrate the model into my C code for the embedded system.
