#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <ctime>
#include "host_main.h"

using namespace std;

#define TEST_USER_COUNT 1000
#define TEST_ROLE_COUNT 1000

static unsigned long long seed{0x872d30b1eab1cd6e};
unsigned long long grn1024() {
    seed = 828090353856353729 * seed + 2309503295186496403;
    return seed % 1025;
}

int main(int argc, char** argv)
{
	ofstream log("lab2.log"); //поток вывода сообщений
	unsigned long long offs=0ull;
	gpc *gpc64_inst; //указатель на класс gpc
	regex select_regex_query("select +(.*?) +from +(.*?) +where +(.*?)=(.*?) +and +(.*?)>(.*);", //запрос
            std::regex_constants::ECMAScript | std::regex_constants::icase);

	//Инициализация gpc
	if (argc<2) {
		log<<"Использование: host_main <путь к файлу rawbinary>"<<endl;
		return -1;
	}

	//Захват ядра gpc и запись sw_kernel
	gpc64_inst = new gpc();
	log<<"Открывается доступ к "<<gpc64_inst->gpc_dev_path<<endl;
	cout<<"Открывается доступ к "<<gpc64_inst->gpc_dev_path<<endl;
	if (gpc64_inst->load_swk(argv[1])==0) {
		log<<"Программное ядро загружено из файла "<<argv[1]<<endl;
		cout<<"Программное ядро загружено из файла "<<argv[1]<<endl;
	}
	else {
		log<<"Ошибка загрузки sw_kernel файла << argv[1]"<<endl;
		return -1;
	}

	//Инициализация таблицы для вложенного запроса
	gpc64_inst->start(__event__(update)); //обработчик вставки 

if (0){
	//1-й вариант: пересылка коротких сообщений
	for (uint32_t user=0;user<TEST_USER_COUNT;user++) {
		for (uint32_t idx=0;idx<TEST_ROLE_COUNT;idx++,offs+=2) {
			gpc64_inst->mq_send(users::key{.idx=idx,.user=user}); //запись о роли #idx
			gpc64_inst->mq_send(users::val{.role=idx,.time=time_t(0)}); //роль и время доступа
		}
	}
}

if (0){
	//2-й вариант: блочная передача
	unsigned long long *buf = (unsigned long long*)malloc(sizeof(unsigned long long)*TEST_USER_COUNT*TEST_ROLE_COUNT*2);
	for (uint32_t user=0,offs=0;user<TEST_USER_COUNT;user++) {
		for (uint32_t idx=0;idx<TEST_ROLE_COUNT;idx++,offs+=2) {
			buf[offs]=users::key{.idx=idx,.user=user};
			buf[offs+1]=users::val{.role=idx,.time=time_t(idx*3600)};
		}
	}
	auto send_buf_th = gpc64_inst->mq_send(sizeof(unsigned long long)*TEST_USER_COUNT*TEST_ROLE_COUNT*2,(char*)buf);
	send_buf_th->join();
	free(buf);
}

    // XXX
    for (uint64_t idx=0; idx<(256+256); idx++) {
        gpc64_inst->mq_send(mystruct::key{.idx=idx});
        gpc64_inst->mq_send(mystruct::val{.value=grn1024()});

        auto where = gpc64_inst->mq_receive();
        auto k = gpc64_inst->mq_receive();
        auto v = gpc64_inst->mq_receive();
    
        if (where == 0ull) {
            cout << "inserted in A idx " << k << " v " << v << endl;
            log << "inserted in A idx " << k << " v " << v << endl;
        } else if (where == 1ull) {
            cout << "inserted in B idx " << k << " v " << v << endl;
            log << "inserted in B idx " << k << " v " << v << endl;
        }
    }

	//Терминальный символ
	gpc64_inst->mq_send(-1ull);

	gpc64_inst->start(__event__(select)); //обработчик запроса поиска 
    while(1) {
        uint64_t result_key = gpc64_inst->mq_receive();
        if (result_key!=-1ull) {
            uint64_t result_val = gpc64_inst->mq_receive();
            cout << "C idx: " << mystruct::key::from_int(result_key).idx << " - ";
            cout << "C value: " << mystruct::val::from_int(result_val).value << endl;
            log << "C idx: " << mystruct::key::from_int(result_key).idx << " - ";
            log << "C value: " << mystruct::val::from_int(result_val).value << endl;
        } else {
            cout << "done" << endl;
            log << "done" << endl;
            break;
        }
    }

	while(0) {
		string query1;
		//разбор полей запроса
		smatch match_query1;
		getline(cin, query1);
		log<<"Введен запрос: "<<query1<<endl;
		if (!query1.compare("exit")) {
			gpc64_inst->mq_send(-1ull);
			break;
		}
		if (regex_match (query1, match_query1, select_regex_query) && 
			match_query1[3]=="user" && 
			match_query1[5] == "time") {
			//match_query1[1] - возвращаемое поле запроса
			//match_query1[2] - номер структуры запроса 
			//match_query1[3] - поле поиска 1
			//match_query1[4] - значение поля поиска 1
			//match_query1[5] - поле поиска 2
			//match_query1[6] - значение поля поиска 2
			log << "Запрос принят в обработку." << endl;
			log << "Поиск ролей пользователя " << match_query1[4] << "и time > " << time_t(stoi(match_query1[6])) << endl;
			gpc64_inst->mq_send(stoi(match_query1[4])); //пользователь
			gpc64_inst->mq_send(stoi(match_query1[6])); //время доступа
			while (1) {
				uint64_t result = gpc64_inst->mq_receive();
				if (result!=-1ull) {
					cout << "Роль: " << users::val::from_int(result).role << " - ";
					cout << "Время доступа: " << users::val::from_int(result).time << endl;
				} else {
					break;
				}
			}
		} else {
      		log << "Ошибка в запросе!" << endl;
		}
	}
	log << "Выход!" << endl;
	return 0;
}

