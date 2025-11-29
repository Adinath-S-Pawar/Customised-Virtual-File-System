/////////////////////////////////////////////////////////////////////
//
//   Header files
//
//////////////////////////////////////////////////////////////////////

#include<stdlib.h>
#include<stdio.h>
//#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#include<iostream>

using namespace std;

/////////////////////////////////////////////////////////////////////
//
//  User defined macros
//
//////////////////////////////////////////////////////////////////////

# define MAXFILESIZE 100

//maximum number of files that we can open
# define MAXOPENEDFILES 20  

 // maximum number of files that we can create
# define MAXINODE 5        

# define READ 1
# define WRITE 2
# define EXECUTE 4

# define REGUlARFILE 1
# define SPECIALFILE 2

# define START  0
# define CURRENT 1
# define END 2

# define EXECUTE_SUCCESS 0

/////////////////////////////////////////////////////////////////////
//
//  User defined macros for error handling
//
//////////////////////////////////////////////////////////////////////

# define ERR_INVALID_PARAMETER -1
# define ERR_NO_INODES -2
# define ERR_FILE_ALREADY_EXIST -3
# define ERR_FILE_NOT_EXIST -4
# define ERR_PERMISSION_DENIED -5
# define ERR_INSUFFICIENT_SPACE -6
# define ERR_INSUFFICIENT_DATA -7

/////////////////////////////////////////////////////////////////////
//
//  Structure name : BootBlock
//  Description :    Holds Information to boot the operating system
//
//////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

/////////////////////////////////////////////////////////////////
//
//  Structure name : SuperBlock
//  Description :    Holds Information about the file system
//
/////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

/////////////////////////////////////////////////////////////////
//
//  Structure name : Inode
//  Description :    Holds Information about the file 
//
/////////////////////////////////////////////////////////////////

typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int LinkCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
}INODE, *PINODE, **PPINODE;

/////////////////////////////////////////////////////////////////
//
//  Structure name : FileTable
//  Description :    Holds Information about the opened file 
//
/////////////////////////////////////////////////////////////////

typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    PINODE ptrinode;
}FILETABLE, *PFILETABLE;

/////////////////////////////////////////////////////////////////
//
//  Structure name : UAREA
//  Description :    Holds Information about the process 
//
/////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[50];
    PFILETABLE UFDT[MAXOPENEDFILES];
};

/////////////////////////////////////////////////////////////////
//
//  Global variables used in Project
//
/////////////////////////////////////////////////////////////////

SuperBlock superobj;
PINODE head = NULL;
UAREA uareaobj;
BootBlock bootobj;

/////////////////////////////////////////////////////////////
//
//   Function Name : InitialiseUAREA
//   Description : It is used to initialise the contents of UAREA
//   Author : Adinath Santosh Pawar
//   Date : 10/08/2025
//
////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;
    while(i < MAXOPENEDFILES)
    {
        uareaobj.UFDT[i] = NULL;
        i++;
    }
    cout<<"CVFS : UAREA initialised successfully \n";
}

/////////////////////////////////////////////////////////////
//
//   Function Name : InitialiseSuperblock
//   Description : It is used to initialise the contents of super block
//   Author : Adinath Santosh Pawar
//   Date : 10/08/2025
//
////////////////////////////////////////////////////////////

void InitialiseSuperblock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    cout<<"CVFS : superblock initialised successfully\n";

}

/////////////////////////////////////////////////////////////
//
//   Function Name : CreateDILB
//   Description : It is used to create linked list of Inodes
//   Author : Adinath Santosh Pawar
//   Date : 10/08/2025
//
////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = new INODE;
        
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->LinkCount = 0;
        newn->Permission = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Buffer = NULL;
        newn->next  = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
        i++;
    }
    cout<<"CVFS : DILB created successfully\n";
}

/////////////////////////////////////////////////////////////
//
//   Function Name : StartAuxilaryDataInitialisation
//   Description : It is used to initialise the Auxilary data
//   Author : Adinath Santosh Pawar
//   Date : 10/08/2025
//
////////////////////////////////////////////////////////////

void StartAuxilaryDataInitialisation()
{
    strcpy(bootobj.Information , "Boot Process of Operating system done \n");

    cout<<bootobj.Information<<endl;

    InitialiseSuperblock();

    CreateDILB();

    InitialiseUAREA();

    cout<<"CVFS : Auxilary data initialised successfully\n";
}

