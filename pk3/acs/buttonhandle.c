#include "zcommon.acs"
#library "buttonhandle"


int dead[32];
int inGame[32];

script 559 OPEN
{
    if (GetCVar("compat_clientssendfullbuttoninfo") == 0)
    {
        ConsoleCommand("compat_clientssendfullbuttoninfo 1");
        Log(s:"compat_clientssendfullbuttoninfo must be on for the mod to work properly");
    }
}

script 560 (void)   /* generic button signaler */
{
    int pln = PlayerNumber();
    int buttons;
    while (!dead[pln] && inGame[pln])
    {
        buttons = GetPlayerInput(-1, MODINPUT_BUTTONS);
        if (buttons & BT_ATTACK)
        {
            GiveInventory("MainFired", 1);
        }
        else
        {
            TakeInventory("MainFired", 16);
        }

        if (buttons & BT_ALTATTACK)
        {
            GiveInventory("AltFired", 1);
        }
        else
        {
            TakeInventory("AltFired", 16);
        }

        Delay(1);
    }
}

script 561 ENTER
{
    inGame[PlayerNumber()] = 1;
    dead[PlayerNumber()] = 0;
    ACS_ExecuteAlways(560, 0, 0,0,0);
}

script 562 DEATH
{
    inGame[PlayerNumber()] = 1;
    dead[PlayerNumber()] = 1;
}

script 563 (int pln) DISCONNECT
{
    dead[PlayerNumber()] = 0;
    inGame[PlayerNumber()] = 0;
}

script 564 RESPAWN
{
    ACS_ExecuteAlways(561, 0, 0,0,0);
}
