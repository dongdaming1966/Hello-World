#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <stdlib.h>
//#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>	
#include <sys/stat.h>	

int main(void)
{
	int shmid; 			//shared memory sign
	int* shmid_mem; 		//to attach the share memory
	struct shmid_ds shmbuffer;	//information about the shared memory
	int ret;
	int fd;

	ret=access("./shm_id.tmp",0);
	
	perror("shm_id.tmp");

	if(ret == -1)
	{
		fd = open("./shm_id.tmp",O_RDWR|O_CREAT);
		if(fd<0)	perror("open shm_id.tmp");
		
		shmid = shmget(IPC_PRIVATE,0x6400,IPC_CREAT | S_IRUSR |S_IWUSR);	//allocate a shared memory segment
		if(shmid < 0)
			perror("Allocate the shared memory segment");
		printf("id:%d\n",shmid);
		
		ret=write(fd,&shmid,sizeof(int));
		if(ret<0)	perror("write shmid");


		shmid_mem = shmat(shmid,0,0);	//attach the shared memory segment
		printf("addr:%x\n",shmid_mem);	//address do not have a sign.use %x instead of %d

		shmctl(shmid,IPC_STAT,&shmbuffer);	//obtain information about the shared memory
		printf("size:%d\n",shmbuffer.shm_segsz);

		*shmid_mem = 621;		//write data into the shared memory
		printf("press enter to quit......");
		getchar();	
		shmdt(shmid_mem);

		close(fd);
	}
	else
	{
		fd = open("./shm_id.tmp",O_RDONLY);
		if(fd<0)	perror("open shm_id.tmp");

		read(fd,&shmid,sizeof(int));
		printf("read:%d\n",shmid);

		shmid_mem = shmat(shmid,0,0);

		printf("receive:%d\n",*shmid_mem);
		
		printf("press enter to quit......");
		getchar();	

		shmctl(shmid,IPC_RMID,0);	//deallocate the shared memory segment
		
		ret = close(fd);
		if(fd<0)	perror("close file");

		ret = remove("./shm_id.tmp");
		perror("remove");
	}
	return 0;
}
