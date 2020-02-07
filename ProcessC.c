#include "util.h"


int main (int argc, char *argv[]){
  struct sh_string *ptr;
  int shm_id, shmtime, semid;
  int *flow, localId, i, j, count = 0, SendFlag = 0, r, r1, r2, counter, endTime;
  FILE *doc, *logfile;
  char **ptr_ch, ch, *ptrStr, *localStr = NULL;
  size_t Length = NULL;
  srand (getpid() * time(NULL));

  logfile = fopen("LogC.txt", "w");
  shm_id = shmget(6789, sizeof(struct sh_string), 0600);
  shmtime = shmget(5678, sizeof(int*), 0600);
  semid = semget(4567, 7, 0600);
  ptr = shmat(shm_id, NULL, 0);
  flow = shmat(shmtime, NULL, 0);

  endTime = atoi(argv[1]);

  localId = 3;
  doc = fopen("doc.txt", "r");
  while (ch != EOF) { //scan doc to count lines
		ch = fgetc(doc);
		if (ch == '\n')
			count++;
	};
	fclose(doc);
  ptr_ch = malloc(count * sizeof(char *));
  doc = fopen("doc.txt", "r");
    i = 0;
    while ( i < count)
        getline(&ptr_ch[i++], &Length, doc); // save the sentence to ptr_ch[]
		fclose(doc);

    strcpy(ptr->in, "");
    strcpy(ptr->out, "");
    count = count - 1;
    while((*flow) < endTime){
      counter = 0;
      j = *flow;
      r2 = rand() % 3 + 1;
      r = rand() % 10;
      r1 = rand() % count;

      down(semid, 5, 1);
      if (r <= 3){
        SendFlag = 1;
        ptrStr = ptr->out;
        *ptrStr = r2;
        ptrStr++;
        strcpy(ptrStr, ptr_ch[r1]);
        fprintf(logfile, "Iteration %d Process C sent :\n %sinto id %d\n\n", j, ptrStr, ptr->out[0]);
      }
      up(semid, 0, 1);
      while(counter < 3){
      down(semid, 6, 1);
      if ((int)strlen(ptr->in) != 0){
  	     localStr = malloc(((int)strlen(ptr->in) + 1) * sizeof(char));
  	     strcpy(localStr, ptr->in);
         ptrStr = localStr;
         ptrStr++;
         fprintf(logfile, "Iteration %d Process C received :\n %s\n\n", j, ptrStr);
         strcpy(ptr->in, "");
  	     free(localStr);
      }
      up(semid, 0, 1);
      counter++;
      }
      down(semid, 5, 1);
    }

    fclose(logfile);
  for (i = 0; i < count; i++)
    free(ptr_ch[i]);
  free(ptr_ch);
exit(0);
}
