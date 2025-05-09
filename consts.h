#ifndef CONSTS_H
#define CONSTS_H

// Порог срабатывания в g (0.5g = резкое движение)
constexpr double MOVEMENT_THRESHOLD = 0.5;
// Порог срабатывания в g^2
constexpr double MOVEMENT_THRESHOLD_SQUARE = MOVEMENT_THRESHOLD * MOVEMENT_THRESHOLD;
// Задержка между сообщениями (мс)
constexpr unsigned long MSG_DELAY = 2000;
// Период между замерами (мс)
constexpr unsigned long PROBE_DELAY = 100;

#endif