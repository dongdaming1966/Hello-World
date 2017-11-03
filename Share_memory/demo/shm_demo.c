#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(void)
{
	int shmid; 			//shared memory sign
	int* shmid_mem; 		//to attach the share memory
	struct shmid_ds shmbuffer;	//information about the shared memory
//*********************************************************************	
//      struct shmid_ds {
//	      struct ipc_perm shm_perm;    // Ownership and permissions 
//            size_t          shm_segsz;   // Size of segment (bytes) 
//            time_t          shm_atime;   // Last attach time 
//            time_t          shm_dtime;   // Last detach time 
//            time_t          shm_ctime;   // Last change time 
//            pid_t           shm_cpid;    // PID of creator 
//            pid_t           shm_lpid;    // PID of last shmat(2)/shmdt(2) 
//            shmatt_t        shm_nattch;  // No. of current attaches 
//               ...
//	};
//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

	shmid = shmget(IPC_PRIVATE,0x6400,IPC_CREAT | S_IRUSR |S_IWUSR);	//allocate a shared memory segment
	if(shmid==-1)
		perror("Allocate the shared memory segment");
	printf("id:%d\n",shmid);

	shmid_mem = shmat(shmid,0,0);	//attach the shared memory segment
	printf("addr:%x\n",shmid_mem);	//address do not have a sign.use %x instead of %d

	shmctl(shmid,IPC_STAT,&shmbuffer);	//obtain information about the shared memory
	printf("size:%d\n",shmbuffer.shm_segsz);

	*shmid_mem = 555;		//write data into the shared memory
	
	shmdt(shmid_mem);		//detach the shared memory segment

	perror("write \"555\" to the share memory");

	shmid_mem = shmat(shmid,(void*)0x5000000,0);	//reattach the shared memory segment at a different address
	printf("new addr:%x\n",shmid_mem);
	printf("data:%d\n",*shmid_mem);

	shmdt(shmid_mem);

	shmctl(shmid,IPC_RMID,0);	//deallocate the shared memory segment

	return 0;
}
