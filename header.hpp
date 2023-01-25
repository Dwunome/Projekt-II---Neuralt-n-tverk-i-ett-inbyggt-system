/*********************************************************************************************
* header.hpp: Inkluderar nödvändiga bibliotek. Globala variabler skapas för att kunna
* nå dessa från olika separata filer samt funktionsdeklarationer för funktioner.
*********************************************************************************************/
#ifndef HEADER_H_
#define HEADER_H_

#include "ann.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <gpiod.h>

extern ann ann1;
extern struct gpiod_line* led;
extern struct gpiod_line* button1;
extern struct gpiod_line* button2;
extern struct gpiod_line* button3;
extern struct gpiod_line* button4;

void setup(void);
void button_read(std::vector<double>& inputs);
void led_write(const std::vector<double>& inputs);

#endif /* HEADER_H_ */
