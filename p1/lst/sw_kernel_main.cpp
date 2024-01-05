#include <stdlib.h>
#include <ctime>
#include "lnh64.hxx"
#include "gpc_io_swk.h"
#include "gpc_handlers.h"
#include "iterators.h"
#include "common_struct.h"
#include "compose_keys.hxx"

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
            case __event__(update) : update(); break;
            case __event__(select) : select(); break;
        }
        set_gpc_state(READY);
    }
}
    
//-------------------------------------------------------------
//      Вставка ключа и значения в структуру
//-------------------------------------------------------------

void update() {

        while(0){ // XXX
                users::key key=users::key::from_int(mq_receive());
                if (key==-1ull) break;
                users::val val=users::val::from_int(mq_receive());
                // Поля структуры могут записываться явно следующим образом 
                //      auto new_key = users::key{.rec_idx=1,.user=2};
                //      auto new_val = users::val{.role=3,.lst_time=0}
                // Копирование полей в переменные можно выполнить следующим образом:
                //      auto user = key.user;
                //      auto [lst_time,role] = val;
                USERS.ins_async(key,val); //Вставка в таблицу с типизацией uint64_t
        } 

        unsigned long long i = 0;
        while(1){ // XXX
            mystruct::key key = mystruct::key::from_int(mq_receive());
            if(key==-1ull) break;
            mystruct::val val = mystruct::val::from_int(mq_receive());

            if (++i % 2) {
                A.ins_async(key,val);
                mq_send(0ull);
                mq_send(key);
                mq_send(val);
            } else {
                B.ins_async(key,val);
                mq_send(1ull);
                mq_send(key);
                mq_send(val);
            }
        }
        //mq_send(-1ull);
}


//-------------------------------------------------------------
//      Передать все роли пользователя и время доступа 
//-------------------------------------------------------------

void select() {
        while(0){ // XXX
                uint32_t quser = mq_receive();
                if (quser==-1) break;
                uint32_t qtime = mq_receive();
                //Найдем все роли пользователя и последнее время доступа:
                // Результаты поиска могут быть доступны следующим образом:
                //      auto user = USERS.search(users::key{.idx=1,.user=2}).key().user;
                //      auto role = USERS.search(users::key{.idx=3,.user=4}).value().role;

                //Вариант 1 - обход записей пользователя явным образом
                auto crole = USERS.nsm(users::key{.idx=users::idx_min,.user=quser});
                while (crole && crole.key().user==quser) {
                       if (crole.value().time>qtime) mq_send(crole.value());
                       crole = USERS.nsm(crole.key());
                }

                //Вариант 2 - использование итератора
                // for (auto val : role_range(USERS,quser)) {
                //        if (val.time>qtime) mq_send(val);
                // }
                mq_send(-1ull);
        }

        //C.or_sync(A.struct_number, B.struct_number);
        lnh_or_sync(A.struct_number, B.struct_number, C.struct_number);

        uint64_t idx = 0;
        while(1){
            auto kv = C.search(mystruct::key{.idx=idx});
            if (kv) {
                auto k = kv.key().idx;
                auto v = kv.value().value;
                mq_send(k);
                mq_send(v);
            } else {
                break;
            }
            ++idx;
        }
        mq_send(-1ull);
}

