/* Семинар 9, задание 10:  Напишите, откомпилируйте и прогоните программы сервера и клиентов для предложенной
 схемы мультиплексирования сообщений.*/
 
/*Сервер*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{

	int msqid; /* IPC дескриптор для очереди сообщений */

	char pathname[] = "server.c"; 

	key_t key; /* IPC ключ */

	int len, maxlen; /* Cчетчик цикла и длина информативной части сообщения */

	struct mymsgbuf
	{
		long mtype;
		struct
		     {
				int PID;
				char ms[100];
		}
		info;
	} 
	mybuf; /*Обращение к структуре*/
	
	/* Генерируем IPC ключ из имени файла 09-1a.c в текущей директории и номера экземпляра очереди сообщений 0 */
	if((key = ftok(pathname,0)) < 0){
	printf("Can\'t generate key\n");
	exit(-1);
	}

	/* Пытаемся получить доступ по ключу к очереди сообщений, если она существует,
	или создать ее, если она еще не существует, с правами доступа
	read & write для всех пользователей */
	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
	printf("Can\'t get msqid\n");
	exit(-1);
	}

	while(1) /*Постоянная работа сервера*/
	    {
			maxlen = sizeof(mybuf.info);

			if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) /*Функция приема*/
			 {
				printf("Can\'t receive message from queue\n");
				exit(-1);
			 }

			mybuf.mtype = mybuf.info.PID;
			//strcpy(mybuf.info.ms,"Hello! I'm server!");
			strcat(mybuf.info.ms," Hello! I'm server!");
			len = sizeof(mybuf.info);

			if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) /*Отправка ответа*/
			 {
				printf("Can\'t send message to queue\n");
				msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
				exit(-1);
			 }
	    }

	return 0;
}
