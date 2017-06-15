#include "PEB.h"
#include <psapi.h>
#include <winuser.h>
#include <wingdi.h>
/*

FS->Thread Environment Block->PEB data struct
PEB->LoaderData (PEB_LDR_DATA structure)
PEB_LDR_DATA->InLoadOrderModuleList (LDR_MODULE structure)
LDR_MODULE structure, MEMBER BaseAddress will be pointer to full address of IMAGE_DOS_HEADER
IMAGE_DOS_HEADER e_lfanew + BASEADRESS will get IMAGE_FILE_HEADER
IMAGE_FILE_HEADER + sizeof(IMAGE_FILE_HEADER) is IMAGE_OPTIONAL_HEADER
IMAGE_OPTION_HEADER member IMAGE_DATA_DIRECTORY will be an array of IMAGE_DATA_DIRECTORY structures
IMAGE_DATA_DIRECTORY[0] member VirtualAddress + BaseAddress will be VA of EAT

*/

int main(int argc, char *argv[])
{
    PBYTE pLdrLoadDll;
    PBYTE pNtAllocateVirtualMemory;
    PBYTE pRtlInitUnicodeString;
    LPTSTR lpFunc;
    TCHAR tBuffer[1024] = {0};
    lpFunc = argv[1];
    
    PPEB pPeb;
    PLDR_MODULE pPldrMod;
    PPEB_LDR_DATA pPebLdrPtr;
    HMODULE pBaseAddress;
     
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_FILE_HEADER pFileHeader;
    PIMAGE_OPTIONAL_HEADER pOptHeader;
    PIMAGE_EXPORT_DIRECTORY pExportTable;
    DWORD dwError; 
    DWORD dwExportTableRVA;
    DWORD dwModuleBase;
    DWORD dwX;
    
    HANDLE hHandle;
    
    asm("Mov %0, Dword Ptr Fs:[0x30] \n" : "=r" (pPeb));
    
    pPebLdrPtr = pPeb->LoaderData;
    
    pPldrMod =((PBYTE)pPebLdrPtr->InLoadOrderModuleList.Flink);
    pPldrMod =((PBYTE)pPldrMod->InLoadOrderModuleList.Flink);
    
    printf("[*]Module name: %ws\r\n", pPldrMod->BaseDllName.Buffer);

    pBaseAddress = pPldrMod->BaseAddress;

    pDosHeader = (PIMAGE_DOS_HEADER)pBaseAddress;
    pFileHeader = (PIMAGE_FILE_HEADER)((PBYTE)pDosHeader + pDosHeader->e_lfanew + 4);
    pOptHeader = (PBYTE)pFileHeader + sizeof(IMAGE_FILE_HEADER);
    dwExportTableRVA = pOptHeader->DataDirectory[0].VirtualAddress;
    pExportTable = (PBYTE)pBaseAddress + dwExportTableRVA;

    
    dwModuleBase = pBaseAddress;

    
    PDWORD	FunctionNameAddressArray = ((DWORD)pExportTable->AddressOfNames) + ((PBYTE)dwModuleBase);
    PWORD	FunctionOrdinalAddressArray = (DWORD)pExportTable->AddressOfNameOrdinals + (PBYTE)dwModuleBase;
    PDWORD	FunctionAddressArray = (DWORD)pExportTable->AddressOfFunctions + (PBYTE)dwModuleBase;
     
    for(dwX=0; dwX < pExportTable->NumberOfNames; dwX++)
    {
         PCHAR pFunctionName= FunctionNameAddressArray[dwX] + (PBYTE)dwModuleBase;
         
         printf("\r\n[*] %s located at Index %ld\r\n", pFunctionName, dwX);
        
         WORD wOrdinal = FunctionOrdinalAddressArray[dwX];
         DWORD dwAddr = FunctionAddressArray[wOrdinal];
              
         printf("Address: 0x%x\r\n", (dwAddr + (PBYTE)dwModuleBase));
         
         system("PAUSE");
         
    }

    system("pause");
  
  
    return 0;
}
