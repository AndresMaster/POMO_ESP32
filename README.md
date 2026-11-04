# POMO_ESP32 - Sistema Pomodoro Físico

## Finalidad
Este proyecto implementa un temporizador de la técnica Pomodoro de forma física y autónoma (Standalone) utilizando un microcontrolador ESP32. Su objetivo es ayudar a gestionar el tiempo de estudio o trabajo de manera visual sin depender de pantallas o aplicaciones, utilizando una interfaz minimalista de 5 LEDs y un único botón.

## Funcionamiento del Programa
El sistema gestiona el tiempo de forma matemática sin bloquear el procesador (usando `millis()`), lo que permite pausar y reanudar los ciclos con total exactitud. 

- **Modo Trabajo (25 minutos):** Los 5 LEDs se van encendiendo de izquierda a derecha. El primer LED se enciende al iniciar el bloque, y se suma un LED cada 5 minutos.
- **Descanso Corto (5 minutos):** Al finalizar los 25 minutos, los LEDs se apagan y comienzan a encenderse nuevamente a razón de un LED por minuto.
- **Descanso Largo (15 minutos):** Tras completar 4 ciclos de trabajo de forma consecutiva, los 5 LEDs se encienden al mismo tiempo y se van apagando progresivamente (uno cada 3 minutos).
- **Botón de Control:** Permite arrancar el sistema por primera vez, pausar el temporizador en cualquier instante y reanudarlo exactamente donde se quedó.

## Conexiones de Hardware (Pines)
- **LEDs:** D27, D26, D32, D33 y D25.
- **Botón:** D12 (Activado por estado LOW, con resistencia interna PULLUP).
