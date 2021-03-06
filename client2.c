/* Семинар 9, задание 10: Напишите, откомпилируйте и прогоните программы сервера и клиентов для предложенной
 схемы мультиплексирования сообщений.*/
 
 /*Клиент 2*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void exit();

	int main()
	{

	int msqid; /* IPC дескриптор для очереди сообщений */

	char pathname[] = "server.c"; /* Имя файла, использующееся для генерации ключа.
	Файл с таким именем должен существовать в текущей директории */

	key_t key; /* IPC ключ */

	int len, maxlen; /* Реальная длина и максимальная длина информативной части сообщения */

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
	mybuf;

	if((key = ftok(pathname,0)) < 0)
	 {
		printf("Can\'t generate key\n");
		exit(-1);
	 }

	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	 {
		printf("Can\'t get msqid\n");
		exit(-1);
	 }

	mybuf.mtype=1;
	mybuf.info.PID=getpid(); /*Идентификатор*/
	strcpy(mybuf.info.ms,"Hello from client!"); /*Передаваемое значение*/
	len=sizeof(mybuf.info); /*Размер сообщение*/

	if(msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0)<0) /*Функция отправки*/
	 {
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	 }


	maxlen = sizeof(mybuf.info);

	if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, getpid(), 0)) < 0) /*Функция приема*/
	 {
		printf("Can\'t receive message from queue\n");
		exit(-1);
	 }

	printf("message type = %ld, square =%s\n", mybuf.mtype,mybuf.info.ms);
	printf("\n");

	return 0;
}
