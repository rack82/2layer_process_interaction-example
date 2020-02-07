#include "util.h"

int main(int argc, char *argv[]){

int i,j, shmtime, shm_A, shm_B, shm_C, ident = 0, endTime, semid, id;
pid_t pidA, pidB, pidC;
char *Dstring = NULL, *sid, ch, *StrTime;

int *flow;
struct sh_string *ptrA, *ptrB, *ptrC;

shmtime = shmget(5678, sizeof(int*), 0600|IPC_CREAT);
shm_A = shmget(1234, sizeof(struct sh_string), 0600|IPC_CREAT);
shm_B = shmget(3456, sizeof(struct sh_string), 0600|IPC_CREAT);
shm_C = shmget(6789, sizeof(struct sh_string), 0600|IPC_CREAT);
semid = semget(4567, 7, 0600|IPC_CREAT);

seminit(semid, 0, 0); //Parent distribute semaphore
seminit(semid, 1, 1); //Process A write out-buf
seminit(semid, 2, 0); //Process A read in-buf
seminit(semid, 3, 1); //Process B write out-buf
seminit(semid, 4, 0); //Process B read in-buf
seminit(semid, 5, 1); //Process C write out-buf
seminit(semid, 6, 0); //Process C read in-buf


flow = shmat(shmtime, NULL, 0);
ptrA = shmat(shm_A, NULL, 0);
ptrB = shmat(shm_B, NULL, 0);
ptrC = shmat(shm_C, NULL, 0);

*flow = 0;
endTime = atoi(argv[1]); //simulation limit

/*calling the children one by one*/
pidA = fork();
if (pidA == 0)
  execl("ProcessA.exe", "ProcessA.exe", argv[1], NULL);

pidB = fork();
if (pidB == 0)
  execl("ProcessB.exe", "ProcessB.exe", argv[1], NULL);

pidC = fork();
if (pidC == 0)
  execl("ProcessC.exe", "ProcessC.exe", argv[1], NULL);

printf("Loading....\n");

/*Simulation is defined by how many iterations will be executed.
Flow is a shared int counter (all processes have access to it) that determines
the number of iterations. It is defined by the user.
*/

while (*flow < endTime){
  down(semid, 0, 3); //stuck until all children fill (if they have to) their out buf

  /*when all children have written in their out-buf,
    parent checks each one and sends the string to the
    referenced in-buffer.
  */
  if ((int)strlen(ptrA->out) != 0){
    id = ptrA->out[0];
    Dstring = malloc(((int)strlen(ptrA->out) + 1) * sizeof(char));
    strcpy(Dstring, ptrA->out);
    strcpy(ptrA->out, "");
    switch(id){
	     case 1:{
		       strcpy(ptrA->in, Dstring);
	     }
       case 2:{
		       strcpy(ptrB->in, Dstring);
	     }
	     case 3: {
		       strcpy(ptrC->in, Dstring);
	     }
    }

    /*when parent finishes writing the string to the particular
      in-buffer, enables the children to start checking their in-buffers.
    */
    free(Dstring);
    up(semid, 2, 1);
    up(semid, 4, 1);
    up(semid, 6, 1);
    down(semid, 0, 3);
 }
 else{
   up(semid, 2, 1);
   up(semid, 4, 1);
   up(semid, 6, 1);
   down(semid, 0, 3);
 }
if ((int)strlen(ptrB->out) != 0){
	id = ptrB->out[0];
	Dstring = malloc(((int)strlen(ptrB->out) + 1) * sizeof(char));
	strcpy(Dstring, ptrB->out);
	strcpy(ptrB->out, "");
	switch(id){
		case 1:{
			strcpy(ptrA->in, Dstring);
		}
		case 2:{
			strcpy(ptrB->in, Dstring);
		}
		case 3:{
			strcpy(ptrC->in, Dstring);
		}
  }
  free(Dstring);
  up(semid, 2, 1);
  up(semid, 4, 1);
  up(semid, 6, 1);
  down(semid, 0, 3);
}
else{
  up(semid, 2, 1);
  up(semid, 4, 1);
  up(semid, 6, 1);
  down(semid, 0, 3);
}
  if ((int)strlen(ptrC->out) != 0){
	id = ptrC->out[0];
	Dstring = malloc(((int)strlen(ptrC->out) + 1) * sizeof(char));
	strcpy(Dstring, ptrC->out);
	strcpy(ptrC->out, "");
	switch(id){
		case 1:{
			strcpy(ptrA->in, Dstring);
		}
		case 2:{
			strcpy(ptrB->in, Dstring);
		}
		case 3:{
			strcpy(ptrC->in, Dstring);
		}
	}
  free(Dstring);
  up(semid, 2, 1);
  up(semid, 4, 1);
  up(semid, 6, 1);
  down(semid, 0, 3);
}
else{
  up(semid, 2, 1);
  up(semid, 4, 1);
  up(semid, 6, 1);
  down(semid, 0, 3);
}
/*Simulation continues and parent enables the children to
  to fill their out-buffer when they reach that point
  */
  (*flow)++;
  up(semid, 1, 2);
  up(semid, 3, 2);
  up(semid, 5, 2);
}

while(wait(NULL) != -1); //Wait all children to exit
printf("Simulation completed\n");
shmctl(shmtime, IPC_RMID, 0);
shmctl(shm_A, IPC_RMID, 0);
shmctl(shm_B, IPC_RMID, 0);
shmctl(shm_C, IPC_RMID, 0);
semctl(semid, 0, IPC_RMID);


return 0;
}
