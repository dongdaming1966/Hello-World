#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>	
#include <sys/stat.h>	
#include <sys/time.h>
int main(void)
{
	int shmid; 			//shared memory sign
	char *shmid_mem; 		//to attach the share memory
	char com[100];
	struct shmid_ds shmbuffer;	//information about the shared memory
	int ret;
	int fd;
	struct timeval send,rev;

	ret=access("./shm_id.tmp",0);
	
	perror("shm_id.tmp");

	if(ret == -1)
	{
		fd = open("./shm_id.tmp",O_RDWR|O_CREAT);
		if(fd<0)	perror("open shm_id.tmp");
		
		shmid = shmget(IPC_PRIVATE,100,IPC_CREAT | S_IRUSR |S_IWUSR);	//allocate a shared memory segment
		if(shmid < 0)
			perror("Allocate the shared memory segment");
		printf("id:%d\n",shmid);
		
		shmid_mem = shmat(shmid,0,0);	//attach the shared memory segment
		shmid_mem[0]='\0';

		ret=write(fd,&shmid,sizeof(int));
		if(ret<0)	perror("write shmid");

		while(1)
		{
			if(shmid_mem[0]=='\0')
			{
				scanf("%s",shmid_mem);
				gettimeofday(&send,NULL);
				while(shmid_mem[0]!='\0');
				gettimeofday(&rev,NULL);
				printf("time used:%d\n",(rev.tv_sec-send.tv_sec)*1000000+rev.tv_usec-send.tv_usec);

			}
			if(!strcmp(shmid_mem,":quit"))	break;
		}

		shmdt(shmid_mem);

		close(fd);
	}
	else
	{
		fd = open("./shm_id.tmp",O_RDONLY);
		if(fd<0)	perror("open shm_id.tmp");

		read(fd,&shmid,sizeof(int));
		printf("read:%d\n",shmid);

		printf("ppress\"m\" to monitor the memory\n others for clean the shared memory directly\n");
		scanf("%s",com);
		if(!strcmp(com,"m"))
		{
			shmid_mem = shmat(shmid,0,0);
			do{
				if(shmid_mem[0]!='\0')
				{
					printf("receive:%s\n",shmid_mem);
					shmid_mem[0]='\0';
				}
			}while(strcmp(shmid_mem,":quit"));
			shmctl(shmid,IPC_RMID,0);	//deallocate the shared memory segment
		}		
		ret = close(fd);
		if(fd<0)	perror("close file");

		ret = remove("./shm_id.tmp");
		perror("remove");
	}
	return 0;
}
