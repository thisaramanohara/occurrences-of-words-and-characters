#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


//structure for words
typedef struct __ {
    char toWord[100];
    unsigned long long int thisWordCount;
    struct __ * nextWord;
} word;

//structure for characters
typedef struct ___ {
    char ch;
    unsigned long long int thisChCount;
    struct ___ * nextCh;
} letter;


word * headWord = NULL; //head pointer of words linkedList

letter * headLetter = NULL; //head pointer of character linkedList

//function prototypes

void removeSymbols(char * line);    //To remove Symbols from the text
void toMakeLowerCase(char * line);  //to convert text to lower case   
void copyingWords(char * line);  //creating nodes for words
void filteringLetters(char * line);  //creating nodes for characters
void fillingBar(int fill);  //to draw a bar in the chart
void lastLine(int indent);  //to draw the last horizontal line in the chart
int maxWordLength(word * start , int iter);     //find the longest word in the limit
void movingCursor(int thisSize , int maxSize);  //to move the cursor
void empty(char * line);    //to clear a text

//for word sorting
word * combine_wordLists(word * part1, word * part2);   //to combine 2 lists
void divide_wordLinkedList(word * head, word** ptr_firstPart, word** ptr_secondPart);     //to divide the word linked list
void sortWords(word ** ptrHead);    //to sort words

//for letter sorting
letter * combine_charLists(letter * part1, letter * part2);     //to combine 2 lists
void divide_charLinkedList(letter * head, letter** ptr_firstPart, letter** ptr_secondPart);   //to divide the char linked list
void sortChars(letter** ptrHead);   //to sort chars


//to count all words
unsigned long long int wordCount = 0;

//to count all letters
unsigned long long int letterCount = 0;


