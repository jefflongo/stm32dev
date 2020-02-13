#include "timer.h"

#include "stm32l0xx_hal.h"

TIM_HandleTypeDef htim2;

bool timer_init(void) {
    TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef sConfigOC = { 0 };

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 8191;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 499;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        return false;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_OC_Init(&htim2) != HAL_OK) {
        return false;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        return false;
    }
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        return false;
    }

    return true;
}

void timer_start(void) {
    HAL_TIM_Base_Start_IT(&htim2);
}

void timer_stop(void) {
    HAL_TIM_Base_Stop_IT(&htim2);
    htim2.Instance->CNT = 0;
}

void timer_handle_event(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void timer_clear(void) {
    htim2.Instance->CNT = 0;
    __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC1);
}
