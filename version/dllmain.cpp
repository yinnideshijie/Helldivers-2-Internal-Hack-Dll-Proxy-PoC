// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include "version.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "Memory.h"
#include "GameData.h"
#include <conio.h> // For _getch() on Windows

HANDLE hMainThread;

// Function to initialize and show the console window
void InitializeConsole() {
    FILE* pFile = nullptr;
    AllocConsole();
    SetConsoleTitle(L"Helldivers 2 DLL Proxy PoC");
    freopen_s(&pFile, "CONOUT$", "w", stdout);
}

//Check Key Pressed
bool keyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 1);
}

//Console Menu using checkbox
struct Checkbox {
    std::string title;
    bool checked;
};

// Function to display checkboxes
void displayCheckboxes(const std::vector<Checkbox>& checkboxes, size_t selectedCheckbox) {
    system("cls"); // Clear the console (Windows specific)

    printf("[Ready] : Select some of the features below by pressing the [Space] key.\n");
    printf("[Ready] : Press [Enter] to run the feature you selected.\n");
    printf("[Ready] : After pressing [Enter], the selected features cannot be changed.\n");

    std::cout << "Checkboxes:\n";
    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (i == selectedCheckbox) {
            std::cout << "> ";
        }
        else {
            std::cout << "  ";
        }

        std::cout << "[" << (checkboxes[i].checked ? 'X' : ' ') << "] " << checkboxes[i].title << "\n";
    }
    std::cout << "\n";
}

