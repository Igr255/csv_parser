#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>

sem_t *sem;
sem_t *waitForChildren;
sem_t *santaSleep;
sem_t *waitingElves;
sem_t *HelpedElves;
sem_t *reindeerWaiting;
sem_t *reindeerArrived;
sem_t *reindeerHitched;
sem_t *elvesGotHelp;
int *outputCounter = 0;
int *elfCount = 0;
int *reinDeerCount = 0;
bool *workshopClosed = false;
bool *wokeByReindeer = false;

bool init() {

    bool initFail = false;

    if ((sem= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((waitForChildren= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((santaSleep= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((waitingElves= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((HelpedElves= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((reindeerWaiting= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((reindeerArrived= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((reindeerHitched= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;
    if ((elvesGotHelp= mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
        initFail = true;

    if (!initFail) {
        if (sem_init(sem, 1, 1) == -1)
            initFail = true;
        if (sem_init(santaSleep, 1, 0) == -1)
            initFail = true;
        if (sem_init(waitingElves, 1, 3) == -1)
            initFail = true;
        if (sem_init(HelpedElves, 1, 0) == -1)
            initFail = true;
        if (sem_init(reindeerWaiting, 1, 0) == -1)
            initFail = true;
        if (sem_init(reindeerArrived, 1, 0) == -1)
            initFail = true;
        if (sem_init(waitForChildren, 1, 0) == -1)
            initFail = true;
        if (sem_init(reindeerHitched, 1, 0) == -1)
            initFail = true;
        if (sem_init(elvesGotHelp, 1, 0) == -1)
            initFail = true;
    }

    if (!initFail) {
        if ((outputCounter= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
            initFail = true;
        if ((elfCount= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
            initFail = true;
        if ((reinDeerCount= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
            initFail = true;
        if ((workshopClosed= mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
            initFail = true;
        if ((wokeByReindeer= mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED)
            initFail = true;
    }

    return initFail;
}

bool destroy(FILE *file) {

    bool delFail = false;

    if (sem_destroy(sem) == -1)
        delFail = true;
    if (sem_destroy(santaSleep) == -1)
        delFail = true;
    if (sem_destroy(waitingElves) == -1)
        delFail = true;
    if (sem_destroy(HelpedElves) == -1)
        delFail = true;
    if (sem_destroy(reindeerWaiting) == -1)
        delFail = true;
    if (sem_destroy(reindeerArrived) == -1)
        delFail = true;
    if (sem_destroy(waitForChildren) == -1)
        delFail = true;
    if (sem_destroy(reindeerHitched) == -1)
        delFail = true;
    if (sem_destroy(elvesGotHelp) == -1)
        delFail = true;

    munmap(sem,sizeof(sem_t));
    munmap(waitForChildren,sizeof(sem_t));
    munmap(santaSleep,sizeof(sem_t));
    munmap(waitingElves,sizeof(sem_t));
    munmap(HelpedElves,sizeof(sem_t));
    munmap(reindeerWaiting,sizeof(sem_t));
    munmap(reindeerArrived,sizeof(sem_t));
    munmap(reindeerHitched,sizeof(sem_t));
    munmap(elvesGotHelp,sizeof(sem_t));
    munmap(outputCounter,sizeof(int));
    munmap(elfCount,sizeof(int));
    munmap(reinDeerCount,sizeof(int));
    munmap(workshopClosed,sizeof(bool));
    munmap(wokeByReindeer,sizeof(bool));

    fclose(file);
    return delFail;
}


void elfFunction(FILE *file, int i, int TE, int pid) {
    srand(time(NULL) * pid * i);

    sem_wait(sem);
    fprintf(file, "%i: Elf %i: started\n", ++(*outputCounter), i + 1);
    fflush(file);
    sem_post(sem);

    while (!(*workshopClosed)) {

        if (TE != 0)
            usleep((rand() % (TE + 1)) * 1000);

        sem_wait(sem);
        fprintf(file, "%i: Elf %i: need help\n", ++(*outputCounter), i + 1);
        fflush(file);
        sem_post(sem);

        sem_wait(waitingElves); // eleves waiting (max 3)
        (*elfCount)++;

        if ((*elfCount) == 3) {
            if (!(*workshopClosed)) {
                sem_post(santaSleep); // calling santa when the row is full
            }
        }

        sem_wait(HelpedElves); // waiting for santa

        if (!(*workshopClosed)) {
            sem_wait(sem);
            fprintf(file, "%i: Elf %i: get help\n", ++(*outputCounter), i + 1);
            fflush(file);
            sem_post(sem);
        }

        (*elfCount)--; // helping elves

        if ((*elfCount) <= 0) { // if there are no elves left call santa to let more of them in
            *elfCount = 0;
            sem_post(elvesGotHelp);
        }
    }

    // elves going on holidays after workshop is closed
    sem_wait(sem);
    fprintf(file, "%i: Elf %i: taking holidays\n", ++(*outputCounter), i + 1);
    fflush(file);
    sem_post(sem);
}

void reindeerFunction(FILE *file, int i, int TR, int pid, int NR) {
    srand(time(NULL) * pid * i);
    usleep(rand()%100);

    sem_wait(sem);
    fprintf(file, "%i: RD %i: rstarted\n",++(*outputCounter), i + 1);
    fflush(file);
    sem_post(sem);

    if (TR != 0) // random work time
        usleep((rand() % (TR + 1)) * 1000);

    sem_wait(sem);
    fprintf(file, "%i: RD %i: return home\n", ++(*outputCounter), i + 1);
    fflush(file);
    sem_post(sem);

    (*reinDeerCount)++;

    if (*reinDeerCount == NR) { // call santa when last reindeer arrives
        (*wokeByReindeer) = true;
        sem_wait(reindeerArrived); // waiting for santa to finish his task with elves
        sem_post(santaSleep); // calling santa to hitch reindeer
    }
    sem_wait(reindeerWaiting); // reindeer waiting to get hitched

    sem_wait(sem);
    fprintf(file, "%i: RD %i: get hitched\n", ++(*outputCounter), i + 1);
    fflush(file);
    sem_post(sem);

    (*reinDeerCount)--;

    if (*reinDeerCount <= 0) {
        sem_post(reindeerHitched); // santa hitched all the reindeer
    }

}

void santaFunction(FILE *file, int NR, int NE) {
    while (!(*workshopClosed)) {

        // santa being lazy
        sem_wait(sem);
        fprintf(file, "%i: Santa: going to sleep\n", ++(*outputCounter));
        fflush(file);
        sem_post(sem);

        sem_post(reindeerArrived);
        sem_wait(santaSleep);

        // closes workshop after being woken up by last reindeer
        if ((*wokeByReindeer)) {
            sem_wait(sem);
            fprintf(file, "%i: Santa: closing workshop\n", ++(*outputCounter));
            fflush(file);
            sem_post(sem);
            *workshopClosed = true;

            // in case there are som elves stuck in the warehouse
            for (int i = 0; i < NE; i++) {
                sem_post(HelpedElves);
                sem_post(waitingElves);
            }
            break;
        }
        else {
            sem_wait(sem);
            fprintf(file, "%i: Santa: helping elves\n", ++(*outputCounter));
            fflush(file);
            sem_post(sem);

            sem_post(HelpedElves);
            sem_post(HelpedElves);
            sem_post(HelpedElves);

            sem_wait(elvesGotHelp);

            sem_post(waitingElves);
            sem_post(waitingElves);
            sem_post(waitingElves);
        }
    }

    for(int i = 0; i < NR; i++) {
        sem_post(reindeerWaiting);
    }

    sem_wait(reindeerHitched);
    sem_wait(sem);
    fprintf(file, "%i: Santa: Christmas started\n", ++(*outputCounter));
    fflush(file);
    sem_post(sem);

    for (int j = 0; j < NE + NR; j++) {
        sem_wait(waitForChildren);
    }
}


int main(int argc, char **argv){

    int NE,NR,TE,TR;
    if (argc == 5) {
        NE = atoi(argv[1]);
        NR = atoi(argv[2]);
        TE = atoi(argv[3]);
        TR = atoi(argv[4]);

        if ((NE <= 0 || NE >= 1000) || (NR <= 0 || NR >= 20) || (TE < 0 || TE > 1000) || (TR < 0 || TR > 1000)){
            fprintf(stderr, "invalid arguments values\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    int eid;
    int pid;

    bool err = init();

    if (err) {
        fprintf(stderr, "Initialization has failed\n");
        exit(1);
    }

    FILE *file;

    file = fopen("proj2.out", "w");
    setbuf(file,NULL);

    pid = fork();
    if (pid == 0) {
        for (int i = 0; i < NE; i++) {
            eid = fork();
            if (eid == 0) {
                elfFunction(file,i,TE,getpid());
                sem_post(waitForChildren);
                exit(0);
            }
        }

        for (int i = 0; i < NR; i++) {
            eid = fork();
            if (eid == 0) {
                reindeerFunction(file,i,TR,getpid(), NR);
                sem_post(waitForChildren);
                exit(0);
            }
        }

    } else {
        santaFunction(file, NR, NE);
        err = destroy(file);
        if(err) {
            fprintf(stderr, "De-allocation of memory failed\n");
            exit(1);
        }
        return 0;
    }
    return 0;
}