/////////////////////////////////////////////////////////////
//
//   Function Name : DisplayHelp
//   Description : Display information about commands
//   Author : Adinath Santosh Pawar
//   Date : 11/08/2025
//
////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-------------------------------------------------------------------------------------\n");
    printf("---------------------------Command Manual of  CVFS---------------------------\n");
    printf("-------------------------------------------------------------------------------------\n");

    printf("man : It is used to display the specific manual page of command \n");
    printf("exit : It is used to terminate the shell of  CVFS \n");
    printf("clear : It is used to clear screen of  CVFS \n");
    printf("creat : It is used to create new regular file \n");
    printf("unlink : It is used to delete existing file \n");
    printf("stat : It is used to display statistical information about file \n");
    printf("ls : It is used to list all files from the directory \n");
    printf("write : It is used to write the data into the file \n");
    printf("read : It is used to read the data from the file \n");

    printf("-------------------------------------------------------------------------------------\n");

}

/////////////////////////////////////////////////////////////
//
//   Function Name : ManPage
//   Description : Display manual page of the command
//   Input :  Accepts the command name
//   Output : Displays the manual details of the command
//   Author : Adinath Santosh Pawar
//   Date : 11/08/2025
//
////////////////////////////////////////////////////////////

void ManPage(
                char *name  // Name of command
            )
{
    if(strcmp(name,"creat") == 0)
    {
        printf("Description : This command is used to create new regular file on our file system\n");

        printf("Usage : creat File_name Permissions \n ");
        printf("File_name : The name of file that you want to create \n");
        printf("Permissions : \n1 : Read \n2 : Write \n3 : Read + Write");
    }
    else if(strcmp(name,"exit") == 0)
    {
        printf("Description : This command is used to terminate  CVFS\n");

        printf("Usage : exit \n ");
    
    }
    else if(strcmp(name,"unlink") == 0)
    {
        printf("Description : This command is used to delete regular file from our file system\n");
        printf("Usage : unlink File_name \n ");
        printf("File_name : The name of file that you want to create \n");
    }
    else if(strcmp(name,"stat") == 0)
    {
        printf("Description : This command is used to display statistical information about file \n");
        printf("Usage : stat File_name \n ");
        printf("File_name : The name of file whose information you want to display \n");
    }
    else if(strcmp(name,"ls") == 0)
    {
        printf("Description : This command is used to list all file names from directory \n");
        printf("Usage : ls \n ");
    }
    else if(strcmp(name,"write") == 0)
    {
        printf("Description : This command is used to write data into file \n");
        printf("Usage : write file_descriptor \n ");
    }
    else if(strcmp(name,"read") == 0)
    {
        printf("Description : This command is used to read data from file \n");
        printf("Usage : read file_descriptor size \n ");

        printf("file_descriptor : Its a value returned by create system call \n");
        printf("Size: Number of bytes you want to read \n");
    }
    // add more options here
    else
    {
        printf("No manual entry for %s \n",name);
    }
}

/////////////////////////////////////////////////////////////
//
//   Function Name : IsFileExists
//   Description : It is used to check whether the given file name exists or not
//   Input :  It accepts the file name 
//   Output : It returns boolean value (True : if present , False : if not present)
//   Author : Adinath Santosh Pawar
//   Date : 11/08/2025
//
////////////////////////////////////////////////////////////

bool IsFileExists(
                    char *name  // name of file that we want to check
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if( (strcmp(name, temp->FileName) == 0) && (temp->FileType == REGUlARFILE))
        {
            bFlag = true;
            break;
        }

        temp = temp->next;

    }

    return bFlag;
}

/////////////////////////////////////////////////////////////
//
//   Function Name : CreateFile
//   Description : It is used to create a new regular file
//   Input :  It accepts the file name and permission
//   Output : It returns file descriptor
//   Author : Adinath Santosh Pawar
//   Date : 11/08/2025
//
////////////////////////////////////////////////////////////

int CreateFile(
                char *name,     // Name of file
                int permission  // Permission to create file
            )
{
    PINODE temp = head;
    int i = 0;

    printf("current inodes remaining : %d \n",superobj.FreeInodes);

    // filters

    // if file name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if entered permission is invalid
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // check whether empty iNodes is present or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // check whether file is already existing or not
    if(IsFileExists(name) == true )
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // loop to search free iNode
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }
    // iNode not found
    if(temp == NULL)
    {
        printf("Inode not found \n");
        return ERR_NO_INODES;
    }

    //search first non null value from ufdt
    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXINODE)
    {
        printf("Unable to create file as MAX OPENED FILE LIMIT REACHED\n");
        return -1;
    }
    
    //allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // initialise elements of file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Count = 1;
    uareaobj.UFDT[i]->Mode = permission;

    // connect file  table with IIT
    uareaobj.UFDT[i]->ptrinode = temp; 
    /*
         uareaobj.UFDT[i] points to FileTable
         uareaobj.UFDT[i]->ptrinode ------> FileTable->ptrinode which points to inode
         temp points to free inode
     */

    strcpy(uareaobj.UFDT[i]->ptrinode->FileName , name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGUlARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->LinkCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // allocate memory for buffer 
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    // decrement the numbers of free inodes by 1
    superobj.FreeInodes--;

    return i;
}

