// ==========================================
// SISTEMA POMODORO ESP32 - ANDRES QUITA ESTRES
// ==========================================
const int ledPins[] = {27, 26, 32, 33, 25};
const int numLeds = 5;

// Pin del Botón
const int buttonPin = 12;

// Tiempos (en milisegundos) - Usamos UL (Unsigned Long) para evitar desbordamientos
const unsigned long MINUTO = 60000UL;
const unsigned long INTERVALO_TRABAJO = 5UL * MINUTO;       // 5 min por LED
const unsigned long INTERVALO_DESCANSO_CORTO = 1UL * MINUTO;// 1 min por LED
const unsigned long INTERVALO_DESCANSO_LARGO = 3UL * MINUTO;// 3 min por LED

// Estados del Pomodoro
enum State { WORK, SHORT_BREAK, LONG_BREAK };
State currentState = WORK;

// Variables de Control de Tiempo
unsigned long previousMillis = 0;
unsigned long currentInterval = INTERVALO_TRABAJO;
unsigned long tiempoPausado = 0;

// Variables de Lógica
int currentLed = 0;
int ciclosCompletados = 0;
bool isRunning = false;
bool systemStarted = false; // Para que inicie apagado hasta el primer click

// Variables del Botón (Antirrebote / Debounce)
bool lastButtonState = false;
bool buttonState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  // Configurar pines
  pinMode(buttonPin, INPUT_PULLUP);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  manejarBoton();

  if (isRunning) {
    unsigned long currentMillis = millis();
    
    // Si ya pasó el tiempo de un intervalo (ej. 5 minutos)
    if (currentMillis - previousMillis >= currentInterval) {
      previousMillis = currentMillis; // Reiniciar contador de este intervalo
      currentLed++;
      
      // Si ya se llenaron los 5 LEDs (cumplimos el ciclo de 25, 5 o 15 min)
      if (currentLed >= numLeds) {
        avanzarEstado();
      }
      
      actualizarLEDs();
    }
  }
}

void manejarBoton() {
  // Leer estado del botón (invertido por el INPUT_PULLUP: presionado = true)
  bool lectura = !digitalRead(buttonPin);
  
  if (lectura != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lectura != buttonState) {
      buttonState = lectura;
      
      if (buttonState == true) { // Acción al presionar el botón
        if (!systemStarted) {
          // Primer inicio del sistema
          systemStarted = true;
          isRunning = true;
          previousMillis = millis();
          actualizarLEDs();
        } else {
          // Alternar Pausa / Reanudar
          if (isRunning) {
            // Guardamos el tiempo que llevaba acumulado en este intervalo
            tiempoPausado = millis() - previousMillis;
            isRunning = false;
          } else {
            // Al reanudar, restamos el tiempo acumulado al millis() actual
            // para que retome exactamente donde se quedó.
            previousMillis = millis() - tiempoPausado;
            isRunning = true;
          }
        }
      }
    }
  }
  lastButtonState = lectura;
}

void avanzarEstado() {
  currentLed = 0; // Reiniciamos el conteo de LEDs
  
  if (currentState == WORK) {
    ciclosCompletados++;
    if (ciclosCompletados >= 4) { // Después de 4 pomodoros
      currentState = LONG_BREAK;
      currentInterval = INTERVALO_DESCANSO_LARGO;
      ciclosCompletados = 0; // Reiniciar para la siguiente ronda
    } else {
      currentState = SHORT_BREAK;
      currentInterval = INTERVALO_DESCANSO_CORTO;
    }
  } else {
    // Si veníamos de cualquier descanso, volver a trabajar
    currentState = WORK;
    currentInterval = INTERVALO_TRABAJO;
  }
}

void actualizarLEDs() {
  if (!systemStarted) {
    return; // Si no ha iniciado, todo permanece apagado
  }

  if (currentState == LONG_BREAK) {
    // DESCANSO LARGO: Inician todos encendidos y se apagan uno a uno cada 3 min
    for (int i = 0; i < numLeds; i++) {
      if (i < currentLed) {
        digitalWrite(ledPins[i], LOW);  // Estos ya se apagaron
      } else {
        digitalWrite(ledPins[i], HIGH); // Estos siguen encendidos
      }
    }
  } else {
    // TRABAJO (25m) y DESCANSO CORTO (5m): Se van encendiendo de izquierda a derecha
    // Nota: El primer LED se enciende en el minuto 0 para representar el bloque en progreso
    for (int i = 0; i < numLeds; i++) {
      if (i <= currentLed) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}