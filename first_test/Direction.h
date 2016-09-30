
#define VMAX 0.35
#define VFMAX 0.4
#define VMIN 0.2

#define diff 0.3

#define diff2 -0.4


void FORWARD(int value)
{
    float servo=0.33;
    if(value>64) {
         value=value-64;
         servo=0.33+0.00925*value;
    }else{
         value=64-value;
         servo=0.33-0.00717*value;
        }
    
    TFC_SetServo(0,servo);
    TFC_SetMotorPWM(VFMAX,VFMAX);
    
}


void stop()
{
    TFC_SetServo(0,0.33);
    TFC_SetMotorPWM(0,0);
}


void TURN_LEFT(int value)
{

    float servo=0.33-0.00717*value;
    float vitesse=((VMAX-VMIN)/0.33)*servo+VMIN;
    TFC_SetServo(0,servo);
    TFC_SetMotorPWM(vitesse-diff,vitesse+diff);




}

void TURN_RIGHT(int value)
{


    float servo=0.33+0.00925*value;
    float vitesse=((VMIN-VMAX)/0.37)*servo+VMAX;
    TFC_SetServo(0,servo);
    TFC_SetMotorPWM(vitesse+diff,vitesse-diff2);


}
