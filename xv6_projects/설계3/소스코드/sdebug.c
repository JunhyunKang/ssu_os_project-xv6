#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5
#define PRINT_CYCLE 10000000
#define TOTAL_COUNTER 500000000

void sdebug_func(void)
{
	int n, pid;

	printf(1,"start sdebug command\n");


	for(n=0; n<PNUM; n++){
		uint counter = 0;
		pid = fork();
		uint stime = uptime();//프로세스 생성 시간
		if(pid < 0)
			break;
		if(pid == 0){ //자식 프로세스
			weightset(n+1); //weight 1부터 +1씩 하면서 지정
			int ppid = getpid();//자식 프로세스의 pid를 얻는 시스템콜
			while(1){
				counter++; //clock tick마다 1씩 증가
				if(counter == TOTAL_COUNTER){ // 이경우 출력하고, 프로세스 종료
					printf(1,"PID: %d terminated\n", ppid);
					exit();
				//	break;
				}
				if(counter == PRINT_CYCLE){ //천만이 되면 정보 출력
					uint etime = uptime();
					uint t = (etime - stime)*10; //tick 차이 ms로 만들기
					printf(1,"PID: %d, WEIGHT: %d, TIMES : %d ms\n", ppid, n+1, t);
				}

			}
		}

			
	}

//생성된 자식프로세스들이 모두 끝날 때까지 기다림
	for(; n > 0; n--){
		if(wait() < 0){
		printf(1, "wait stopped early\n");
		exit();
		}
	}

	if(wait() != -1){
		printf(1, "wait got too many\n");
		exit();
	}

	printf(1,"end of sdebug command\n");

	return;
}

int main(void)
{
	sdebug_func();
	exit();
}