int main( int argc , char **argv){
 
    char line[2000]={0};    //to hold a line of the text

    int wordMode = 0;   //By default word Mode option is ON
    int charMode = 0;   //By default character Mode option is OFF
    int scaled = 0;     //By default scaled option is OFF

    unsigned long long int filesSize = 0;    //to hold the size of files

    int j,i;    //for counting

    //options
    char * cFlag = "-c";
    char * wFlag = "-w";
    char * scaleFlag = "--scaled";
    char * lFlag = "-l";

    int iter = 10;  //By default limit is 10

    int numFiles = argc;    

    //when only programme name is typed
    if(numFiles==1){
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
    }

    FILE * file;       // the FILE pointer to access the files
    FILE * fileCheck;   //for checking the size of the files


    // Arguments checking
    for(i=1;i<argc;i++){

        //when Character Mode is applied
        if( !(strcmp(argv[i],cFlag)) ){
            charMode = 1;

        //when word Mode is applied
        }else if( !(strcmp(argv[i],wFlag)) ){
            wordMode = 1;
          
        //when scaled Mode is applied
        }else if( !(strcmp(argv[i],scaleFlag)) ){
            scaled = 1;
        
        //when limit argument is applied   
        }else if( !(strcmp(argv[i],lFlag)) ){


            //to check whether the other arguments are correct but only '-l' exists lastly
            if(!(strcmp(argv[argc-1],lFlag))){

                printf("Not enough options for [-l]\n");
                printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
            }

            //checking the next argument

            //when the next argument contains 1 character
            if(strlen(argv[i+1])==1){
               
                if( !(isdigit(argv[i+1][0])) ){     //if it's not a number
                    printf("Invalid options for [-l]\n");
                    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                    return 0;
                }else{
                    iter = atoi(argv[i+1]);     //get the integer character to the iter variable
                }        

            }else{      //if the next argument contains more than 1 character

                //to check whether there is a non integer character
                for(j=1;j<strlen(argv[i+1]);j++){
                    if( !(isdigit(argv[i+1][j])) ){
                        printf("Invalid options for [-l]\n");
                        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                        return 0;
                    }
                }    
                
                //if the next argument is a negative value
                if(argv[i+1][0]=='-'){
                    printf("Invalid option(negative) for [-l]\n");
                    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                    return 0;

                }

                //get the integer value to the iter variable
                else{
                    iter = atoi(argv[i+1]);
                }
                

            }
        }
        //if other argument is applied    
        else if(argv[i][0]=='-'){
            if( !(!(strcmp(argv[i],cFlag)) || !(strcmp(argv[i],scaleFlag)) || !(strcmp(argv[i],lFlag)) || !(strcmp(argv[i],wFlag))) ){
                printf("Invalid option [%s]\n",argv[i]);
                printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
                return 0;
                }
            
        }

    }    
            
    //if both word Mode and character Mode is ON
    if(charMode==1 && wordMode==1){
        printf("[-c] and [-w] cannot use together\n");
        printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
    }

    //reading files
    for(i=1;i<numFiles;i++){

            //if it finds a '-l' argument
            if(!(strcmp(argv[i],lFlag))){
                i++;   //to ignore the integer argument which presents after '-l' argument
                continue;   
            }

            //if it finds a '-' character
            else if(argv[i][0]=='-'){
                continue;
            }

            //opening file to read
            file = fopen(argv[i],"r");

            //open the same file to check the size of the file
            fileCheck = fopen(argv[i],"r");


            //if the file does not exist
            if(file == NULL){
                printf("Cannot open one or more given files\n");
                return 0;
            }

            //getting the size of the file
            fseek(fileCheck,0L,SEEK_END);
            filesSize += ftell(fileCheck);

            //if the selected file is empty
            if(ftell(fileCheck) == 0){
                continue;
            }
            
            //getting the content of the file line by line
            while(fgets(line,2000,file)!=NULL){
               
                removeSymbols(line);    //removing symbols of the line

                toMakeLowerCase(line);  //convert all characters in the line to lowercase

                //if character Mode is ON
                if(charMode==1){

                    filteringLetters(line);     //putting characters to a linked list

                }

                //if word Mode is ON
                else{
                
                    copyingWords(line);     //putting words to a linked list
               
                }
              
            }
            
        }

    //if size of the all files is zero
    if(filesSize==0){
        printf("No data to process\n");
        return 0;
    }

    int times = 0;  //for counting
    int fill;   //to find how many units are required to draw a bar of the chart

    if(charMode == 1){  //if chracter Mode is ON

        //check whether the limit is larger than the no. of total characters
        if(iter > letterCount){
            iter = letterCount;
        }

        //to sort the nodes
        sortChars(&headLetter); 
        
        //tempory pointer that points to the head of the linked list
        letter * ccc = headLetter;
      
        if(scaled==1){      //when scaled Mode is ON

            //if limit is zero no new line
            if(iter!=0){
                printf("\n");
            }
            
            unsigned long long int max;     //to find the maximum frequency
            float pre;  //percentage of the maximum frequency
            while(ccc !=NULL && times<iter){

                if(times==0){   //in the first iteration
                    max = ccc->thisChCount;
                    pre = (max*100) / (float)letterCount;
    
                }
                
                if(pre>=10 && pre<100){
                    fill = (70 * ccc->thisChCount) / max;
        
                }
                else if(pre<10){
                    fill = (71 * ccc->thisChCount) / max;
                }
                else if(pre>=100){
                    fill = (69 * ccc->thisChCount) / max;
                }

                //print the 1st line of the bar
                printf("   ");  printf("\u2502");   fillingBar(fill);   printf("\n");

                //print the 2nd line of the bar
                printf(" %c ",ccc->ch);   printf("\u2502");   fillingBar(fill);     printf("%.2f%%\n",(ccc->thisChCount)*100/(float)letterCount);
                
                //print the 3rd line of the bar
                printf("   ");  printf("\u2502");   fillingBar(fill);   printf("\n   \u2502\n");

                ccc = ccc->nextCh;  //go to the next node
                times++;

            }

            if(iter!=0){
                printf("   ");
                lastLine(3);    //to print the last line of the chart

            }
            
        }
            //if the scaled mode is OFF
        else{
         
            //if limit is zero no new line
            if(iter!=0){
                printf("\n");
            }

            unsigned long long int max;     //to find the maximum frequency
            float pre;  //percentage of the maximum frequency
            while(ccc !=NULL && times<iter ){

                if(times==0){   //in the first iteration
                    max = ccc->thisChCount;
                    pre = (max*100) / (float)letterCount;
    
                }
                
                if(pre>=10 && pre<100){
                    fill = (70 * ccc->thisChCount) / letterCount;
        
                }
                else if(pre<10){
                    fill = (71 * ccc->thisChCount) / letterCount;
                }
                else if(pre>=100){
                    fill = (69 * ccc->thisChCount) / letterCount;
                }
                
                //print 1st line of the bar
                printf("   ");  printf("\u2502");   fillingBar(fill);   printf("\n");
                
                //print 2nd line of the bar
                printf(" %c ",ccc->ch);   printf("\u2502");   fillingBar(fill);     printf("%.2f%%\n",(ccc->thisChCount)*100/(float)letterCount);
                
                //print 3rd line of the bar
                printf("   ");  printf("\u2502");   fillingBar(fill);   printf("\n   \u2502\n");

                ccc = ccc->nextCh;  //go to the next node
                times++;
            }

            if(iter!=0){
                printf("   ");
                lastLine(3);    //to print the last line of the chart

            }

        }

    }
        //when word Mode is ON
    else{   

        //check whether the limit is larger than the no. of total words
        if(iter > wordCount){
            iter = wordCount;
        }


        //to sort the nodes
        sortWords(&headWord);
        
        //to find the size of the longest word
        word * p1 = headWord;  
        int maxLength = maxWordLength(p1,iter);

        //tempory pointer that points to the head node
        word * p2 = headWord;

        if(scaled == 0){    //when scaled Mode is OFF

            //if limit is zero no new line
            if(iter!=0){
                printf("\n");
            }

            unsigned long long int maxW;    //to find the maximum frequency
            float pre;  //percentage of the maximum frequency
            while(p2 != NULL && times<iter){

                if(times==0){   //in the 1st iteration
                    maxW = p2->thisWordCount;
                    pre = (maxW*100) / (float)wordCount;
                }
              
                if(pre>=10 && pre<100){
                    fill = ((71-maxLength) * p2->thisWordCount) / wordCount;
                }
                else if(pre<10){
                    fill = ((72-maxLength) * p2->thisWordCount) / wordCount;
                }
                else if(pre>=100){
                    fill = ((70 - maxLength) * p2->thisWordCount) / wordCount;
                }
              
                //printing the 1st line of the bar
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502");    fillingBar(fill);   printf("\n");

                //printing the 2nd line of the bar
                printf(" %s",p2->toWord);    movingCursor(strlen(p2->toWord),maxLength);     printf("\u2502");    fillingBar(fill);
                printf("%.2f%%\n",(p2->thisWordCount)*100/(float)wordCount);

                //printing the 3rd line of the bar
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502");    fillingBar(fill);   printf("\n");
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502\n");

                p2 = p2->nextWord;  //go to the next node
                times++;
            }

            if(iter!=0){
                printf(" ");
                movingCursor(0,maxLength);  //move the cursor
                lastLine(maxLength +2); //printing the last line

            }
          

        }
        else{   //if scaled mode is ON
            
            unsigned long long int maxW;    //to find the maximum frequency
            float pre;  //percentage of the maximum frequency

            //if limit is zero no new line
            if(iter!=0){
                printf("\n");
            }

            while(p2 != NULL && times<iter){

                if(times==0){   //in the 1st iteration
                    maxW = p2->thisWordCount;
                    pre = (maxW*100) / (float)wordCount;
                }

                if(pre>=10 && pre<100){
                    fill = ((71-maxLength) * p2->thisWordCount) / maxW;
                }
                else if(pre<10){
                    fill = ((72-maxLength) * p2->thisWordCount) / maxW;
                }
                else if(pre>=100){
                    fill = ((70 - maxLength) * p2->thisWordCount) / maxW;
                }

                //printing the 1st line of the bar
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502");    fillingBar(fill);   printf("\n");

                //printing the 2nd line of the bar
                printf(" %s",p2->toWord);    movingCursor(strlen(p2->toWord),maxLength);     printf("\u2502");    fillingBar(fill);
                printf("%.2f%%\n",(p2->thisWordCount)*100/(float)wordCount);

                //printing the 3rd line of the bar
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502");    fillingBar(fill);   printf("\n");
                printf(" ");    movingCursor(0,maxLength);     printf("\u2502\n");

                p2 = p2->nextWord;  //go to the next node
                times++;
            }

            if(iter!=0){
                printf(" ");
                movingCursor(0,maxLength);  //move the cursor
                lastLine(maxLength +2); //printing the last line

            }
            
        }

    }

    return 0;
        
}

    
//To remove Symbols from the text
void removeSymbols(char * line){    

    char tempLine[2000]={0};    //tempory char array to hold the line

    int tempIndex = 0;
    int i;

    for(i=0;line[i]!='\0';i++){
        if( isalnum(line[i]) || line[i]==32 ){
            tempLine[tempIndex] = line[i];
            tempIndex++;
        }
    }

    empty(line);    //to clear the line
    strcpy(line,tempLine);  //copy the processed text

}

