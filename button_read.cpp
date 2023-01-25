/*********************************************************************************************
* button_read.cpp: 
* button_read(): Lagrar värdet 1 i vektorns respektive index ifall respektive tryckknapp 
* trycks ned. 4 st if-satser = 4 tryckknappar.
* 
* led_write(): 

* old_output används för att lagra tidigare värde för output för att undvika att samma värde
* skrivs ut oavbrutet i main-filens while-sats. 
* Prediktion av de inmatade värdena från tryckknapparna utförs via funktionen ann1.predict,
* och tänder 
*********************************************************************************************/
#include "header.hpp"


void button_read(std::vector<double>& inputs)
{           
    if (gpiod_line_get_value(button1))
    {
        inputs[3] = 1;
    }

    if (gpiod_line_get_value(button2))
    {
        inputs[2] = 1;
    }

    if (gpiod_line_get_value(button3))
    {
        inputs[1] = 1;
    }

    if (gpiod_line_get_value(button4))
    {
        inputs[0] = 1;
    }
  
    return;
}

void led_write(const std::vector<double>& inputs)
{
    static int old_output = 0;
    const auto prediction = ann1.predict(inputs)[0];
    const auto output = static_cast<int>(prediction + 0.5); 
    gpiod_line_set_value(led, output);

    std::cout << std::fixed;

    if (output != old_output)
    {
        if (prediction > -0.001 && prediction < 0.001) std::cout << "Predicted output: 0.0\n";
        else std::cout << "Predicted output: " << std::setprecision(1) << prediction << "\n";
        
        std::cout << "Output value: " << output << "\n\n";
        old_output = output;
    } 
}
