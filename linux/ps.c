#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

char * get_ps_location(int num, char * str) {
    sprintf(str, "/proc/%d/status", num);
    return str;
}

int main(void) {
    FILE * status; 
    DIR * proc_dp;
    struct dirent * proc_dirp;
    char * PROC_ADDRESS = "/proc";
    int get_proc_number = 0;
    char proc_location[50];
    char buffer[BUFSIZ]; 
    int line = 0;

    proc_dp = opendir(PROC_ADDRESS);
    while((proc_dirp = readdir(proc_dp)) != NULL) {
	get_proc_number = atoi(proc_dirp -> d_name); //get All proc folders only number
	if(get_proc_number == 0)
	    continue;
	else {
	    get_ps_location(get_proc_number, proc_location);
	    status = fopen(proc_location, "r");
	    while(fgets(buffer, BUFSIZ, status) != NULL) { //print All status file...
	        line++;
		printf("%s", buffer);
	    }
	    line = 0;
	}
    }
    close(proc_dp);
    return 0;
}