/////////////////////////////////////////////////////////////
//
//   Function Name : UnlinkFile
//   Description : It is used to delete a regular file
//   Input :  It accepts the file name 
//   Output : It returns status(0,1)
//   Author : Adinath Santosh Pawar
//   Date : 15/08/2025
//
////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name  // Name of file
)
{
    int i = 0;
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if( IsFileExists(name) == false )
    {
        return ERR_FILE_NOT_EXIST;
    }

    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if( strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0 )
            {
                // Deallocate the memory of buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);

                // reset all values of Inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->LinkCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;

                // Deallocate memory of file table 
                free(uareaobj.UFDT[i]);

                //set NULL to UFDT number
                uareaobj.UFDT[i] == NULL;

                // increment the value of free Inodes count
                superobj.FreeInodes++;

                break;
            }
        }   // end of if strcmp

    }   // end of for

    return EXECUTE_SUCCESS;
} // end of unlinkfile

/////////////////////////////////////////////////////////////
//
//   Function Name : ls_file
//   Description : It is used to display the information about all files in directory
//   Input :  Nothing
//   Output : Nothing
//   Author : Adinath Santosh Pawar
//   Date : 15/08/2025
//
////////////////////////////////////////////////////////////

void ls_file()
{
    PINODE temp = head;

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s \n",temp->FileName);
        }
        temp = temp->next;
    }
}

/////////////////////////////////////////////////////////////
//
//   Function Name : stat_file
//   Description : It is used to display the information about the given file
//   Input :  file name
//   Output : Nothing
//   Author : Adinath Santosh Pawar
//   Date : 15/08/2025
//
////////////////////////////////////////////////////////////

// CVFS > stat demo.txt


int stat_file(
                    char *name  // Name of file
)
{
    PINODE temp = head;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if( IsFileExists(name) == false )
    {
        return ERR_FILE_NOT_EXIST;
    }

    while(temp != NULL)
    {
        if( (strcmp(name, temp->FileName) == 0) && (temp->FileType != 0))
        {
            printf("------------------Statistical information of file------------------ \n");
            printf("File name : %s \n",temp->FileName);
            printf("File size on Disk : %d \n",temp->FileSize);
            printf("Actual File size  : %d \n",temp->ActualFileSize);
            printf("Link count : %d \n",temp->LinkCount);
            printf("File permission : ");
            if(temp->Permission == READ)
            {
                printf("READ \n");
            }
            else if(temp->Permission == WRITE)
            {
                printf("WRITE \n");
            }
            else if(temp->Permission == READ + WRITE)
            {
                printf("Read + Write \n");
            }
            printf("File type : ");
            if(temp->FileType == REGUlARFILE)
            {
                printf("Regular file \n");
            }
            else if(temp->FileType == SPECIALFILE)
            {
                printf("Special file \n");
            }
            printf("----------------------------------------------------------------------\n");
        }

        temp = temp->next;
    }

    return EXECUTE_SUCCESS;
} // end of 

/////////////////////////////////////////////////////////////
//
//   Function Name : write_file
//   Description : It is used to write the contents into the file
//   Input :  file descriptor
//            Address of buffer which contains data
//            Size of data that we want to write
//   Output : Number of bytes successfully written into file
//   Author : Adinath Santosh Pawar
//   Date : 15/08/2025
//
////////////////////////////////////////////////////////////

// CVFS > write 3

int write_file(
                    int fd,         // file descriptor
                    char *data,     // data that we want to write
                    int size        // size of data that we want to write
                )       
{
    unsigned long int offset = 0;

    // invalid value of fd
    if(fd < 0 || fd > MAXOPENEDFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // file is not opened or create with given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // if there is no permission to write the data into the file
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    //unable to write as there is no sufficient space
    if( (MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset ) < size )
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //write actual size 
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer,data,size);

    //update write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // update actual size of file
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;    

    return size;

}

/////////////////////////////////////////////////////////////
//
//   Function Name : read_file
//   Description : It is used to read the contents from the file
//   Input :  file descriptor
//            Address of empty buffer 
//            Size of data that we want to read
//   Output : Number of bytes successfully read from file
//   Author : Adinath Santosh Pawar
//   Date : 15/08/2025
//
////////////////////////////////////////////////////////////

// CVFS > read 3 10