//to convert text to lower case
void toMakeLowerCase(char * line){

    int i;
    for(i=0;line[i]!='\0';i++){
        if(isupper(line[i])){
            line[i] = line[i] + 32;     //make the letter to lowercase
        }
    }

}

//creating nodes for words
void copyingWords(char * line){

    int i,j,k;  //for counting
    char temp[100]={0};     //tempory char array to hold a word
    word * tempHead;    //tempory word pointer
    for(i=0;line[i]!='\0';i++){

        tempHead = headWord;    //tempory pointer points to the head of the linked list

        if(headWord == NULL){   //if the head of the linked list points to NULL
            
            word * firstNode = (word*)malloc(sizeof(word));     //create a new node
            wordCount++;
            
            //get a word to the tempory char array
            for(j=0;line[j]!='\0';j++){
                if(line[j]!=' '){
                    for(k=0; (line[j]!=' ')&&(line[j]!='\0') ;k++){
                        temp[k]=line[j];
                        j++;
                    }

                    i = j;

                    //copy the word to the node
                    strcpy(firstNode->toWord,temp);
                    firstNode->thisWordCount = 1;
                    break;
                }
            }
           
            //linking the new node to linked list
            firstNode->nextWord = headWord;
            headWord = firstNode;   

        }

        if(line[i]!=' ' && line[i]!='\0'){  //if a new word found

            empty(temp);    //clear the tempory char array
            
            //copy the word to tempory char array
            for(k=0; (line[i]!=' ')&&(line[i]!='\0') ;k++){
                temp[k]=line[i];
                i++;
            }
        }
        else{   //if no word found
            continue;
            }

        //to check whether the founded word is in the linked list
        while(tempHead!=NULL){

                //if the founded word exists in the list
                if(!(strcmp(tempHead->toWord,temp))){   
                    (tempHead->thisWordCount)++;
                    wordCount++;
                    break;
                }

                //if it does not exist in the list
                else if(tempHead->nextWord == NULL){
                    
                    //create a new node
                    word * newNode = (word*)malloc(sizeof(word));
                    wordCount++;
                    newNode->thisWordCount = 1;

                    //copy the word to the node
                    strcpy(newNode->toWord,temp);

                    //linking the new node to the linked list
                    newNode->nextWord = tempHead->nextWord;
                    tempHead->nextWord = newNode;

                    break;
                }
                //going to the next node 
                else{   
                    tempHead = tempHead->nextWord;
                }

            }

    }

}


