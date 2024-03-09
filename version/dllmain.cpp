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
#include "xorstr.hpp"


HANDLE hMainThread;
HMODULE g_hModule;

// Function to initialize and show the console window


void DestroyConsole()
{
    DestroyWindow(GetConsoleWindow());
    FreeConsole();
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


DWORD WINAPI Payload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    HMODULE dllfwd = dllforward::setup();
    if (dllfwd)
        CloseHandle(dllfwd);

    size_t selectedCheckbox = 0;
    char userInput;

    HMODULE moduleHandle = nullptr;
    GameData gData;

    do
    {
        moduleHandle = GetModuleHandle(_XOR_(L"game.dll"));
        Sleep(1000);
    } while (!moduleHandle);
    Sleep(100);

     //           if (!gData.InfHealth) // no need but its old code when activate using hotkey, but need to much hotkey for all feature
     //           {
     //               BYTE WriteHealthBytes[] =
     //               {
     //                   0x48, 0x85, 0xDB,                               // test rbx,rbx
     //                   0x74, 0x03,                                     // jz short @f
     //                   0x45, 0x89, 0x38,                               //  mov [r8],r15d
     //                                                                   // @@:
     //                   0x49, 0x8B, 0x84, 0xDE, 0x28, 0x04, 0x00, 0x00, // mov rax,[r14+rbx*8+00000428]
     //                   0x8B, 0x48, 0x10,                               // mov ecx,[rax+10]
     //                   0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_WriteHealth
     //               };

					//BYTE SetHealthBytes[] =
     //               {
     //                   0x48, 0x85, 0xD2,                                                       // test rdx,rdx
     //                   0x75, 0x0C,                                                             // jnz short @f
     //                   0x41, 0xC7, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, //   mov [r11+rcx*4+00004C28],#9999
     //                                                                                           // @@:
     //                   0x41, 0x8B, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,                         // mov eax,[r11+rcx*4+00004C28]
     //                   0x48, 0x8B, 0x5C, 0x24, 0x20,                                           // mov rbx,[rsp+20]
     //                   0x48, 0x8B, 0x74, 0x24, 0x28,                                           // mov rsi,[rsp+28]
     //                   0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_SetHealth
     //               };

     //               uintptr_t InfHealth = Memory::FindPattern(_XOR_("game.dll"), _XOR_("45 89 38 49 8B 84 DE 28 04 00 00"));
     //               LPVOID memory = Memory::AllocateMemory(InfHealth, sizeof(WriteHealthBytes));
     //               Memory::CreateTrampoline(InfHealth, memory);
     //               Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth + 14, WriteHealthBytes, Memory::ArrayLength(WriteHealthBytes));

     //               uintptr_t InfHealth1 = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 8B 84 8B 28 4C 00 00"));
     //               memory = Memory::AllocateMemory(InfHealth1, sizeof(SetHealthBytes));
     //               Memory::CreateTrampoline(InfHealth1, memory);
     //               Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth1 + 18, SetHealthBytes, Memory::ArrayLength(SetHealthBytes));

     //               gData.InfHealth = !gData.InfHealth;
     //               //create trampolin

     //           }
     
                if (gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 FF 08 4A 8B 84 ED"));
                    Memory::Nop((LPVOID)(GrenadesAddress), 3);
                    gData.InfGrenades = !gData.InfGrenades;

                }
           
                if (gData.InfGrenadesLegit && gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("4D 03 C6 41 FF 08"));
                    BYTE GrenadeBytes[] =
                    {
                        0x4D, 0x01, 0xF0,                               //add r8,r14
                        0x41, 0x83, 0x38, 0x01,                         //cmp dword ptr [r8],01
                        0x74, 0x03,                                     //je short @f
                        0x41, 0xFF, 0x08,                               // dec [r8]
                        0x4A, 0x8B, 0x84, 0xED, 0x30, 0x20, 0x00, 0x00, // mov rax,[rbp+r13*8+00002030]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Grenades
                    };
                    LPVOID memory = Memory::AllocateMemory(GrenadesAddress, sizeof(GrenadeBytes));
                    Memory::CreateTrampoline(GrenadesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, GrenadesAddress + 14, GrenadeBytes, Memory::ArrayLength(GrenadeBytes));
                    gData.InfGrenadesLegit = !gData.InfGrenadesLegit;

                }
            


                if (gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 83 2C C2 01 4D 8D 04 C2 49 8B 84 CA"));
                    BYTE AmmoPatch[] = { 0x00 };
                    Memory::Patch((LPVOID)(AmmoAddress + 4), AmmoPatch, 1);
                    gData.InfAmmo = !gData.InfAmmo;

                }
            

  
                if (gData.InfAmmoLegit && gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 83 2C C2 01"));
                    BYTE AmmoBytes[] =
                    {
                        0x41, 0x83, 0x3C, 0xC2, 0x02,                    // cmp dword ptr [r10+rax*8],#2
                        0x7E, 0x05,                                      // jle short @f
                        0x41, 0x83, 0x2C, 0xC2, 0x01,                    // sub dword ptr [r10+rax*8],01
                        0x4D, 0x8D, 0x04, 0xC2,			                 // lea r8,[r10+rax*8]
                        0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,  // mov rax,[r10+rcx*8+00002028]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_SubtractAmmo
                    };
                    LPVOID memory = Memory::AllocateMemory(AmmoAddress, sizeof(AmmoBytes));
                    Memory::CreateTrampoline(AmmoAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, AmmoAddress + 17, AmmoBytes, Memory::ArrayLength(AmmoBytes));
                    gData.InfAmmoLegit = !gData.InfAmmoLegit;

                }
            
                if (gData.InfSyringes)
                {
                    uintptr_t Syringes = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 FF CF 3B C2 74 61"));
                    Memory::Nop((LPVOID)(Syringes), 3);
                    gData.InfSyringes = !gData.InfSyringes;

                }
            


                if (gData.InfSyringesLegit && gData.InfSyringes)
                {
                    uintptr_t SyringesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 03 C9 45 8B BC CA C8 75 00 00"));
                    BYTE SyringesBytes[] =
                    {
                        0x48, 0x01, 0xC9,                                     // add rcx,rcx
                        0x45, 0x8B, 0xBC, 0xCA, 0xC8, 0x75, 0x00, 0x00,       // mov r15d,[r10+rcx*8+000075C8]
                        0x41, 0x83, 0xBC, 0xCA, 0xC8, 0x75, 0x00, 0x00, 0x01, // cmp dword ptr [r10+rcx*8+000075C8],01 { 1 }
                        0x74, 0x03,                                           // je short @f
                        0x41, 0xFF, 0xCF,                                     // dec r15d
                        0x39, 0xD0,                                           // cmp eax,edx
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Syringes
                    };
                    LPVOID memory = Memory::AllocateMemory(SyringesAddress, sizeof(SyringesBytes));
                    Memory::CreateTrampoline(SyringesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, SyringesAddress + 14, SyringesBytes, Memory::ArrayLength(SyringesBytes));
                    gData.InfSyringesLegit = !gData.InfSyringesLegit;

                }
            

                if (gData.InfStamina)
                {
                    uintptr_t Stamina = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 41 0F 11 08 8B 48 10 E8 ?? ?? ?? ?? 41 8B 47 48"));
                    BYTE StaminaPatch[] = { 0xF3, 0x41, 0x0F, 0x11, 0x30 };
                    Memory::Patch((LPVOID)(Stamina), StaminaPatch, 5);
                    gData.InfStamina = !gData.InfStamina;

                }
            


                if (gData.InfStratagems)
                {
                    uintptr_t Stratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 86 BF 01 00 00 0F"));
                    BYTE StratagemsPatch1[] = { 0x90, 0xE9 };
                    Memory::Patch((LPVOID)(Stratagems), StratagemsPatch1, 2);
                    gData.InfStratagems = !gData.InfStratagems;

                }
            

                if (gData.InfMissionTime)
                {
                    uintptr_t MissionTime = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 0F 5C C7 F3 41 0F 5F C5"));
                    Memory::Nop((LPVOID)(MissionTime), 4);
                    gData.InfMissionTime = !gData.InfMissionTime;

                }
            


                if (gData.NoReload)
                {
                    uintptr_t NoReload_v2 = Memory::FindPattern(_XOR_("game.dll"), _XOR_("FF 4C 87 04 83 FE FF 74 52 49 8B 00 F6 40 14 01"));
                    Memory::Nop((LPVOID)(NoReload_v2), 4);
                    //BYTE NoReloadByte[] =
                    //{
                    //    0x90,
                    //    0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,
                    //    0x8B, 0x48, 0x10,
                    //    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                    //    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    //};

                    //uintptr_t NoReload = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10"));

                    //LPVOID memory = Memory::AllocateMemory(NoReload, 0x100);
                    //Memory::CreateTrampoline(NoReload, memory);
                    //Memory::WriteAssemblyInstructions((uintptr_t)memory, NoReload + 14, NoReloadByte, Memory::ArrayLength(NoReloadByte));
                    //gData.NoReload = !gData.NoReload;

                }
            


                //if (!gData.MaxResources)
                //{
                //    BYTE MaxResourcesByte[] =
                //    {
                //        0x41, 0x81, 0x84, 0x8A, 0xEC, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                //        0x41, 0x81, 0x84, 0x8A, 0xF0, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                //        0x41, 0x81, 0x84, 0x8A, 0xF4, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                //        0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,
                //        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                //        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                //    };

                //    uintptr_t MaxResources = Memory::FindPattern(_XOR_("game.dll"), _XOR_("45 01 B4 8A EC 17 00 00"));

                //    LPVOID memory = Memory::AllocateMemory(MaxResources, 0x100);
                //    Memory::CreateTrampoline(MaxResources, memory);
                //    Memory::WriteAssemblyInstructions((uintptr_t)memory, MaxResources + 17, MaxResourcesByte, Memory::ArrayLength(MaxResourcesByte));
                //    gData.MaxResources = !gData.MaxResources;

                //}
            

                //if (!gData.Add5Resources && !gData.MaxResources)
                //{
                //    uintptr_t ResourcesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("45 01 B4 8A EC 17 00 00"));
                //    BYTE ResourcesBytes[] =
                //    {
                //        0x41, 0xBE, 0x05, 0x00, 0x00, 0x00,              // mov r14d, #5
                //        0x45, 0x01, 0xB4, 0x8A, 0xEC, 0x17, 0x00, 0x00,  // add [r10+rcx*4+000017EC],r14d
                //        0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,        // lea rax,[rdx+00000099]
                //        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Add5Resources
                //    };
                //    LPVOID memory = Memory::AllocateMemory(ResourcesAddress, sizeof(ResourcesBytes));
                //    Memory::CreateTrampoline(ResourcesAddress, memory);
                //    Memory::WriteAssemblyInstructions((uintptr_t)memory, ResourcesAddress + 15, ResourcesBytes, Memory::ArrayLength(ResourcesBytes));
                //    gData.Add5Resources = !gData.Add5Resources;

                //}
            

 
                if (gData.InstantRailGun)
                {
                    BYTE InstantRailGunByte[] =
                    {
                        0x43, 0xC7, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x40, // mov [r14+r15+00002C2C],403FFFFC { 3.00 }
                        0xF3, 0x43, 0x0F, 0x10, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00,             // movss xmm0,[r14+r15+00002C2C]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Railgun
                    };

                    uintptr_t InstantRailGun = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 43 0F 11 84 37 2C 2C 00 00"));

                    LPVOID memory = Memory::AllocateMemory(InstantRailGun, sizeof(InstantRailGunByte));
                    Memory::CreateTrampoline(InstantRailGun, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InstantRailGun + 14, InstantRailGunByte, Memory::ArrayLength(InstantRailGunByte));
                    gData.InstantRailGun = !gData.InstantRailGun;

                }
            


                if (gData.Speedhack)
                {
                    BYTE SpeedhackByte[] =
                    {
                        0x41, 0xC7, 0x46, 0x0C, 0x00, 0x00, 0xC0, 0x40,
                        0xF3, 0x41, 0x0F, 0x59, 0x56, 0x0C,
                        0xF3, 0x41, 0x0F, 0x59, 0x56, 0x10,
                        0x0F, 0x28, 0xE2,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Railgun
                    };

                    uintptr_t Speedhack = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 41 ?? ?? ?? ?? F3 41 ?? ?? ?? ?? 0F 28 ?? F3 0F ?? ?? 0F 5A"));

                    LPVOID memory = Memory::AllocateMemory(Speedhack, sizeof(SpeedhackByte));
                    Memory::CreateTrampoline(Speedhack, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, Speedhack + 15, SpeedhackByte, Memory::ArrayLength(SpeedhackByte));
                    gData.Speedhack = !gData.Speedhack;

                }
            

 
                if (gData.Recoil)
                {
                    BYTE RecoilByte[] =
                    {
                        0xEB
                    };

                    uintptr_t Recoil = Memory::FindPattern(_XOR_("game.dll"), _XOR_("44 8B 7C 24 ?? 41 3B 46 08"));
                    Memory::Patch((LPVOID)(Recoil+9), RecoilByte, 1);
                    gData.Recoil = !gData.Recoil;

                }
            


                if (gData.NoCannonOverheat)
                {
                    uintptr_t CannonOverheat = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 0F 11 09 4C 8B C1 49"));
                    Memory::Nop((LPVOID)(CannonOverheat), 4);
                    gData.NoCannonOverheat = !gData.NoCannonOverheat;

                }
            


                if (gData.InfSpecWeapon)
                {
                    uintptr_t SpecWeapon = Memory::FindPattern(_XOR_("game.dll"), _XOR_("44 89 7F 08 41 80 BC 24"));
                    Memory::Nop((LPVOID)(SpecWeapon), 4);
                    gData.InfSpecWeapon = !gData.InfSpecWeapon;

                }
            


                if (gData.NoStasTurretOverHeat)
                {
                    uintptr_t NoStasTurretOverHeat = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 42 0F 11 84 32 ?? ?? ?? ?? 8B 55 38 43 89 94 35"));
                    Memory::Nop((LPVOID)(NoStasTurretOverHeat), 10);
                    gData.NoStasTurretOverHeat = !gData.NoStasTurretOverHeat;

                }
            


                if (gData.JetpackNoCD)
                {
                    uintptr_t JetpackNoCD = Memory::FindPattern(_XOR_("game.dll"), _XOR_("8B 08 89 8C BE ?? ?? 00 00"));
                    Memory::Nop((LPVOID)(JetpackNoCD + 2), 7);
                    gData.JetpackNoCD = !gData.JetpackNoCD;

                }
            


                if (gData.ShieldNoCD)
                {
                    BYTE ShieldNoCDByte[] =
                    {
                        0xF3, 0x0F, 0x5C, 0xC9, 0x90
                    };

                    uintptr_t ShieldNoCD = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 41 0F 5C CA F3 0F 11 8C EE"));
                    Memory::Patch((LPVOID)(ShieldNoCD), ShieldNoCDByte, 5);
                    gData.ShieldNoCD = !gData.ShieldNoCD;

                }
            
            


                if (gData.InfBackpack)
                {
                    uintptr_t Backpack = Memory::FindPattern(_XOR_("game.dll"), _XOR_("2B C6 4D 8D 85 48 04 00 00"));
                    Memory::Nop((LPVOID)(Backpack), 2);
                    gData.InfBackpack = !gData.InfBackpack;

                }
            

     
                if (gData.ShowAllMapIcons)
                {
                    BYTE ShowAllMapIconsByte[] =
                    {
                        0xB8, 0x01, 0x00, 0x00, 0x00, 0x90
                    };

                    BYTE ShowAllMapIconsByte1[] =
                    {
                        0xF8
                    };

                    BYTE ShowAllMapIconsByte2n4[] =
                    {
                        0x90, 0xE9
                    };

                    BYTE ShowAllMapIconsByte3[] =
                    {
                        0xEB, 0x04
                    };

                    uintptr_t ShowAllMapIconsAddr = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 0F B6 44 97 23"));
                    uintptr_t aob_CheckIfAlienHivesAreObstructed = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 80 BE 3C BA 07 00 00"));
                    uintptr_t aob_CheckIfMinorInterestBlipIsDiscovered = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 ?? ?? ?? ?? 48 8B 44 24 ?? 80 78 29 00"));
                    uintptr_t aob_GetMinorInterestBlipIcon = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 84 ?? ?? ?? ?? 48 8B 4C 24 ?? F3 41 0F 10 4F"));
                    uintptr_t aob_CheckMissionBlip = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 59 02 00 00 49 8D"));
                     
                    Memory::Patch((LPVOID)(ShowAllMapIconsAddr), ShowAllMapIconsByte, 6);
                    Memory::Patch((LPVOID)(aob_CheckIfAlienHivesAreObstructed), ShowAllMapIconsByte1, 1);
                    Memory::Nop((LPVOID)(aob_CheckIfAlienHivesAreObstructed+1), 7);
                    Memory::Patch((LPVOID)(aob_CheckIfMinorInterestBlipIsDiscovered), ShowAllMapIconsByte2n4, 2);
                    Memory::Patch((LPVOID)(aob_GetMinorInterestBlipIcon), ShowAllMapIconsByte3, 2);
                    Memory::Patch((LPVOID)(aob_CheckMissionBlip), ShowAllMapIconsByte2n4, 2);
                    gData.ShowAllMapIcons = !gData.ShowAllMapIcons;

                }
            

                if (gData.AllStratagems)
                {
                    BYTE AllStratagemsByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllStratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 89 5C 24 ?? 48 8B D9 85 D2 75 09"));
                    Memory::Patch((LPVOID)(AllStratagems), AllStratagemsByte, 3);
                    gData.AllStratagems = !gData.AllStratagems;

                }
            


                if (gData.AllEquipment)
                {
                    BYTE AllEquipmentByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllEquipment = Memory::FindPattern(_XOR_("game.dll"), _XOR_("83 B9 ?? ?? ?? ?? ?? 75 ?? 85 D2 74 ?? 44 8B 89 ?? ?? ?? ?? 45 33 C0 45 85 C9 74 ?? 48 8D 81 ?? ?? ?? ?? 39 50 ?? 74 ?? 41 FF C0 48 83 C0 ?? 45 3B C1 72 ?? 32 C0 C3 8B 00 48 69 C8"));
                    Memory::Patch((LPVOID)(AllEquipment+11), AllEquipmentByte, 3);
                    gData.AllEquipment = !gData.AllEquipment;

                }
            

                if (gData.AllArmor)
                {
                    BYTE AllArmorByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllArmor = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 83 EC ?? 44 8B 49 ?? 45 33 C0"));
                    Memory::Patch((LPVOID)(AllArmor), AllArmorByte, 3);
                    gData.AllArmor = !gData.AllArmor;

                }
            

           
       

    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

