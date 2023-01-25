#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <gpiod.h>

void init_gpio()
{
    struct gpiod_chip* chip0 = gpiod_chip_open("/dev/gpiochip0");
    struct gpiod_line* led = gpiod_chip_get_line(chip0, 17);
    struct gpiod_line* button1 = gpiod_chip_get_line(chip0, 27);
    struct gpiod_line* button2 = gpiod_chip_get_line(chip0, 22);
    struct gpiod_line* button3 = gpiod_chip_get_line(chip0, 23);
    struct gpiod_line* button4 = gpiod_chip_get_line(chip0, 24);

    gpiod_line_request_output(led, "led", 0);
    gpiod_line_request_input(button1, "button1");
    gpiod_line_request_input(button2, "button2");
    gpiod_line_request_input(button3, "button3");
    gpiod_line_request_input(button4, "button4");
    
    const int led_on = gpiod_line_set_value(led, 1);
    const int button1_pressed = gpiod_line_get_value(button1);
    const int button2_pressed = gpiod_line_get_value(button2);
    const int button3_pressed = gpiod_line_get_value(button3);
    const int button4_pressed = gpiod_line_get_value(button4);

    return;
}


#endif /* GPIO_HPP_ */
