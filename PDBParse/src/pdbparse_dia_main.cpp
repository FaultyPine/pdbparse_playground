
#include <windows.h>
#include "atlbase.h"
#include <vector>
#include "DIA2Dump/DIA2Dump.h"
#include "DIA2Dump/PrintSymbol.h"

DWORD g_machineType = CV_CFL_80386;

bool ScanForGlobals(IDiaSymbol* globalSymbol, std::vector<IDiaSymbol*>& outSymbols)
{
    enum SymTagEnum dwSymTags[] = { SymTagFunction, SymTagThunk, SymTagData };
    ULONG celt = 0;
    IDiaEnumSymbols *pEnumSymbols;
    for (size_t i = 0; i < _countof(dwSymTags); i++, pEnumSymbols = NULL) {
        if (SUCCEEDED(globalSymbol->findChildren(dwSymTags[i], NULL, nsNone, &pEnumSymbols))) 
        {
            IDiaSymbol *pSymbol;
            while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) 
            {
                outSymbols.push_back(pSymbol);
                //pSymbol->Release();
            }
            pEnumSymbols->Release();
        }
        else {
            return false;
        }
    }
    return true;
}


int main()
{
    const wchar_t *filename = L"../TestProj/main.pdb";
    IDiaDataSource *diaDataSource;
    IDiaSession *diaSession;
    IDiaSymbol *globalSymbol;
    if (!LoadDataFromPdb(
        filename,
        &diaDataSource,
        &diaSession,
        &globalSymbol,
        g_machineType))
    {
        printf("Couldn't find pdb to load\n");
        return 1; 
    }
    
    std::vector<IDiaSymbol*> globals = {};
    if (ScanForGlobals(globalSymbol, globals))
    {
        for (IDiaSymbol* symbol : globals)
        {
            DWORD dwSymTag;
            DWORD dwRVA;
            DWORD dwSeg;
            DWORD dwOff;
            BSTR bstrName;
  
            if (symbol->get_symTag(&dwSymTag) != S_OK) 
            {
                break;
            }

            if (symbol->get_relativeVirtualAddress(&dwRVA) != S_OK) 
            {
                dwRVA = 0xFFFFFFFF;
            }

            symbol->get_addressSection(&dwSeg);
            symbol->get_addressOffset(&dwOff);

            wprintf(L"%s: [%08X][%04X:%08X] ", rgTags[dwSymTag], dwRVA, dwSeg, dwOff);
  
            if (dwSymTag == SymTagThunk) {
                if (symbol->get_name(&bstrName) == S_OK) {
                    wprintf(L"%s\n", bstrName);
                    SysFreeString(bstrName);
                }
                else {
                    if (symbol->get_targetRelativeVirtualAddress(&dwRVA) != S_OK) {
                        dwRVA = 0xFFFFFFFF;
                    }
                    symbol->get_targetSection(&dwSeg);
                    symbol->get_targetOffset(&dwOff);
                    wprintf(L"target -> [%08X][%04X:%08X]\n", dwRVA, dwSeg, dwOff);
                }
            }
            else {
                // must be a function or a data symbol
                BSTR bstrUndname;
                if (symbol->get_name(&bstrName) == S_OK) {
                    if (symbol->get_undecoratedName(&bstrUndname) == S_OK) {
                        wprintf(L"%s(%s)\n", bstrName, bstrUndname);
                        SysFreeString(bstrUndname);
                    }
                    else {
                        wprintf(L"%s\n", bstrName);
                    }
                    if (StrStrNIW(bstrName, L"g_appState", 10))
                    {
                        printf("Here's appstate\n");
                    }
                    SysFreeString(bstrName);
                }
            }
        }
        printf("Got globals!\n");
    }

    for (IDiaSymbol* globSymbol : globals)
    {
        globSymbol->Release();
    }
    Cleanup(globalSymbol, diaSession);
    return 0;
}