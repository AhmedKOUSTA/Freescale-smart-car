#include "mbed.h"
#include "TFC.h"
#include "Direction.h"

//This macro is to maintain compatibility with Codewarrior version of the sample.   This version uses the MBED libraries for serial port access
Serial PC(USBTX,USBRX);

#define TERMINAL_PRINTF     PC.printf
#define SENS 1500

//This ticker code is used to maintain compability with the Codewarrior version of the sample.   This code uses an MBED Ticker for background timing.

#define NUM_TFC_TICKERS 4


Ticker TFC_TickerObj;
int old_indice=64;
int indice_re=64;
volatile uint32_t TFC_Ticker[NUM_TFC_TICKERS];


int tab_min(void)
{

    double min=0;
    int indice=64;


    min=TFC_LineScanImage0[64];
    for(int i=20; i<108; i++) {
        if(TFC_LineScanImage0[i]<=min) {
            min=TFC_LineScanImage0[i];
            indice=i;
        }
    }

    if(min<SENS) {
        old_indice=indice;
        
        return indice;

    } else {
        return old_indice;
    }
}





void TFC_TickerUpdate()
{
    int i;

    for(i=0; i<NUM_TFC_TICKERS; i++) {
        if(TFC_Ticker[i]<0xFFFFFFFF) {
            TFC_Ticker[i]++;
        }
    }
    if(TFC_Ticker[0]>5 && TFC_LineScanImageReady>0) {
        TFC_Ticker[0] = 0;
        TFC_LineScanImageReady=0;
        indice_re=tab_min();
    }
}




//appele de la fonction: Direction();


int main()
{
    uint32_t t = 0;

    PC.baud(115200);
    TFC_TickerObj.attach_us(&TFC_TickerUpdate,2000);

    int state=0;
    TFC_Init();
    TFC_HBRIDGE_ENABLE;

    for(;;) {

        int erreur;

        if(TFC_PUSH_BUTTON_0_PRESSED) {
            state=1;

        }

        if(TFC_PUSH_BUTTON_1_PRESSED) {
            state=0;

        }

        switch(state) {
            case 0:
                //Let's put a pattern on the LEDs
                if(TFC_Ticker[1] >125) {
                    TFC_Ticker[1] = 0;

                    if(t==4) {
                        t=0;
                    } else
                        t=4;
                    TFC_SetBatteryLED_Level(t);
                    stop();
                }

                break;
            case 1:
                if(TFC_Ticker[3]>10) {

                    TFC_Ticker[3]=0;
                    //Let's put a pattern on the LEDs
                    if(t==0)
                        t=4;
                    else
                        t--;
                    TFC_SetBatteryLED_Level(t);

                    if(indice_re>80) {
                        erreur=indice_re-64;
                        TURN_RIGHT(erreur);


                    }  else if(indice_re<50) {
                        erreur=64-indice_re;
                        TURN_LEFT(erreur);

                    }

                    else if(indice_re>=50 && indice_re<=80) {
                        erreur=indice_re;
                        FORWARD(erreur);
                    } else
                        stop();
                }

                break;
        }
    }


}

