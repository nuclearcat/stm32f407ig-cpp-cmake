#include "main.h"
#include "cmsis_os.h"
#include "blinker.h"

Test::Test() {

}

void Test::blink_once (int delay) {
    osDelay(delay);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    osDelay(delay);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}
