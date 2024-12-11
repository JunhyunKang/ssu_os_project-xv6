#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];
char* _gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  i--;
  buf[i] = '\0';
  return buf;
}

int tokenizing(char* buf, int t, char* target){
	int i;
	int j=0;

	for(i=t; i<1024;i++ ){
		target[j++]=buf[i];

		if(buf[i]==' '||buf[i]=='\n'||buf[i]=='\0')
			break;
	}

	j--;
	//printf(1,"===================t %d", i);
	target[j]='\0';

	return ++i;
}

void get_user_list(){
	int fd, i, t=0;
	int c;
	char buf[1024];

	fd=open("list.txt", O_RDONLY);



	if((c=read(fd, buf, sizeof(buf)))<0)
	{
		printf(1,"read list.txt error\n");
		exit();
	}

	if(c==0)
		return;

	
	for(i=0;i<10;i++){
		// list.txt 에서 Username, Password 정보를 userID, pwdID에 저장
	
		//memset(userID[i],0, 32);
		//memset(pwdID[i],0, 32);
		t=tokenizing(buf,t, userID[i]);
		t=tokenizing(buf,t, pwdID[i]);
		
	//	printf(1,"i:   user %s    pwd %s\n", userID[i], pwdID[i]);
	}

}

int check_idpw(){
	//입력받은 Username, Password와 list.txt 비교
	int i;
	char username[32];
	char password[32];

	printf(1,"Username: ");
	_gets(username, sizeof(username));

	printf(1,"Password: ");
	_gets(password, sizeof(password));


//	printf(1,"check u: %s, p: %s \n", username, password);
	for(i=0;i<10;i++){
		if(!strcmp(username, userID[i]))
			if(!strcmp(password, pwdID[i])){
				return 1;
			}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int pid, wpid;

	

	get_user_list();
	

	while(1){	
		if(check_idpw()){

			for(;;){
				printf(1, "ssu_login: starting login\n");
				pid = fork();
				if(pid < 0){
					printf(1, "ssu_login: fork failed\n");
					exit();
				}
				if(pid == 0){
					exec("sh", argv);
					printf(1,"ssu_login: exec sh failed\n");  
					exit();
				}
				while((wpid=wait()) >= 0 && wpid != pid)
					printf(1, "zombie!\n");
			}
			exit();
		}
	}
	
	exit();
}
