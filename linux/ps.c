#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

char * get_ps_location(int pid, char * str) {
    sprintf(str, "/proc/%d/status", pid);
    return str;
}

int main(void) {
    FILE * status; 
    DIR * proc_dp;
    struct dirent * proc_dirp;
    char * PROC_ADDRESS = "/proc";
    int pid = 0;
    char proc_location[50];
    char buffer[BUFSIZ]; 
    int line = 0;
    char * temp;
    bool data_vaild = false;
    int count = 0;

    proc_dp = opendir(PROC_ADDRESS);
    while((proc_dirp = readdir(proc_dp)) != NULL) {
	pid = atoi(proc_dirp -> d_name); //get pid 
	if(pid == 0)
	    continue;
	else {
	    printf("==========================\n");
	    get_ps_location(pid, proc_location);
	    status = fopen(proc_location, "r");
	    while(fgets(buffer, BUFSIZ, status) != NULL) { //print All status file...
	        line++;
		temp = strtok(buffer, "\t");
		while(temp != NULL) {
		    if(data_vaild == true) {
		        printf("%8s ", temp);
			data_vaild = false;
		    }	
		    else if(strstr(temp, "Name")) {
		        printf("Name ");	
			data_vaild = true;
		    }
		    else if(strstr(temp, "Pid") && count == 0) {
			printf("Pid ");
			data_vaild = true;
			count += 1;
		    }
		    else if(strstr(temp, "PPid")) {
			printf("PPid ");
			data_vaild = true;
		    }
		    else if(strstr(temp, "VmSize")) {
			printf("VmSize ");
			data_vaild = true;
		    }
		    else if(strstr(temp, "Threads")) {
			printf("Threads ");
			data_vaild = true;
		    }
		    temp = strtok(NULL, "\t");
		}
	    }
	    count = 0;
	    line = 0;
	}
    }
    close(proc_dp);
    return 0;
}
