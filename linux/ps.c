#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

typedef struct _process_info {
    struct _process_info *prev;
    struct _process_info *next; 
    int pid;
    char ppid[10];
    char name[50];
    char VmSize[20];
}process_info;

process_info *head, *tail;

void init_ps_list(void) {
    head = (process_info*)malloc(sizeof(process_info));
    tail = (process_info*)malloc(sizeof(process_info));
    head -> next = tail;
    head -> prev = head;
    tail -> next = tail;
    tail -> prev = head;
}

void add_ps_list(process_info * p) {
    if(head -> next == tail) {
        p -> prev = head;
	p -> next = tail;
	head -> next = p;
	tail -> prev = p;
    }
    else {
        p -> next = tail;
	p -> prev = tail -> prev;
	tail -> prev -> next = p;
	tail -> prev = p;
    }
}

void print_all_list(process_info *p) {
    for(p; p != tail; p = p -> next) {
	if(!strstr(p -> VmSize, " "))
	    continue;
        printf("======================================\n");	
	printf("Name : %sPid : %d\nPPid : %sVmSize : %s", (*p).name, (*p).pid, (*p).ppid, (*p).VmSize); 
    }
    printf("======================================\n");	
}

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

    init_ps_list();
    proc_dp = opendir(PROC_ADDRESS);
    while((proc_dirp = readdir(proc_dp)) != NULL) {
	pid = atoi(proc_dirp -> d_name); //get pid 
	if(pid == 0)
	    continue;
	else {
	    process_info *p = (process_info*)malloc(sizeof(process_info));
	    p -> pid = pid; 
	    get_ps_location(pid, proc_location);
	    status = fopen(proc_location, "r");
	    while(fgets(buffer, BUFSIZ, status) != NULL) { //print All status file...
	        line++;
		temp = strtok(buffer, "\t");
		while(temp != NULL) {
		    if(data_vaild == true) {
		        if(count == 1)
			    strcpy(p -> name, temp);
		        else if(count == 2)
			    strcpy(p -> ppid, temp);
		        else if(count == 3)
			    strcpy(p -> VmSize, temp);	
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
	    add_ps_list(p);
	    count = 0;
	    line = 0;
	}
    }
    print_all_list(head -> next);
    close(proc_dp);
    return 0;
}
