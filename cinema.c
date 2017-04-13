#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Hall {

    char* hall_name;
    char* movie_name;
    int hall_width;
    int hall_height;
    char** seats;

    struct Hall* next;
};

FILE* file_open(char *);
FILE* file_output();
void command_definer(struct Hall*,FILE*,FILE *);
struct Hall* create_hall(struct Hall*, char*, char*, int, int);
void buy_ticket(struct Hall *,char *,FILE *);
void cancel_ticket(struct Hall *halls,char *command_name,FILE *);
void show_hall(struct Hall *,char *,FILE *);
void statistics(struct Hall *,char *,FILE *);
void array_clean(struct Hall *halls);

/* input open function */
FILE* file_open(char *argv){

    FILE *file = fopen(argv,"r");
    if ( file  == NULL){
        printf("input.txt not found!");
        return 0;
    }
    return file;
}
/* output function */
FILE* file_output(){

    FILE *output = fopen("output.txt","w");
    return output;
}
/*defines command*/
void command_definer(struct Hall* halls,FILE *file,FILE *output){


    char line[1024];
    char *command_name;

    while (fgets(line, sizeof(line), file)) {


        command_name=strtok(line," ");
        /* check command name and run function*/
        if(strcmp("CREATEHALL",command_name) == 0){
            char *hall_name = strtok(NULL," \n\"");
            char *movie_name = strtok(NULL," \n\"");
            int hall_width = atoi(strtok(NULL," \n\""));
            int hall_height = atoi(strtok(NULL," \n\""));

            halls = create_hall(halls, hall_name, movie_name, hall_width, hall_height);

        }
        else if(strcmp("BUYTICKET",command_name) == 0){

            buy_ticket(halls,command_name,output);
            printf("\n");
            fprintf(output,"\n");
        }
        else if(strcmp("CANCELTICKET",command_name) == 0){

            cancel_ticket(halls,command_name,output);
            printf("\n");
            fprintf(output,"\n");
        }
        else if(strcmp("SHOWHALL",command_name) == 0){

            show_hall(halls,command_name,output);
            printf("\n");
            fprintf(output,"\n");
        }
        else if(strcmp("STATISTICS",command_name) == 0){

            statistics(halls,command_name,output);
            printf("\n");
            fprintf(output,"\n");
        }
        else{
            printf("COMMAND IS NOT CORRECT!");
            printf("\n");
            fprintf(output,"\n");
        }


    }
    /* close input and output */
    fclose(file);
    fclose(output);

}
/* create hall function. it creates new hall */
struct Hall* create_hall(struct Hall* halls, char* hall_name, char* movie_name, int hall_width, int hall_height){

    /* create new hall */
    struct Hall* new_hall = (struct Hall*)malloc(sizeof(struct Hall));

    new_hall->next = halls;
    new_hall->hall_height = hall_height;
    new_hall->hall_width = hall_width;

    new_hall->hall_name = (char*)malloc(strlen(hall_name+1));
    new_hall->movie_name = (char*)malloc(strlen(movie_name)+1);

    new_hall->seats = (char**)malloc(hall_height * sizeof(char*));

    strcpy(new_hall->hall_name, hall_name);
    strcpy(new_hall->movie_name, movie_name);

    int x=0;
    int y=0;

    for(x = 0; x < hall_height; x++) {
        new_hall->seats[x] = (char*)malloc(hall_width * sizeof(char));
        for(y = 0; y < hall_width; y++) {

            new_hall->seats[x][y] = ' ';
        }
    }