int read_file(
                int fd,         // file descriptor
                char *data,     // address of empty buffer
                int size        //Number of bytes that we want to read
            )
{
    // invalid value of fd
    if(fd < 0 || fd > MAXOPENEDFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL || size <=0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // file is not opened or create with given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // if there is no permission to read the data into the file
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    //unable to read as there is no sufficient data
    if( (MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset ) < size )
    {
        return ERR_INSUFFICIENT_DATA;
    }

    strncpy(data, uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset ,size);

    uareaobj.UFDT[fd]->ReadOffset =  uareaobj.UFDT[fd]->ReadOffset + size;

    return size;

}




///////////////////////////////////////////////////////////////////
//
//   Entry point function of project (main)
//
//////////////////////////////////////////////////////////////////
int main()
{
    char str[80] = {'\0'}; // max size of char on single line on console
    int iCount = 0;
    int iRet = 0;
    char Command[5][80];
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL;

    StartAuxilaryDataInitialisation();

    cout<<"-------------------------------------------------------------------------------------\n";
    cout<<"------------------------- CVFS started successfully------------------------\n";
    cout<<"-------------------------------------------------------------------------------------\n";

    while(1)
    {
        fflush(stdin);

        strcpy(str,""); //flush

        printf("\n CVFS > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s",Command[0],Command[1],Command[2],Command[3]);
        
        fflush(stdin);

        if(iCount == 1)
        {
            //  CVFS > exit
            if(strcmp(Command[0] , "exit") == 0)   
            {
                printf("Thank you for using  CVFS \n");
                printf("Deallocating all resources \n");

                break;
            }
            //  CVFS > help
            else if(strcmp(Command[0] , "help") == 0)
            {
                DisplayHelp();
            }
            //  CVFS > clear
            else if(strcmp(Command[0] , "clear") == 0)
            {
                system("cls");
            }
             //  CVFS > ls
            else if( strcmp(Command[0] , "ls") == 0 )
            {
                ls_file();
            }
            else
            {
                printf("Command not found... \n");
                printf("Please refer Help option or use man command \n");

            }



        }   // end of if iCount == 1

        else if(iCount == 2)
        {
            //  CVFS > man creat
            if(strcmp(Command[0],"man") == 0)
            {
                ManPage(Command[1]);
            }
            //  CVFS > unlink Demo.txt
            else if(strcmp(Command[0],"unlink") == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("Unlink operation is successfully performed \n");
                }
                else if(iRet = ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to do Unlink Activity as file is not present \n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function \n");
                    printf("Please check man page for more details \n");
                }
            }
            //  CVFS > stat demo
            else if(strcmp(Command[0],"stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to display statistics as file is not present \n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function \n");
                    printf("Please check man page for more details \n");
                }
            }
             //  CVFS > write 3
            else if(strcmp(Command[0],"write") == 0)
            {
                printf("Please enter data to write in file \n");
                fgets(InputBuffer, MAXFILESIZE,stdin);

                iRet = write_file(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);

                if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Insufficient space int the data block for the file \n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no write permission \n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function \n");
                    printf("Please check man page for more details \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : FD is invalid \n");
                }

                else
                {
                    printf("%d bytes gets written successfully into the file \n",iRet);
                    printf("data from file is : %s\n",uareaobj.UFDT[0]->ptrinode->Buffer);
                }
            }
            else
            {
                printf("Command not found... \n");
                printf("Please refer Help option or use man command \n");

            }

        }   // end of if iCount == 2

        else if(iCount == 3)
        {
            //  CVFS > creat Ganesh.txt 3
            if(strcmp(Command[0],"creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]) ); //ascii to int

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function \n");
                    printf("Please check man page for more details \n");
                }

                else if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there is no iNodes \n");
                }

                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file as file is already existing \n");
                }
                else
                {
                    printf("File successfully created with fd %d : \n ",iRet);
                }

            }
            // CVFS > read 3 10
            else if(strcmp(Command[0],"read") == 0 )
            {
                EmptyBuffer = (char *)malloc( sizeof( atoi(Command[2]) ) );

                iRet = read_file( atoi(Command[1]), EmptyBuffer, atoi(Command[2]) );

                if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data int the data block of the file \n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to read as there is no read permission \n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function \n");
                    printf("Please check man page for more details \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : FD is invalid \n");
                }
                else
                {
                    printf("read operation successfull \n");
                    printf("Data from file is %s \n",EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {
                printf("Command not found... \n");
                printf("Please refer Help option or use man command \n");
            }



        }   // end of if iCount == 3

        else if(iCount == 4)
        {

        }   // end of if iCount == 4
        else
        {
            printf("Command not found... \n");
            printf("Please refer Help option or use man command \n");

        }   // end of invalid command part


    }// end of while (custom shell )

    return 0;

}   // end of main