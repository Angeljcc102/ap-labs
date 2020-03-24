#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"
#define SIZE 300000

void analizeLog(char *logFile, char *report);
int numSize(int n);

struct package{
    char name[100];
    char install_date[17];
    char last_update[17];
    int updates;
    char removal_date[17];
};

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int package_report, pacman_file;
    ssize_t bytes;

    char text[SIZE];
    int  j, space, inst_num_packages,rem_num_packages,up_num_packages = 0;
    struct package packages_list[4000];
    struct package tempo;

    pacman_file = open(logFile, O_RDONLY);

    if(pacman_file == -1){
        printf("Failed to open and read the file\n");
        exit(1);
    }

    bytes = read(pacman_file, text, SIZE);

    close(pacman_file);

    for(int i = 0; i < bytes ; i++){
        struct package aux;
        if(text[i] == '\n'){
            while(j < i){
                if(text[j] == '[' && space == 0){
                    j++;
                    int k = 0;
                    while(text[j] != ']'){
                        aux.install_date[k] = text[j];
                        k++;
                        j++;
                    }
                }
                if(text[j] == ' '){
                    space++;
                }
                if(space == 2){
                    int k = 0;
                    j++;
                    if(text[j] == 'i' && text[j+2] == 's' && text[j+5] == 'l' && text[j+8] == 'd'){
                        j = j+10;
                        while(text[j] != '('){
                            aux.name[k] = text[j];
                            k++;
                            j++;
                        }
                        aux.last_update[0] = '-';
                        aux.removal_date[0] = '-';
                        packages_list[inst_num_packages] = aux;
                        space++;
                        inst_num_packages++;
                        j = i;               
                    }
                    else if(text[j] == 'r' && text[j+2] == 'm' && text[j+4] == 'v' && text[j+6] == 'd'){
                        j = j+8;
                        rem_num_packages++;
                        while(text[j] != '('){
                            aux.name[k] = text[j];
                            k++;
                            j++;
                        }
                        for(int l = 0; l < inst_num_packages; l++){
                            if(strcmp(aux.name, packages_list[l].name) == 0){
                                strcpy(packages_list[l].removal_date, aux.install_date);
                                l = inst_num_packages;
                            }
                        }  
                        j = i;        
                    }
                    else if(text[j] == 'u' && text[j+2] == 'g' && text[j+5] == 'd' && text[j+7] == 'd'){
                        j = j+9;
                        up_num_packages++;
                        while(text[j] != '('){
                            aux.name[k] = text[j];
                            k++;
                            j++;
                        }
                        for(int l = 0; l < inst_num_packages; l++){
                            if(strcmp(aux.name, packages_list[l].name) == 0){
                                strcpy(packages_list[l].last_update, aux.install_date);
                                packages_list[l].updates++;
                                l = inst_num_packages;
                            }
                        }
                        j = i;
                    }
                    else if(text[j] == 'r' && text[j+2] == 'i' && text[j+5] == 't' && text[j+10] == 'd'){
                        j = j+12;
                        while(text[j] != '('){
                            aux.name[k] = text[j];
                            k++;
                            j++;
                        }
                        for(int l = 0; l < inst_num_packages; l++){
                            if(strcmp(aux.name, packages_list[l].name)== 0){
                                strcpy(packages_list[l].install_date, aux.install_date);
                                packages_list[l].last_update[0] = '-';
                                packages_list[l].removal_date[0] = '-';
                                packages_list[l].updates = 0;
                                l = inst_num_packages;
                            }
                        }
                        j = i;
                    }
                    else{
                        j = i;
                    }
                }
                j++;
            }
            j = i;
            space = 0;
            aux = tempo;
        }
    }

    package_report = open(REPORT_FILE, O_CREAT | O_WRONLY, 0600);

    if(package_report == -1){
        printf("Failed to create and open the file\n");
        exit(1);
    }

    char char_inst_num [numSize(inst_num_packages)],
    char_rem_num [numSize(rem_num_packages)],
    char_up_num [numSize(up_num_packages)],
    char_current_num [numSize(inst_num_packages-rem_num_packages)];

    sprintf(char_inst_num, "%d", inst_num_packages);
    sprintf(char_rem_num, "%d", rem_num_packages);
    sprintf(char_up_num, "%d", up_num_packages);
    sprintf(char_current_num, "%d", inst_num_packages-rem_num_packages);

    write(package_report,"Pacman Packages Report\n", 23);
    write(package_report,"----------------------\n",23);

    write(package_report,"- Installed packages : ",23);
    write(package_report, char_inst_num, numSize(inst_num_packages));
    write(package_report, "\n", 1);

    write(package_report,"- Removed packages   : ",23);
    write(package_report, char_rem_num, numSize(rem_num_packages));
    write(package_report, "\n", 1);

    write(package_report,"- Upgraded packages  : ",23);
    write(package_report, char_up_num, numSize(up_num_packages));
    write(package_report, "\n", 1);

    write(package_report,"- Current installed  : ",23);
    write(package_report, char_current_num, numSize(inst_num_packages-rem_num_packages));
    write(package_report, "\n", 1);
    write(package_report, "\n", 1);

    write(package_report, "List of packages\n", 17);
    write(package_report, "----------------\n", 17);

    for(int i = 0; i < inst_num_packages; i ++){
        int j = 0;
        write(package_report, "- Package Name       : ", 23);
        while(packages_list[i].name[j] != '\0'){
            write(package_report, &packages_list[i].name[j] , 1);
            j++;
        }
        write(package_report, "\n", 1);

        write(package_report, " - Install date      : ", 23);
        write(package_report, packages_list[i].install_date, 16);
        write(package_report, "\n", 1);

        write(package_report, " - Last update date  : ",23);
        if(packages_list[i].last_update[0] == '-'){
            write(package_report, "-", 1);
        }
        else{
            write(package_report, packages_list[i].last_update, 16);
        }
        write(package_report, "\n", 1);

        write(package_report, " - How many updates  : ",23);
        char char_updates[numSize(packages_list[i].updates)];
        sprintf(char_updates, "%d", packages_list[i].updates);
        write(package_report, char_updates, numSize(packages_list[i].updates));
        write(package_report, "\n", 1);

        write(package_report, " - Removal date      : ",23);
        if(packages_list[i].removal_date[0] == '-'){
            write(package_report, "-", 1);
        }
        else{
            write(package_report, packages_list[i].removal_date, 16);
        }
        write(package_report, "\n", 1);

    }

    close(package_report);
}

int numSize(int n){
    int i = 1;
    while(n > 9){
        n /= 10;
        i++;
    }
    return i;
}
