#include <Arduino.h>
#include <SPIFFS.h>
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>

#define DHT_PIN 4      
#define DHT_TYPE DHT22
#define LOG_FILE "/dht_log.txt" 
#define INTERVAL 30000  // 30 seconds

const char* ssid = "Neo"; // Enter the SSID
const char* password = "password"; // Enter the right password

DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);
bool wifiConnected = false; // To check WiFi status
bool stopLogging = false; // To stop logging when END command is given

// Logging sensor data in the file
void writeLogFile(float temperature, float humidity) {
    File file = SPIFFS.open(LOG_FILE, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open log file");
        return;
    }
    file.printf("Time : %lu, Temp : %.2f, Hum : %.2f\n", millis() / 1000, temperature, humidity);
    file.close();
}

// Read from the file and display in serial monitor
void readLogFile() {
    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (!file) {
        Serial.println("No log file found.");
        return;
    }
    Serial.println("----- DHT22 Sensor Readings -----");
    while (file.available()) {
        Serial.write(file.read());
    }
    Serial.println("\n----- END OF FILE -----");
    file.close();
}

// Clear the file
void clearLogFile() {
    SPIFFS.remove(LOG_FILE);
    Serial.println("Log file erased.");
}

// Downloading through WiFi
void Download() {
    File file = SPIFFS.open(LOG_FILE, FILE_READ);
    if (!file) {
        server.send(404, "text/plain", "Log file not found");
        return;
    }

    server.sendHeader("Content-Type", "text/plain");
    server.sendHeader("Content-Disposition", "attachment; filename=dht_log.txt");
    server.sendHeader("Connection", "close");

    String fileContent = "";
    while (file.available()) {
        fileContent += (char)file.read();
    }
    file.close();

    server.send(200, "text/plain", fileContent);
}

// Start Wi-Fi connection
void startWiFi() {
    if (wifiConnected) {
        WiFi.disconnect();
        Serial.println("WiFi disconnected.");
        wifiConnected = false;
    } else {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi...");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nWiFi connected! IP Address: " + WiFi.localIP().toString());

        server.on("/download", Download);
        server.begin();
        Serial.println(" Download file using : http://" + WiFi.localIP().toString() + "/download");

        wifiConnected = true;
    }
}

// HELP menu
void showHelp() {
    Serial.println("\nAvailable Commands:");
    Serial.println("EXPORT - Show log file data");
    Serial.println("CLEAR  - Erase log file");
    Serial.println("WIFI   - Connect/Disconnect WiFi & show link");
    Serial.println("END    - Terminate Process");
    Serial.println("HELP   - Show this help menu\n");
}

void setup() {
    Serial.begin(115200);
    dht.begin();

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        return;
    }

    Serial.println("Logging started. Type 'HELP' for available commands.");
}

void loop() {
    if (wifiConnected) {
        server.handleClient();  // Handle web requests
    }

    if (!stopLogging) { // stops when END is detected
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        
        if (!isnan(temperature) && !isnan(humidity)) {
            Serial.printf("Logged: Temp: %.2f°C, Humidity: %.2f%%\n", temperature, humidity);
            writeLogFile(temperature, humidity);
        } else {
            Serial.println("Failed to read from DHT sensor!");
        }

        // Check for user commands during delay
        unsigned long waitStart = millis();
        while (millis() - waitStart < INTERVAL) {
            if (Serial.available()) {
                String command = Serial.readStringUntil('\n');
                command.trim();

                if (command == "EXPORT") {
                    readLogFile();
                } else if (command == "CLEAR") {
                    clearLogFile();
                } else if (command == "WIFI") {
                    startWiFi();
                } else if (command == "END") {
                    stopLogging = true;
                    Serial.println("Logging stopped. Type 'HELP' for available commands.");
                } else if (command == "HELP") {
                    showHelp();
                }
            }
        }
    }
}
