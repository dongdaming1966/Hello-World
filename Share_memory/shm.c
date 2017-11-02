#include <stdio.h>
#include <shmdata.h>

struct shared_use_st
{
	int written; //0 for writable,others for readable
	char text[2048]; //text array
}

int main(void)
{
	int shmid; //shared memory sign

	shmid = shmget();
}
