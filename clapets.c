#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"
#include "stdint.h"
#include "hardware/clocks.h"

#define PIN_SERVO 15
#define PERIODE 20 // période de 20 ms

void InitServo() {
    stdio_init_all();
    gpio_set_function(PIN_SERVO, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PIN_SERVO); // Détermine le slice PWM associé à la broche GPIO spécifiée.
    uint channel = pwm_gpio_to_channel(PIN_SERVO); // Détermine le canal PWM associé à la broche GPIO spécifiée.

    pwm_set_clkdiv(slice_num, 256.0f); // Réglage du diviseur pour ralentir l'horloge
    pwm_set_wrap(slice_num, 9804); // Réglage du temps de cycle sur 9804 (20 ms)
    pwm_set_enabled(slice_num, true); // Active le signal PWM
}

void RotateServo() {
    uint slice_num = pwm_gpio_to_slice_num(PIN_SERVO); // Détermine le slice PWM associé à la broche GPIO spécifiée.
    uint channel = pwm_gpio_to_channel(PIN_SERVO); // Détermine le canal PWM associé à la broche GPIO spécifiée.
    pwm_set_chan_level(slice_num, channel, 750); // Réglage de la période d'impulsion (2,4 ms) -> 0 degré
    sleep_ms(2000); // Attente de 2 secondes
    pwm_set_chan_level(slice_num, channel, 196); // Réglage de la période d'impulsion (0,4 ms) -> 180 degrés
    sleep_ms(2000); // Attente de 2 secondes
}

int main() {
    uint slice_num = pwm_gpio_to_slice_num(PIN_SERVO); // Détermine le slice PWM associé à la broche GPIO spécifiée.
    uint channel = pwm_gpio_to_channel(PIN_SERVO); // Détermine le canal PWM associé à la broche GPIO spécifiée.
    InitServo();

    RotateServo(); // Effectue une rotation de 180 degrés
    return 0;
}
