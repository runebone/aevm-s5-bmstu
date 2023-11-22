#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "host_main.h"

#define BUF_SIZE 1000000*sizeof(unsigned int)

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char** argv)
{

	unsigned int err = 0;
	double LNH_CLOCKS_PER_SEC;
	gpc *gpc64_inst;

	if (argc<2) {
		printf("Usage: host_main <rawbinary file>\n");
		return -1;
	}

	//Захват ядра gpc и запись sw_kernel
	gpc64_inst = new gpc();
	printf("Open gpc on %s\n",gpc64_inst->gpc_dev_path);
	if (gpc64_inst->load_swk(argv[1])==0) {
		printf("Rawbinary loaded from %s\n",argv[1]);
	}
	else {
		printf("Rawbinary %s file error\n",argv[1]);
		return -1;
	}
	//Обработкчик для чтения версии sw_kernel
	gpc64_inst->start(__event__(get_version));
	printf("sw_kernel version: 0x%0llx\n", gpc64_inst->mq_receive());

	//Запуск обработчика для измерения тактовой частоты gpc
	gpc64_inst->start(__event__(frequency_measurement));
	gpc64_inst->sync();
	sleep(1);
	gpc64_inst->sync();
	LNH_CLOCKS_PER_SEC = (double)gpc64_inst->mq_receive();
	printf("Leonhard clock frequency (LNH_CF) %f MHz\n", LNH_CLOCKS_PER_SEC/1000000.0);

	//Обработкчик для посылки эхо-пакетов
	gpc64_inst->start(__event__(echo_mq));
	//Создание исходного массив
	unsigned long long *buf_out=(unsigned long long *)malloc(BUF_SIZE);
	unsigned long long *buf_in=(unsigned long long *)malloc(BUF_SIZE);
	for (unsigned int i=0;i<(BUF_SIZE>>3);i++) {
		buf_out[i]=((unsigned long long)rand()<<32)|rand();
	}
	//Запуск потоков приема-передачи
	auto send_thread = gpc64_inst->mq_send(BUF_SIZE,(char*)buf_out);
	auto receive_thread =  gpc64_inst->mq_receive(BUF_SIZE,(char*)buf_in);
	send_thread->join();
	receive_thread->join();
	for (unsigned int i=0;i<(BUF_SIZE>>3);i++) {
		if (buf_out[i]!=buf_in[i]) {
			printf("Error: buf_out[%d]=0x%016llx - buf_in[%d]=0x%016llx\n",i,buf_out[i],i,buf_in[i]);
			err++;
		}
	}
	if (!err) {printf("Test done\n");}

	// gpc64_inst->finish(); //newer finished

	//Освобождение ресурсов
	delete(gpc64_inst);
	return 0;
}