    return new_hall;

}
/* buy ticket function */
void buy_ticket(struct Hall *halls,char *command_name,FILE *output){

    char *movie_name = strtok(NULL," \n\"");
    char *seat_label = strtok(NULL," \n\"");
    char *ticket_type = strtok(NULL," \n\"");
    int requested_seat = atoi(strtok(NULL," \n\""));

    int column = (seat_label[0] - 'A');
    int row = strtol(&seat_label[1], NULL, 10);
    int k;
    int control = 0;
    /* check movie name is empty or not */
    if(strcmp(seat_label,"Student") ==0 || strcmp(seat_label,"FullFare")== 0){
        printf("MOVIE NAME CANNOT BE EMPTY");
        fprintf(output,"MOVIE NAME CANNOT BE EMPTY");
    }
    else {
        while (halls) {

            if (strcmp(movie_name, halls->movie_name) == 0) {

                /* check seat is defined or not defined */
                if(row>halls->hall_height || column+requested_seat>halls->hall_width){
                    printf("ERROR: Seat %s is not defined at \"%s\"",seat_label,halls->hall_name);
                    fprintf(output,"ERROR: Seat %s is not defined at \"%s\"",seat_label,halls->hall_name);
                }
                else{
                    for(k=0;k<requested_seat;k++){
                        if(halls->seats[row-1][column+k] == 's' || halls->seats[row-1][column+k] == 'f' ){
                            control++;
                        }
                    }
                    /* fill seats */
                    if(control == 0){
                        printf("%s [%s] Seat(s) ",halls->hall_name,halls->movie_name);
                        fprintf(output,"%s [%s] Seat(s) ",halls->hall_name,halls->movie_name);
                        for(k=0;k<requested_seat;k++){
                            if(halls->seats[row-1][column+k] != 's' || halls->seats[row-1][column+k] != 'f' ){
                                if(strcmp(ticket_type,"Student") == 0){
                                    halls->seats[row-1][column+k] = 's';
                                }
                                else if(strcmp(ticket_type,"FullFare") == 0){
                                    halls->seats[row-1][column+k] = 'f';
                                }
                                printf("%c%d", 'A' + k + column, row);
                                fprintf(output,"%c%d", 'A' + k + column, row);
                                if(k<requested_seat-1){
                                    printf(",");
                                    fprintf(output,",");
                                }
                            }
                        }
                        printf(" have been sold");
                        fprintf(output," have been sold");
                    }
                    else{
                        printf("ERROR: Specified seat(s) in %s are not available! They have been already taken.",halls->hall_name);
                        fprintf(output,"ERROR: Specified seat(s) in %s are not available! They have been already taken.",halls->hall_name);
                    }

                }
                return;
            }


            halls = halls->next;
        }
        printf("ERROR: movie name \"%s\" is incorrect.",movie_name);
        fprintf(output,"ERROR: movie name \"%s\" is incorrect.",movie_name);
    }


}
/* cancel ticket function */
void cancel_ticket(struct Hall *halls,char *command_name,FILE *output){

    char *movie_name = strtok(NULL," \n\"");
    char *seat_label = strtok(NULL," \n\"");
    /* check movie name empty or not */
    if(seat_label==NULL){
        printf("ERROR: Movie name cannot be empty");
        fprintf(output,"ERROR: Movie name cannot be empty");
    }
    else{
        int column = (seat_label[0] - 'A');
        int row = strtol(&seat_label[1], NULL, 10);

        while(halls){
                /* cancel seat */
            if (strcmp(movie_name, halls->movie_name) == 0) {

                if(halls->seats[row-1][column] == 's' || halls->seats[row-1][column] == 'f'){
                    halls->seats[row-1][column] = ' ';
                    printf("%s [%s] purchase is cancelled. Seat %s is now free.",halls->hall_name,halls->movie_name,seat_label);
                    fprintf(output,"%s [%s] purchase is cancelled. Seat %s is now free.",halls->hall_name,halls->movie_name,seat_label);
                }
                else{

		    printf("ERROR: Seat %s",seat_label);
		    fprintf(output,"ERROR: Seat %s",seat_label);

                    printf(" in \"%s\" was not sold.",halls->hall_name);
                    fprintf(output," in \"%s\" was not sold.",halls->hall_name);
                }

                return;
            }
            halls = halls->next;
        }


    }

}
/* show hall function */
void show_hall(struct Hall *halls,char *command_name,FILE *output){


    int k,l;
    char *hall_name = strtok(NULL," \n\"");
    if(hall_name==NULL){
        printf("ERROR: Hall name cannot be empty");
        fprintf(output,"ERROR: Hall name cannot be empty");
    }
    else{
        while(halls) {
                /*print cinema hall with chars */
            if (strcmp(hall_name, halls->hall_name) == 0) {

                printf("%s sitting plan\n",hall_name);
                fprintf(output,"%s sitting plan\n",hall_name);
                for(k=0;k<halls->hall_height;k++){
                    printf("  ");
                    fprintf(output,"  ");
                    for(l=0; l<halls->hall_width; l++){
                        printf("--");
                        fprintf(output,"--");
                    }

                    printf("-\n%-2d",k+1);
                    fprintf(output,"-\n%-2d",k+1);

                    for(l=0; l<halls->hall_width; l++) {

                        printf("|%c", halls->seats[k][l]);
                        fprintf(output,"|%c", halls->seats[k][l]);
                    }
                    printf("|\n");
                    fprintf(output,"|\n");
                }
                printf("  ");
                fprintf(output,"  ");

                for(l=0; l<halls->hall_width; l++){
                    printf("--");
                    fprintf(output,"--");
                }
                printf("-\n  ");
                fprintf(output,"-\n  ");
                for(l=0; l<halls->hall_width; l++) {

                    printf(" %c", 'A' + l);
                    fprintf(output," %c", 'A' + l);
                }
                printf("\n");
                fprintf(output,"\n");

                for(k=0; k<halls->hall_width - 6; k++) {

                    printf(" ");
                    fprintf(output," ");
                }
                printf("C U R T A I N\n");
                fprintf(output,"C U R T A I N\n");


                return;
            }
            halls = halls->next;
        }
    }


}
/* show statistics function */
void statistics(struct Hall *halls,char *command_name,FILE *output){

    int k,l;

    printf("Statistics\n");
    fprintf(output,"Statistics\n");


    while(halls !=NULL){
        int student = 0;
        int fullFare = 0;

        /* calculate student and fullfare number and print statistic */
        for(k=0; k<halls->hall_height; k++){
            for(l=0; l<halls->hall_width; l++){
                if (halls->seats[k][l]=='s') {
                    student++;
                }
                if (halls->seats[k][l]=='f') {
                    fullFare++;
                }

            }
        }

        printf("%s %d student(s), %d full fare(s), sum:%d TL\n",halls->movie_name , student, fullFare, ((student*7) +(fullFare*10)));
        fprintf(output,"%s %d student(s), %d full fare(s), sum:%d TL\n",halls->movie_name , student, fullFare, ((student*7) +(fullFare*10)));
        halls = halls->next;
    }



}
/* array clean function. it cleans arrays*/
void array_clean(struct Hall *halls){

    struct Hall *test;
    test=halls;
    int i;
    /* clean mallocs */
    while(test){

        free(test->hall_name);
        free(test->movie_name);

        for(i=0; i<test->hall_height; i++)
            free(test->seats[i]);

        free(test->seats);
    }

}

int main(int argc,char *argv[]){


    FILE *file =NULL;
    FILE *output =NULL;
    file = file_open(argv[1]);
    output = file_output();
    struct Hall *halls=NULL;
    command_definer(halls,file,output);
    array_clean(halls);

    return 0;
}