//creating nodes for characters
void filteringLetters(char * line){

    letter * temp;

    for(int i=0;line[i]!='\0';i++){

        temp = headLetter;  //temp points to the head of the linked list

        if(line[i]==' '){   //if it finds a blank
            continue;
        }

        //if the head of the linked list points to NULL
        if(headLetter == NULL){

            //create a new node
            letter * firstNode = (letter*)malloc(sizeof(letter));
            letterCount++;
            //inserting the character to the node
            firstNode->ch = line[i];
            firstNode->thisChCount = 1;

            //linking the new node to the linked list
            firstNode->nextCh = headLetter;
            headLetter = firstNode;
       }

       //to check whether the founded character is in the linked list  
       while(temp != NULL){

           //if the founded character exist in the list
           if(line[i]==temp->ch){
               (temp->thisChCount)++;
               letterCount++;
               break;

            }

            //if it does not exist in the list
            else if(temp->nextCh == NULL){

                //create a new node
                letter * newNode = (letter*)malloc(sizeof(letter));
                letterCount++;

                //inserting the character to the node
                newNode ->ch = line[i];
                newNode->thisChCount = 1;

                //linking the new node to the linked list
                newNode->nextCh = temp->nextCh;
                temp->nextCh = newNode;
                break;
            }
           //going to the next node
            else{
                temp = temp->nextCh;

            }
        }

    }

}


//to draw a bar in the chart
void fillingBar(int fill){
    int i;
    for(i=0;i<fill;i++){
        printf("\u2591");
    }
}


//to draw the last horizontal line in the chart
void lastLine(int indent){
    int i;
    printf("\u2514");
    for(i=indent;i<79;i++){
        printf("\u2500");
    }
    printf("\n");
}


//find the longest word in the limit
int maxWordLength(word * start , int iter){
    word * p1 = start;
    int max = -1;
    
    int i;

    for(i=0;i<iter;i++){
        
        if(max < (int)strlen(p1->toWord)){  
            max = (int)strlen(p1->toWord);
        }
        p1 = p1->nextWord;  //going to the next node
    }

    return max;

}


//to move the cursor
void movingCursor(int thisSize , int maxSize){
    int dif = maxSize - thisSize;
    int i;
    for(i=0;i<=dif;i++){
        printf(" ");
    }
}


