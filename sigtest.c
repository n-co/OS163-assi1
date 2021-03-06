#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"

void
sig_handler_1(int signum){
  printf(1, "this is signal handler #1. signum = %d, pid = %d\n", signum, getpid());
}

void
sig_handler_2(int signum){
  printf(1, "this is signal handler #2. signum = %d, pid = %d\n", signum, getpid());
}

void
sig_handler_3(int signum){
  printf(1, "this is signal handler #3. signum = %d, pid = %d\n", signum, getpid());
}

void
sig_handler_4(int signum){
  printf(1, " TEST 4: testing signal BEFORE change.. \n");
  //printf(1, "before change signal \n");

}

void
sig_handler_5(int signum){
  //printf(1, "after change signal \n");
  printf(1, "********* TEST 4 PASSED - signal changed *********\n");

}

void
sig_handler_6(int signum){
  //printf(1, "after change signal \n");
  printf(1, "********* TEST 5 PASSED - child get signal *********\n");

}

void
test_one_fork(){
  int parentpid = getpid();
  int pid;
  signal(13,sig_handler_3);
  pid = fork();

if (pid < 0) {
  printf(1, "fork failed\n");
  exit(0);
}

  if(pid==0){
  	//child
    int childpid = getpid();
  	printf(1, "CHILD START: %d (fork=%d)    (parent pid %d)\n",childpid, pid, parentpid);
  	sigsend(parentpid, 13);
  	printf(1, "CHILD END:   %d (fork=%d)    (parent pid %d)\n",childpid, pid, parentpid);
  	exit(0);
  }
  else{
  	//parent
  	printf(1, "PARENT START: %d (fork=%d)\n", parentpid, pid);
  	wait(0);
  	
  	printf(1, "PARENT END:   %d (fork=%d)\n", parentpid, pid);
  	printf(1, "********* TEST 1 PASSED *********\n");
  }


}

void
test_wrong_signal_number(){

  sighandler_t sig_error=signal(300,sig_handler_3);

  if((int)(sig_error) == -1)  
  	printf(1, "********* TEST 2 PASSED *********\n");
  else
  	printf(1, "######### TEST 2 FAILED #########\n");

}


void
test_wrong_pid(){
//int parentpid=getpid();
  int pid;
  signal(1,sig_handler_4);
  pid = fork();
  if(pid==0){
  	//child
    int sig_status=sigsend(999999, 1);
  	if (sig_status==-1)
  		printf(1, "********* TEST 3 PASSED *********\n");
  	else
  		printf(1, "######### TEST 3 FAILED #########\n");

  	exit(0);
  }
  else{
  	//parent
  	wait(0);
  }
}

void
test_change_signal(){
  int parentpid=getpid();
  int pid,pid2;
  signal(5,sig_handler_4);
  pid = fork();
  if(pid==0){
  	//child
    sigsend(parentpid, 5);

  	exit(0);
  }
  else{
  	//parent
  	wait(0);
  	sighandler_t test_change=signal(5,sig_handler_5);
  	if (test_change!=sig_handler_4){
  		printf(1, "######### TEST 4 FAILED #########\n");
      exit(0);
  	}
  	pid2=fork();
  	if(pid2==0){
  		sigsend(parentpid, 5);
  		exit(0);
  	}
  	else 
  		wait(0);

  }
  
}

void
test_child_signal(){
  int c1,c2;
  c1 = fork();
  if(c1==0){
    //child 1
    c1 = getpid();
    c2 = fork();
    signal(25,sig_handler_6);
    if(c2==0){
      sigsend(c1,25);
      exit(0);
    }
    else{
      wait(0);
      exit(0);
    }
  }
  else
    wait(0);
}

int
main(void){
  printf(1, "signal test - start\n");
	  test_one_fork();
	  test_wrong_signal_number();
	  test_wrong_pid();
    test_change_signal();
    test_child_signal();
  printf(1, "signal test - end\n");
  exit(0);
}