# **ESP32 DHT22 Data Logger with Wi-Fi Download**  

## **Description**  
This project implements a **DHT22 temperature and humidity logger** using the **ESP32**. It logs sensor data to the **SPIFFS file system** every **30 seconds** and allows users to **export, clear, or download logs** over **Wi-Fi via HTTP**. The system also supports **serial command-line control** for flexibility.  


## **Hardware Requirements**  
- **ESP32 WROOM 32D**  
- **DHT22 Temperature & Humidity Sensor**  
- **220Ω Resistor (±5%)**  
- **USB Cable & Power Source**  

## **Connections**  
| Component  | ESP32 Pin |
|------------|----------|
| DHT22 Data | GPIO 4   |
| VCC        | 3.3V     |
| GND        | GND      |
| 220Ω Resistor | Between GPIO 4 & VCC |


## Features  
- **SPIFFS Storage**: Saves sensor readings in a file.  
- **Wi-Fi Connectivity**: Allows log file download over HTTP.  
- **Command-line Interface**: Controls logging via the serial monitor.  
- **Logging Interval**: Records data every 30 seconds.  
- **Web Server**: Hosts the log file for easy retrieval.  

## Installation & Setup  
1. **Clone the repository**  
   ```bash
   git clone https://github.com/yourusername/ESP32-DHT22-Logger.git  
   cd ESP32-DHT22-Logger  
   ```  

2. **Install the required libraries** in the **Arduino IDE**:  
   - `DHT sensor library`  
   - `ESP32 SPIFFS`  
   - `WiFi`  
   - `WebServer`  

3. **Upload the code** to the ESP32.  

## Usage  
- **Serial Commands:**  
  - `EXPORT` → Show log file data  
  - `CLEAR` → Erase log file  
  - `WIFI` → Toggle Wi-Fi & show download link  
  - `END` → Stop logging  
  - `HELP` → Show available commands  



## Demo  
https://github.com/user-attachments/assets/aedf5ca2-c170-4966-bd1d-de639a57aaed

## Web Interface  
Once connected to Wi-Fi, access the log file by visiting:  
```  
http://<ESP32-IP>/download  
```  



## License  
This project is open-source and available under the MIT License.  

