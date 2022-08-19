#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

typedef struct _process_info {
    int pid;
    char ppid[10];
    char name[50];
    char VmSize[20];
}process_info;

char * get_ps_location(int pid, char * str) {
    sprintf(str, "/proc/%d/status", pid);
    return str;
}

void ps_view(void) {
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
    process_info p;

    proc_dp = opendir(PROC_ADDRESS);
    while((proc_dirp = readdir(proc_dp)) != NULL) {
	pid = atoi(proc_dirp -> d_name); //get pid 
	if(pid == 0)
	    continue;
	else {
	    p.pid = pid; 
	    get_ps_location(pid, proc_location);
	    status = fopen(proc_location, "r");
	    while(fgets(buffer, BUFSIZ, status) != NULL) { //print All status file...
	        line++;
		temp = strtok(buffer, "\t");
		while(temp != NULL) {
		    if(data_vaild == true) {
			temp = strtok(temp, "\n");
		        if(count == 1)
			    strcpy(p.name, temp);
			else if(count == 2)
			    strcpy(p.ppid, temp);
		        else if(count == 3)
			    strcpy(p.VmSize, temp);	
			data_vaild = false;
		    }	
		    else if(strstr(temp, "Name")) {
			count = 1;
			data_vaild = true;
		    }
		    else if(strstr(temp, "PPid")) {
			data_vaild = true;
			count = 2;
		    }
		    else if(strstr(temp, "VmSize")) {
			data_vaild = true;
			count = 3;
		    }
		    temp = strtok(NULL, "\t");
		}
	    }
	    printf("%4d %8s %8s %8s\n", p.pid, p.name, p.ppid, p.VmSize);
	    count = 0;
	    line = 0;
	    fclose(status);
	}
    }
    close(proc_dp);
}

int main(void) {
    //ps_view();
    while(1) { 
        ps_view();
	sleep(1);
        system("clear");
    }	
    return 0;
}