//to clear a text
void empty(char * line){
    int i;
    for(i=0;line[i]!='\0';i++){
        line[i]='\0';
    }
}


//to sort words
void sortWords(word ** ptrHead){ 

    word* part1; 
    word* part2;
    //temp points to the pointer of the head of the linked list
    word * temp = *ptrHead; 
    
    //if head is NULL or next node of the head is NULL
    if ((temp == NULL) || (temp->nextWord == NULL)) { 

        return; 
    } 
  
    //to divide the linked list into 2 halves
    divide_wordLinkedList(temp, &part1, &part2); 
  
    //sort words in part1
    sortWords(&part1); 

    //sort words in part2
    sortWords(&part2); 
  
    //combine the sorted two parts
    *ptrHead = combine_wordLists(part1, part2); 
    
} 


//to combine 2 lists
word * combine_wordLists(word * part1, word * part2){ 

    word * result = NULL; 
    
    //checking whether the parts are NULL
    if (part1 == NULL){ 
        return (part2); 
    }    
    else if (part2 == NULL){
        return (part1); 
    }

    //sorting process
    if (part1->thisWordCount >= part2->thisWordCount) { 
        result = part1; 
        result->nextWord = combine_wordLists(part1->nextWord, part2); 
    } 
    else { 
        result = part2; 
        result->nextWord = combine_wordLists(part1, part2->nextWord); 
    } 
    
    return (result); 
} 


//to divide the word linked list
void divide_wordLinkedList(word * head, word** ptr_firstPart, word** ptr_secondPart){
 
    word * ptrFast; 
    word * ptrSlow; 

    //checking whether the head or next of head is NULL 
    if(head == NULL || head->nextWord == NULL){
        *ptr_firstPart = head;  //firstpart pointing to the head
        *ptr_secondPart = NULL;
    }
    else{
        
        //pointing to nodes
        ptrSlow = head; 
        ptrFast = head->nextWord; 

        while (ptrFast != NULL) { 
            ptrFast = ptrFast->nextWord; 
            if (ptrFast != NULL) { 
                ptrSlow = ptrSlow->nextWord; 
                ptrFast = ptrFast->nextWord; 
            } 
        }

        //pointing to nodes
        *ptr_firstPart = head;
        *ptr_secondPart = ptrSlow->nextWord;
        ptrSlow->nextWord = NULL;

    }


}    


//to sort chars
void sortChars(letter** ptrHead){ 

    letter* part1; 
    letter* part2; 
    //temp points to the pointer of the head of linked list
    letter* temp = *ptrHead; 
  
    //if head is NULL or next node of the head is NULL
    if ((temp == NULL) || (temp->nextCh == NULL)) { 
        return; 
    } 
  
    //to divide the linked list in 2 halves
    divide_charLinkedList(temp, &part1, &part2); 
  
    //sort characters in part1
    sortChars(&part1); 

    //sort characters in part2
    sortChars(&part2); 
  
    //combine the sorted two parts
    *ptrHead = combine_charLists(part1, part2); 
    
} 


//to combine 2 lists
letter * combine_charLists(letter * part1, letter * part2){

    letter * result = NULL; 
  
    //checking whether the parts are NULL
    if (part1 == NULL){
        return (part2); 
    }    
    else if (part2 == NULL){ 
        return (part1); 
    }

    //sorting process
    if (part1->thisChCount >= part2->thisChCount) { 
        result = part1; 
        result->nextCh = combine_charLists(part1->nextCh, part2); 
    } 
    else { 
        result = part2; 
        result->nextCh = combine_charLists(part1, part2->nextCh); 
    } 
    
    return (result); 
} 


//to divide the char linked list
void divide_charLinkedList(letter * head, letter ** ptr_firstPart, letter ** ptr_secondPart){ 

    letter * ptrFast; 
    letter * ptrSlow; 

    //checking whether the head or next of head is NULL 
    if(head == NULL || head->nextCh == NULL){
        *ptr_firstPart = head;
        *ptr_secondPart = NULL;
    }
    else{

        //pointing to nodes
        ptrSlow = head; 
        ptrFast = head->nextCh; 

        while (ptrFast != NULL) { 
            ptrFast = ptrFast->nextCh; 
            if (ptrFast != NULL) { 
                ptrSlow = ptrSlow->nextCh; 
                ptrFast = ptrFast->nextCh; 
            } 
        }
        
        //pointing to nodes
        *ptr_firstPart = head;
        *ptr_secondPart = ptrSlow->nextCh;
        ptrSlow->nextCh = NULL;

    }


} 