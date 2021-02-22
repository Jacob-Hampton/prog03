//TODO Add comments, validate floats?
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//enums~ish for the query
static const char* END = "end";
static const char* X = "-x";
static const char* Y = "-y";
static const char* NAME = "-n";
static const char* HELP = "help";

//struct that holds the information of an image 
typedef struct pic{
    int longitude;      //longitude of where photo was taken
    int latititude;     //latitude of where photo was taken
    char* location;     //name of location where photo was taken
    int index;          //index of the pic inside an array
}pic;

//struct that holdls information of a location
typedef struct location{
    char* name;         //Location name shared by all pics
    pic* pictures;      //All pics that have that location name
    int numOfPics;      //total number of pics that have that location name
    int index;          //current index
}location;

//this function takes in the number of arguments from the command line
//then checks to make sure its a number of pictures and returns those pictures
int validateArgc(int argc){
    float argcF = argc;                         //argc as a float so arithmatic works
    float numOfPicsFloat = (argcF -1) / 3;      //accurate number of pictures
    int numOfPicsInt = (argcF -1) / 3;          //relative number of pictures
    //must be at least 3 arguments for there to be one picture
    if(argc < 4){                           
        printf("usage: ./prog03 <12 12 location>\n");
        exit(1);
    }//must be a multiple of 3 arguments not including the file
    else if((numOfPicsFloat - numOfPicsInt) != 0){
        printf("%d valid Picture(s) entered\n"
        "Each picture must have a longitude latitude and location.\n"
        "usage example: <234 -173 Boston>\n",numOfPicsInt);
        exit(1);
    }
    return numOfPicsInt;
}

//simplely takes in a charater varifies its an int then returns it as such
int isInt(char* number, int index){
    int valid = atoi(number);
    if (!isdigit(*number))
    {
        if(valid < 0){
            return valid;
        }
        printf("Longitude and latitude for picture number:%d must be a number\n",index+1);
        exit(1);
    }else{
        return valid;
    }
}

//prints a single formated pic struct
void printPicture(pic picture){
    printf("image %03d. location:  %4d, %4d. Name: %s\n",
       picture.index, picture.longitude, picture.latititude, picture.location);
}

//prints an array of pic structs formatted
void printPictures(pic* pictures, int numberOfPics){
   for (size_t i = 0; i < numberOfPics; i++)
   {
       printPicture(pictures[i]);
   } 
}

//creates a location struct that only holds a name and a count, then returns it
location createLocation(char* name){
    location location;
    location.name = (char*)malloc(strlen(name)*sizeof(char));
    location.name = name;
    location.numOfPics=1;
    return location;
}

//updates the total number of locations by returning a 1 based off if a new location was found
int updateNumberOfLocations(location* locationsTemp, int numberOfLocations, pic newPic){
    if(numberOfLocations == 0){
        locationsTemp[0] = createLocation(newPic.location);
        return 1;
    }
    for (unsigned i = 0; i < numberOfLocations; i++)
    {
        if(strcmp(locationsTemp[i].name,newPic.location)==0){
            locationsTemp[i].numOfPics++;
            return 0;
        }
    }
    locationsTemp[numberOfLocations] = createLocation(newPic.location);
    return 1;
}

//creates the accurate amount of memory for each of the location structs, then assigns the pic pointers to the correct pics
//returns the accuratly assigned array of locations
location* finalizeLocations(location* locationTemp,int numberOfLocations,pic* pictures, int numberOfPics){
    for(unsigned i =0;i<numberOfLocations;i++){
        locationTemp[i].pictures = (pic* )malloc(locationTemp[i].numOfPics * sizeof(struct pic));
        locationTemp[i].index=0;
    }
    for (unsigned i = 0; i < numberOfPics; i++)
    {
        for (unsigned j = 0; j < numberOfLocations; j++)
        {
            if (strcmp(pictures[i].location,locationTemp[j].name)==0)
            {
                locationTemp[j].pictures[locationTemp[j].index]= pictures[i];
                locationTemp[j].index++;
            }
        }
    }
    return locationTemp;
}

//creates a pic struct with all of the fields assigned
pic createPic(char* longitude,char* latitude,char* location,int index){
    pic picture;
    int len =  strlen(location);
    picture.longitude = isInt(longitude,index);
    picture.latititude= isInt(latitude,index);
    picture.index = index;
    picture.location = (char* ) malloc(len* sizeof(char));
    picture.location = location;
    return picture;
}

//performs a query for the user to search thier photos
void query(pic* pictures,int numberOfPics, location* finalLocations, int numberOfLocations){
    printf("Enter a command or 'help' for a list of commands\n");
    while(1){
        char input[4];                              //command
        int data= 0;                                //searched latitude/longitude
        char name[80];                              //searched location name 
        scanf("%s",input);
        if(strcmp(input,Y)==0 || strcmp(input,X)==0 ){
            scanf("%d",&data);  
            for(int i=0;i<numberOfPics;i++){
                if (strcmp(input,Y)==0 && data == pictures[i].latititude){
                    printPicture(pictures[i]);
                }
                if (strcmp(input,X)==0 && data == pictures[i].longitude){
                    printPicture(pictures[i]);
                }
            }
        }else if(strcmp(input,NAME)==0){
            scanf("%s",name);
            for (unsigned i = 0; i < numberOfLocations; i++){
                if(strcmp(finalLocations[i].name,name)==0){
                    printPictures(finalLocations[i].pictures,finalLocations->numOfPics);
                }
            }           
        }else if(strcmp(input,HELP)==0){
            printf("Command options are:\n"
            "\t-y <number>\n"
            "\t-x <number>\n"
            "\t-n <name>\n"
            "\tend\n");
        }else if (strcmp(input,END)==0){
           exit(0);
        }else{
            printf("Enter a command or 'help' for a list of commands\n");
        }
    }
}


int main(int argc, char* argv[]){
    int numberOfPics = validateArgc(argc);                  //number of photos entered
    //array of pics
    struct pic* pictures = (pic* )malloc(numberOfPics * sizeof(struct pic));
    //array of locations
    struct location* locationsTemp = (location*)malloc(numberOfPics * sizeof(struct location));
    int argIndex = 0, numberOfLocations =0;                 //current photo index, counter of locations
    for (unsigned i = 0; i < numberOfPics; i++){
        int argIndex = (3*i) +1;                            //calculate which is the first argument of a photo
        pictures[i] = createPic(argv[argIndex],argv[argIndex+1],argv[argIndex+2],i);
        numberOfLocations += updateNumberOfLocations(locationsTemp,numberOfLocations,pictures[i]);
    }
    //create a new array of locations to be properly sized
    location* finalLocations = (location*) malloc(numberOfLocations * sizeof(struct location));
    finalLocations = finalizeLocations(locationsTemp,numberOfLocations,pictures,numberOfPics);
    printPictures(pictures,numberOfPics);                   //display photos to user
    query(pictures,numberOfPics,finalLocations,numberOfLocations);
    return 0;
}