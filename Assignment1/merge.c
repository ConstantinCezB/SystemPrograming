#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void print(char* message);
void saveNumber (int *numStorage, char c);
bool isDigitOrChar(char c);

int main (int argc, char *argv[]) {

    if(argc != 2){
        print("Provide the name of the two files needed.");
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY, 0644);
    // int fd2 = open(argv[1], O_RDONLY, 0644);
    // int fd_write = open(argv[1], O_RDWR, 0644);
    

    
    if(fd1 == -1){
        print("Cannot create or open file");
        return 1;
    }
    

    char pic1Format[10], pic2Format[10];
    int pic1_x = 0, pic1_y = 0, pic1_scale = 0, pic2_x = 0, pic2_y = 0, pic2_scale = 0;




    int stage = 0;
    char buf[1];
    int indexFormat = 0;
    bool reading = true, pixelReadingStage = true;


    while (read(fd1, buf, 1) > 0){
        
        if(stage == 3)
        {
            break;
        }

        if(buf[0] == '\n')
        {
            if(stage == 0) pic1Format[indexFormat] = '\0';
            if(reading)stage++;
            reading = true; 
        }  
        
        if(buf[0] == '#')
        {
            reading = false;
        }

        if(reading && (isDigitOrChar(buf[0])))
        {
            if(stage == 0 )
            {
                pic1Format[indexFormat++] = buf[0];
            }
            else if(stage == 1)
            {
              
                if(buf[0] == ' ')
                {
                    
                    pixelReadingStage = false;
                }
                if((buf[0] >= '0' && buf[0] <= '9')){
                    if(pixelReadingStage)
                    {
                        saveNumber(&pic1_x, buf[0]);
                    }
                    else
                    {
                        printf("int '%c'\n", buf[0]);
                        saveNumber(&pic1_y, buf[0]);
                    }
                }

            }
            else if(stage == 2)
            {
                saveNumber(&pic1_scale, buf[0]);
                
            }

        }
        

    }

    
    printf("format: %s,  x: %d,  y: %d,  scale: %d\n\n\n", pic1Format, pic1_x, pic1_y, pic1_scale);

    close(fd1);
    return 0;
}

void print(char* message){
    write(STDOUT_FILENO, message, strlen(message));
}

void saveNumber (int *numStorage, char c) {
    *numStorage = (*numStorage * 10) + (c - '0');
}

bool isDigitOrChar(char c){
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}
