/**
 * @file adc_pot.c
 * @brief sensor reading and confirmation with LEDs
 * @author Briyith Guacas (briyithguacas@unicauca.edu.co)
 *         Karol Palechor (karolpalechor@unicauca.edu.co)
 *         Johana Puerres (johanapuerres@unicauca.edu.co)
 * \date 2024-05-03
 */

#include <xc.h>
#include <pic16f887.h>
#include "config.h"
#include <math.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Inicialización de funciones y variables.
 */
void read_temperature(void);
void read_luz(void);
void desplegar_LCD(void);
void apagarLed(void);
void read_microfono(void);
void read_potenciometro(void);
int lu, micro, pot, value_adc = 0, value_pot = 0;
float temp;
char bufferT[16], bufferM[16];

#define LED_RED PORTAbits.RA1
#define LED_BLUE PORTAbits.RA2
#define LED_GREEN PORTAbits.RA3


void main(void) {
    // Configuraciones iniciales de puertos
    OSCCON = 0x71;
    TRISA0 = 1;
    TRISA1 = 0;
    TRISA2 = 0;
    TRISA3 = 0;
    TRISE1 = 1;
    TRISB1 = 1;
    TRISB0 = 1;
    ANSEL = 0x41;
    ANSELH = 0x14;
    
    // Configuraciones del módulo ADC
    ADCON1bits.ADFM = 0; 
    ADCON1bits.VCFG0 = 0; 
    ADCON1bits.VCFG1 = 0; 
    ADCON0bits.ADCS = 0b01;
    
    //Inicialización del LCD
    LCD_Init();
    LCD_String_xy(0, 0, "Bienvenido.");
    __delay_ms(2000);
    LCD_Clear();
    
     // Encender LEDs de prueba
    LED_RED = 1;
    LED_BLUE = 1;
    LED_GREEN = 1;
    __delay_ms(2000);
    apagarLed();
    
    // Bucle principal
    while (1) {
        read_temperature();
        read_luz();
        read_microfono();
        read_potenciometro();
        desplegar_LCD();
        __delay_ms(1000);
        
        // Lógica de control de LEDs
        if (temp > 23) {
            LED_RED = 1;  // ROJO
        } else if (temp < 20 && lu > 11) { 
            LED_BLUE = 1; // AZUL
        } else {
            LED_GREEN = 1; // VERDE   
        }
        __delay_ms(3000);
        apagarLed();
        LCD_Clear();
    }
}


/**
 * @brief Función para leer la temperatura del sensor.
 */
void read_temperature(void) {
    ADCON0bits.CHS = 0b0000; /* Canal AN0 para temperatura */
    ADCON0bits.ADON = 1; /* Habilitar ADC */
    __delay_us(30);
    ADCON0bits.GO_DONE = 1; /* Iniciar conversión */
    while (ADCON0bits.GO_DONE); /* Esperar conversión */
    ADCON0bits.ADON = 0; /* Deshabilitar ADC */
    value_adc = 1023 - (ADRESH * 4); /* Calcular valor del sensor */
    temp = (float)(value_adc * 0.04058); /* Convertir a temperatura */
}

/**
 * @brief Función para leer la intensidad de luz.
 */
void read_luz(void) {
    ADCON0bits.CHS = 0b0110; /* Canal AN6 para luz */
    ADCON0bits.ADON = 1; /* Habilitar ADC */
    __delay_us(30);
    ADCON0bits.GO_DONE = 1; /* Iniciar conversión */
    while (ADCON0bits.GO_DONE); /* Esperar conversión */
    ADCON0bits.ADON = 0; /* Deshabilitar ADC */
    lu = ADRESH; /* Almacenar valor de luz */
}

/**
 * @brief Función para leer el nivel del micrófono.
 */
void read_microfono(void) {
    ADCON0bits.CHS = 0b1010; /* Canal AN10 para micrófono */
    ADCON0bits.ADON = 1; /* Habilitar ADC */
    __delay_us(30);
    ADCON0bits.GO_DONE = 1; /* Iniciar conversión */
    while (ADCON0bits.GO_DONE); /* Esperar conversión */
    ADCON0bits.ADON = 0; /* Deshabilitar ADC */
    micro = ADRESH; /* Almacenar valor del micrófono */
}

/**
 * @brief Función para leer el valor del potenciómetro.
 */
void read_potenciometro(void) {
    ADCON0bits.CHS = 0b1100; /* Canal AN12 para potenciómetro */
    ADCON0bits.ADON = 1; /* Habilitar ADC */
    __delay_us(30);
    ADCON0bits.GO_DONE = 1; /* Iniciar conversión */
    while (ADCON0bits.GO_DONE); /* Esperar conversión */
    ADCON0bits.ADON = 0; /* Deshabilitar ADC */
    value_pot = ADRESH * 4; /* Almacenar valor del potenciómetro */
    pot = (int)((value_pot * 5.0) / 1023.0); /* Convertir a escala 0-5V */
}


/**
 * @brief Desplegar la información en el LCD.
 */
void desplegar_LCD(void) {
    sprintf(bufferT, "T: %.2f  L: %d", temp, lu);
    LCD_String_xy(1, 1, bufferT);
    sprintf(bufferM, "M: %d  P: %d", micro, pot);
    LCD_String_xy(2, 1, bufferM);
}

/**
 * @brief Apagar todos los LEDs.
 */
void apagarLed(void) {
    LED_RED = 0;
    LED_BLUE = 0;
    LED_GREEN = 0;
}
