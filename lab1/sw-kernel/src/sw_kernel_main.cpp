#include <stdlib.h>
#include "lnh64.h"
#include "gpc_io_swk.h"
#include "gpc_handlers.h"

#define SW_VERSION 0x28102023
#define __fast_recall__

extern lnh lnh_core;
volatile unsigned int event_source;

int main(void) {
    /////////////////////////////////////////////////////////
    //                  Main Event Loop
    /////////////////////////////////////////////////////////
    //Leonhard driver structure should be initialised
    lnh_init();
    for (;;) {
        //Wait for event
        event_source = wait_event();
        switch(event_source) {
            /////////////////////////////////////////////
            //  Measure GPN operation frequency
            /////////////////////////////////////////////
            case __event__(frequency_measurement) : frequency_measurement(); break;
            case __event__(get_lnh_status) : get_lnh_status(); break;
            case __event__(get_version): get_version(); break;
            case __event__(echo_mq): echo_mq(); break;

        }
        set_gpc_state(READY);
    }
}
    
//-------------------------------------------------------------
//      Глобальные переменные (для сокращения объема кода)
//-------------------------------------------------------------
    
        unsigned int LNH_key;
        unsigned int LNH_value;
        unsigned int LNH_status;
        uint64_t TSC_start,TSC_stop;
        int i,j;
        unsigned int err=0;
     

//-------------------------------------------------------------
//      Измерение тактовой частоты GPN
//-------------------------------------------------------------
 
void frequency_measurement() {
    
        sync();
        TSC_start = TSC; 
        sync();
        mq_send(TSC-TSC_start);

}


//-------------------------------------------------------------
//      Получить версию микрокода 
//-------------------------------------------------------------
 
void get_version() {
    
        mq_send(SW_VERSION);

}
   

//-------------------------------------------------------------
//      Получить регистр статуса LOW Leonhard 
//-------------------------------------------------------------
 
void get_lnh_status() {
    
        mq_send(LNH_STATE);
}

//-------------------------------------------------------------
//      Передача эхо пакетов через очереди сообщений
//-------------------------------------------------------------
 
void echo_mq() {
	unsigned long long data;
        while(1) {
                data = mq_receive();
                mq_send(data);
        }
}
