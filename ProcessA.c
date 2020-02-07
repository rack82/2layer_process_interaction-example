#include "util.h"


int main (int argc, char *argv[]){
  struct sh_string *ptr;
  int shm_id, shmtime, semid, r, j, counter, endTime;
  int *flow, localId, i, count = 0,r1, r2, SendFlag = 0;
  char **ptr_ch, ch, *ptrStr, *localStr = NULL;
  FILE *doc, *logfile;
  size_t Length = 0;
  ssize_t nread;
  srand (getpid() * time(NULL));

  logfile = fopen("LogA.txt", "w");
  shm_id = shmget(1234, sizeof(struct sh_string), 0600);
  shmtime = shmget(5678, sizeof (int*), 0600);
  semid = semget(4567, 7, 0600);
  ptr = shmat(shm_id, NULL, 0);
  flow = shmat(shmtime, NULL, 0);

  endTime = atoi(argv[1]);

  localId = 1;
  doc = fopen("doc.txt", "r");
  if (!doc){
    fprintf(stderr, "could not open '%s' : %s", "doc.txt", strerror(errno));
    exit(1);
  }
  while (ch != EOF) { //scan doc to count lines
		ch = fgetc(doc);
		if (ch == '\n')
			count++;
	};

	fclose(doc);
  ptr_ch = malloc(count * sizeof(char *));

  doc = fopen("doc.txt", "r");
  if (!doc){
    fprintf(stderr, "could not open '%s' : %s", "doc.txt", strerror(errno));
    exit(1);
  }
    i = 0;
    while ( i < count)
        getline(&ptr_ch[i++], &Length, doc); // save the sentence to ptr_ch[]
    fclose(doc);

  count = count - 1;
  strcpy(ptr->in, "");
  strcpy(ptr->out, "");
  while((*flow) < endTime){
    counter = 0;
    j = *flow;
    r2 = rand() % 3 + 1;
    r = rand() % 10;
    r1 = rand() % count;

    down(semid, 1, 1);
    if (r <= 3){ // prob 30% to send something
      SendFlag = 1;
      ptrStr = ptr->out;
      *ptrStr = r2; // save send id in out[0] buffer
      ptrStr++;
      strcpy(ptrStr, ptr_ch[r1]);
      fprintf(logfile, "Iteration %d Process A sent :\n %sinto id %d\n\n", j, ptrStr, ptr->out[0]);
    }
    up(semid, 0, 1); // let Parent check and distribute strings to other processes
    /* Every child will check 3 times their in-buffer
      because it might receive at most 3 strings.
      */
    while(counter < 3){
    down(semid, 2, 1); // child waits for parent to distribute the strings
    if ((int)strlen(ptr->in) != 0){
	     localStr = malloc(((int)strlen(ptr->in) + 1) * sizeof(char));
	     strcpy(localStr, ptr->in);
       ptrStr = localStr;
       ptrStr++;
       fprintf(logfile, "Iteration %d Process A received :\n %s\n\n", j, ptrStr);
       strcpy(ptr->in, "");
	     free(localStr);
    }
    up(semid, 0, 1);
    counter++;
    }
    down(semid, 1, 1); //wait for parent to increase flow
  }

  fclose(logfile);
  for (i = 0; i < count; i++)
    free(ptr_ch[i]);
  free(ptr_ch);

exit(0);
}
