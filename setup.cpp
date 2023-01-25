/*********************************************************************************************
* setup.cpp: Skapar ett neuralt nätverk av klassen 'ann' där antal inputs, dolda lager, 
* dolda noder samt utgångar sätts. Därefter aktiveras GPIO's på RPI'n. Två dubbelvektorer 
* skapas där inmatning av träningsdata sker för in samt förväntad utdata. Efter att träning
* har skett så aktiveras tryckknapparna då knapparnas GPIO's sätts till inputs. 
* Detta görs för att undvika att utskrift via tryckknapp sker innan träning är klar. 
*********************************************************************************************/

#include "header.hpp"

ann ann1(4, 1, 4, 1);
static struct gpiod_chip *chip0 = gpiod_chip_open("/dev/gpiochip0");
struct gpiod_line *led = gpiod_chip_get_line(chip0, 17);
struct gpiod_line *button1 = gpiod_chip_get_line(chip0, 27);
struct gpiod_line *button2 = gpiod_chip_get_line(chip0, 22);
struct gpiod_line *button3 = gpiod_chip_get_line(chip0, 23);
struct gpiod_line *button4 = gpiod_chip_get_line(chip0, 24);

/*********************************************************************************************
* setup(): Dubbelvektorn train_in innehåller 16 index st med 4 index i sig. Dessa skall 
* motsvara alla olika scenarion för i vilken kombination de 4 st tryckknapparna trycks ned.
* För varje möjligt scenario via tryckknapparna läge ges även ett förväntat tillstånd som
* en lysdiond skall anta via dubbelvektorn train_out. Mönstret som träningsdatan uppfyller
* är att lysdioden endast skall tändas om OJÄMNT antal tryckknappar trycks ned, annars 
* så skall den vara släckt.  
*********************************************************************************************/

void setup(void)
{
    const std::vector<std::vector<double>> train_in = {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1}, 
                                                       {0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, 
                                                       {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1}, 
                                                       {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}};

    const std::vector<std::vector<double>> train_out = {{0}, {1}, {1}, {0}, 
                                                        {1}, {0}, {0}, {1}, 
                                                        {1}, {0}, {0}, {1}, 
                                                        {0}, {1}, {1}, {0}};

    std::cout << "Program start!\n";
    ann1.set_training_data(train_in, train_out);
    ann1.train(100000, 0.03);

    gpiod_line_request_output(led, "led", 0);
    gpiod_line_request_input(button1, "button1");
    gpiod_line_request_input(button2, "button2");
    gpiod_line_request_input(button3, "button3");
    gpiod_line_request_input(button4, "button4");
}
