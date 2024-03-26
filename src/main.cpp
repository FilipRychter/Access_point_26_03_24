#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char* ssid = "";
const char* password = "";
const char* apSSID = "";
const char* apPassword = "";

TaskHandle_t taskHandle = NULL;

bool initializeSoftAP();

void taskFunction(void *parameter) {
    while(1) {
        // Sprawdzanie połączenia z routerem WiFi
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Brak połączenia. Ponowna próba połączenia...");
            WiFi.begin(ssid, password);
        }

        // Sprawdzanie połączenia z punktem dostępowym
        if (WiFi.softAPgetStationNum() == 0) {
            Serial.println("Brak klientów.");
        } else {
            Serial.print("Liczba klientów: ");
            Serial.println(WiFi.softAPgetStationNum());
        }

        vTaskDelay(pdMS_TO_TICKS(10000)); // Opóźnienie 10 sekund przed kolejnym sprawdzeniem połączenia
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Inicjalizacja WiFi w trybie Access Point
    while (!initializeSoftAP()) {
        Serial.println("Błąd inicjalizacji. Ponowna próba...");
        delay(1000);
    }

    xTaskCreate(
            taskFunction,        // Funkcja, która ma być wykonywana przez zadanie
            "WiFiCheckTask",     // Nazwa zadania
            10000,               // Stos zadania
            NULL,                // Parametr przekazywany do zadania
            1,                   // Priorytet zadania
            &taskHandle          // Uchwyt do zadania
    );
}

bool initializeSoftAP() {
    if (WiFi.softAP(apSSID, apPassword)) {
        Serial.println("Access Point został uruchomiony");
        Serial.print("SSID: ");
        Serial.println(apSSID);
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
        return true;
    } else {
        return false;
    }
}

void loop() {
    // Tutaj możesz umieścić kod do obsługi innych zadań w pętli głównej
    delay(1000);
}
