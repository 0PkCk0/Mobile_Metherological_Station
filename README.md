# Mobile Metherological Station
Organizing something outdoor near your home, with your friends and family can be somewhat frustating due to the unpredictability of the weather. What of a great opportunity to build a metherological station! \
Thanks to the versatility, portability and a budget-friendly price of embedded systems, you can build your own mobile station, put it in you yard and monitor the weather from your office with little money.
For this projecy we used the power of esp32, a microcontroller with an outstanding price/value ratio, capable of many things and buyable with as little as 12 euros. Then we opted for the espidf framework instead of the arduino one: this chioce was made on the basis of having more control of the station and this pro outweighted the con of having far less documentation than the latter. \
Our ultimate purpose is to build a light and cheap metherological station, simple to maintain and transport but also as accurate as possible, in order to be reliable and useful. \
Now let's dig deeper into this project!

## Project Structure
![Alt text](images/project_flow.jpeg)

## Repository layout

![Alt text](images/undefined-2.jpg)
## Components and parts (hardware and software)
### Software
You can use any IDE which suits you better but we as a team recommend using VSCode with the platformIO extension installed.

<img width="298" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/f3d8d0d5-9367-47af-8279-133ea0da6c3e">

### Hardware
#### Pyrex bowl(to make the weather station waterproof)
![Alt text](images/pyrex.jpeg)
#### Esp32-Cam(with a fish-eye cam)  
![Alt text](images/ESP32-CAM.jpg)

The EspCam requires a power source given that it has not any usb ports and also a serial interface to program the microcontroller \
We opted for an ARDUINO NANO which fits perfectly our purpose.

![Alt text](images/arduino_nano.jpeg)

#### Esp32-S3 eye 
![Alt text](images/ESP32-S3-EYE-ezgif.com-resize.jpg)


## Getting Started
### Creating a Project
Having installed the PlatformIO IDE extension, we need to open our project: you can do it by clicking on the PlatformIO extension, going in the QUICK ACCESS section e go to Projects & Configuration 

<img width="349" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/4fe0d5c3-f4cd-46ec-b86a-ef2509abecb6">

Here we need to click on Create New Project 

<img width="663" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/9d4e5892-60d2-476f-921f-b359a668741e"> 

In the project wizard set these configurations and click finish 

<img width="600" alt="image" src="https://github.com/0PkCk0/Mobile_Metherological_Station/assets/62013889/3b57e867-f2db-4e3a-8541-1d8bb5be8ba7"> 

Now you are ready for the coding!
### Through the code

## Group divisions
(Terremo solo i nomi alla fine, se volete useremo username) \
Cerka Patrick : Wifi, ThingSpeak (API), Display \
Gore Stefan : AIMeteoDetection(FreeRTOS) \
Mattei Filippo : Humidity, Temperature, Led Status, Internal temperature, GPS  \
Pietri Francesco : Pressure, Temperature, Cloud Detection
