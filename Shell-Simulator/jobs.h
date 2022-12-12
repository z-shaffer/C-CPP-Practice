
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */
#define MAXLINE    1024   /* max line size */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* The job struct */
typedef struct 
{              
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
} job_t;

extern job_t jobs[MAXJOBS]; 

void clearjob(job_t *job);
void initjobs(job_t *jobs);
int maxjid(job_t *jobs); 
int addjob(job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(job_t *jobs, pid_t pid); 
pid_t fgpid(job_t *jobs);
job_t *getjobpid(job_t *jobs, pid_t pid);
job_t *getjobjid(job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(job_t *jobs);

