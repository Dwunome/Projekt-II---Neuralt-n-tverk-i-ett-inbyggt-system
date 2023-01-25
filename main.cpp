/*********************************************************************************************
* main: Anropar funktionen setup() som tillgängliggör RPI's GPIO's samt skapar och tränar
* ett neuralt nätverk. En vektor skapas med storleken 4 och startvärden 0 som används för 
* lagring av 4st tryckknappar. Dessa knappar läses av via funktionen button_read(). 
* Funktionen led_write() skriver ut predikterat utvärde samt faktiskt utvärde i terminalen 
* samt tänder en LED om villkoren för detta är uppfyllda.
*
* Programmet kan köras via kommandot "make" i terminalen.
*********************************************************************************************/
#include "header.hpp"

int main()                 
{
    setup();
    
    while(true)
    {  
        std::vector<double> inputs(4,0);
        button_read(inputs);
        led_write(inputs);
    }
    return 0;
}