DWORD WINAPI Payload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    dllforward::setup();
        
    //Show Console
    InitializeConsole();

    //Console Menu
    const int numCheckboxes = 10;
    std::vector<Checkbox> checkboxes = { {"Inf Health", false}, {"Inf Granades", false}, {"Inf Ammo", false}, {"Inf Syringes", false}, {"Inf Stamina", false}, {"Inf Strategems", false}, {"Inf Mission Time", false}, {"No Reload", false}, {"Max Resources", false}, {"No Recoil", false} }; // Initialize all checkboxes to unchecked
    size_t selectedCheckbox = 0;
    char userInput;


    HMODULE moduleHandle = nullptr;
    GameData gData;

    printf("[Init] - PoC DLL Proxy...\n");
    printf("[Init] - Thanks to cfemen and gir489...\n");
    printf("[Init] - Waiting for helldivers 2 to fully open...\n");
    do
    {
        moduleHandle = GetModuleHandle(L"game.dll");
        Sleep(1000);
    } while (!moduleHandle);
    Sleep(100);

    

    printf("[Init] - Helldivers 2 Is Ready \n");

    do {

        displayCheckboxes(checkboxes, selectedCheckbox);

        // Get user input
        userInput = _getch(); // Use _getch() for reading a single character without pressing Enter

        // Handle arrow key input
        switch (userInput) {
        case 72: // Up arrow key
            selectedCheckbox = (selectedCheckbox == 0) ? numCheckboxes - 1 : selectedCheckbox - 1;
            break;

        case 80: // Down arrow key
            selectedCheckbox = (selectedCheckbox == numCheckboxes - 1) ? 0 : selectedCheckbox + 1;
            break;

        case ' ': // Space bar
            // Toggle the state of the selected checkbox
            checkboxes[selectedCheckbox].checked = !checkboxes[selectedCheckbox].checked;
            break;

        case 13: // Enter key
            std::cout << "Activate Cheat Feature.\n";
            break;

        case 27: // Esc key
            
            break;

        default:
            // Ignore other key presses
            break;
        }

    } while (userInput != 13); // Loop until the Esc key is pressed
    
    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (checkboxes[i].checked)
        {
            if (checkboxes[i].title == "Inf Health")
            {
                if (!gData.InfHealth) // no need but its old code when activate using hotkey, but need to much hotkey for all feature
                {
                    BYTE InfHealthByte[] =
                    {
                        0xB8, 0x0F, 0x27, 0x00, 0x00,
                        0x41, 0x89, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,
                        0x48, 0x8B, 0x5C, 0x24, 0x20,
                        0x48, 0x8B, 0x74, 0x24, 0x28,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t InfHealth = Memory::FindPattern("game.dll", "41 8B 84 8B 28 4C 00 00 48 8B 5C 24 20 48 8B 74 24 28");
                    LPVOID memory = Memory::AllocateMemory(0x1000);
                    Memory::CreateTrampoline(InfHealth, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth + 18, InfHealthByte, Memory::ArrayLength(InfHealthByte));
                    gData.InfHealth = !gData.InfHealth;
                    //create trampolin
                    printf("[Active] Infinite Health\n");
                }
            }

            if (checkboxes[i].title == "Inf Granades")
            {
                if (!gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern("game.dll", "41 FF 08 4A 8B 84 ED");
                    printf("[Active] Infinite Health : %p\n", GrenadesAddress);
                    Memory::Nop((LPVOID)(GrenadesAddress), 3);
                    gData.InfGrenades = !gData.InfGrenades;
                    printf("[Active] Infinite Grenades\n");
                }
            }

            if (checkboxes[i].title == "Inf Ammo")
            {
                if (!gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern("game.dll", "41 83 2C C2 01 4D 8D 04 C2 49 8B 84 CA");
                    BYTE AmmoPatch[] = { 0x00 };
                    Memory::Patch((LPVOID)(AmmoAddress + 4), AmmoPatch, 1);
                    gData.InfAmmo = !gData.InfAmmo;
                    printf("[Active] Infinite Ammo\n");
                }
            }

            if (checkboxes[i].title == "Inf Syringes")
            {
                if (!gData.InfSyringes)
                {
                    uintptr_t Syringes = Memory::FindPattern("game.dll", "41 FF CF 3B C2 74 61");
                    Memory::Nop((LPVOID)(Syringes), 3);
                    gData.InfSyringes = !gData.InfSyringes;
                    printf("[Active] Infinite Syringes\n");
                }
            }

            if (checkboxes[i].title == "Inf Stamina")
            {
                if (!gData.InfStamina)
                {
                    uintptr_t Stamina = Memory::FindPattern("game.dll", "F3 41 0F 11 08 8B 48 10 E8 F1");
                    BYTE StaminaPatch[] = {  0xF3, 0x41, 0x0F, 0x11, 0x30 };
                    Memory::Patch((LPVOID)(Stamina), StaminaPatch, 5);
                    gData.InfStamina = !gData.InfStamina;
                    printf("[Active] Infinite Stamina\n");
                }
            }

            if (checkboxes[i].title == "Inf Strategems") 
            {
                if (!gData.InfStrategems)
                {
                    uintptr_t Strategems = Memory::FindPattern("game.dll", "C0 F3 48 0F 2C C8 48 03 48 18 48 89 8C 37 40 02 00 00");
                    uintptr_t Strategems2 = Memory::FindPattern("game.dll", "0F 86 BF 01 00 00 0F");
                    BYTE StrategemsPatch1[] = { 0x8D, 0x01, 0x90 };
                    BYTE StrategemsPatch2[] = { 0x90, 0xE9 };
                    Memory::Nop((LPVOID)(Strategems+6), 4);
                    Memory::Patch((LPVOID)(Strategems + 25), StrategemsPatch1, 3);
                    Memory::Patch((LPVOID)(Strategems + 33), StrategemsPatch1, 3);
                    Memory::Patch((LPVOID)(Strategems2), StrategemsPatch2, 2);
                    gData.InfStrategems = !gData.InfStrategems;
                    printf("[Active] Infinite Strategems\n");
                }
            }

            if (checkboxes[i].title == "Inf Mission Time")
            {
                if (!gData.InfMissionTime)
                {
                    uintptr_t MissionTime = Memory::FindPattern("game.dll", "F3 0F 5C C7 F3 41 0F 5F C5");
                    Memory::Nop((LPVOID)(MissionTime), 4);
                    gData.InfMissionTime = !gData.InfMissionTime;
                    printf("[Active] Infinite Mission Time\n");
                }
            }

            if (checkboxes[i].title == "No Reload")
            {
                if (!gData.NoReload)
                {
                    BYTE NoReloadByte[] =
                    {
                        0x90,
                        0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,
                        0x8B, 0x48, 0x10,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t NoReload = Memory::FindPattern("game.dll", "41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10");

                    LPVOID memory = Memory::AllocateMemory(0x1000);
                    Memory::CreateTrampoline(NoReload, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, NoReload + 14, NoReloadByte, Memory::ArrayLength(NoReloadByte));
                    gData.NoReload = !gData.NoReload;
                    printf("[Active] No Reload\n");
                }
            }

            if (checkboxes[i].title == "Max Resources")
            {
                if (!gData.MaxResources)
                {
                    BYTE MaxResourcesByte[] =
                    {
                        0x41, 0x81, 0x84, 0x8A, 0xEC, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF0, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF4, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t MaxResources = Memory::FindPattern("game.dll", "45 01 B4 8A EC 17 00 00");

                    LPVOID memory = Memory::AllocateMemory(0x1000);
                    Memory::CreateTrampoline(MaxResources, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, MaxResources + 17, MaxResourcesByte, Memory::ArrayLength(MaxResourcesByte));
                    gData.MaxResources = !gData.MaxResources;
                    printf("[Active] x500 Samples\n");
                }
            }

            if (checkboxes[i].title == "No Recoil")
            {
                if (!gData.Recoil)
                {
                    BYTE RecoilByte[] =
                    {
                        0xEB
                    };

                    uintptr_t Recoil = Memory::FindPattern("game.dll", "75 24 45 8B C7");
                    Memory::Patch((LPVOID)(Recoil), RecoilByte, 1);
                    gData.Recoil = !gData.Recoil;
                    printf("[Active] No Recoil\n");
                }
            }
        }
    }
    printf("[Exit] Unload\n");
    FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 0);
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

