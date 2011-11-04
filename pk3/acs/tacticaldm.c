#include "zcommon.acs"

#library "tacticaldm"

#include "tactFunctions.h"
#include "tactConstants.h"

#define classCount 4

// I'll ignore the 80-char line thing just for this

int classNames[classCount]   = {"Soldier",        "Shotgunner",        "Auto-Soldier",       "Chaingunner"};
int classImgs[classCount]    = {"SOLDIERD",       "SSGERD",            "SOLDIERD",           "SOLDIERD"};
int classPacks[classCount]   = {"SoldierPack",    "ShotgunnerPack",    "AutoSoldierPack",    "ChaingunnerPack"};
int botPacks[classCount]     = {"SoldierPackBot", "ShotgunnerPackBot", "AutoSoldierPackBot", "ChaingunnerPackBot"};
int classHealths[classCount] = {100,              90,                  100,                  140};
int classSpeeds[classCount]  = {0.9,              1.0,                 0.85,                 0.6};
int classJumpZs[classCount]  = {7.0,              8.0,                 7.0,                  6.0};
int classCosts[classCount]   = {0,                500,                 750,                  3000};


int teamNames[5] = {"Blue", "Red", "Green", "Gold", "No"};

int teamCash[5];
int teamNewCash[5][5];

int classDescs[classCount]   = {
"Well, at least this is free. Being the grunt of the army, you don't really get\n\
much in the way of benefits. You get an assault rifle, a pistol, some grenades,\n\
and that's it. You can't even take buildings down, for christ's sake! No one\n\
would really want to be this guy when the war starts raging, but at the\n\
beginning, you have to choke the shit down and get over it.\n\
\n\
At least it kills infantry well enough.",

"You just came straight from a deathmatch, didn't you. Well, you'll feel right\n\
at home with this guy - he has both shotguns and you probably won't be running\n\
out of ammo any time soon. However, he's a glass cannon, with only 90 health.\n\n\
As for those buildings, who cares about them? Most certainly not you - you just\n\
want to kill everything! As a result, you have no C4's on you, meaning you can't\n\
effectively take down a building - unless you think shooting at it with your\n\
shotguns will do much damage. In which case, have fun.\n\
However, after a while, you may have the deepest and healthiest respect for your\n\
shotgun out of every object in the universe. Don't go crazy, though.",

"So, what's the difference between this guy and the normal soldier? Well,\n\
besides the fact that he has an automatic assault rifle, and that you go a bit\n\
slower, not much at all.\n\
Now, you'd think an automatic assault rifle would be better, right? Well...\n\
sorta, depending on who you want to believe. Obviously, it's automatic, but as\n\
a result, its accuracy is much worse compared to its semi-auto brethren. You\n\
can hold more ammo for it, but it doesn't pack as much as a punch as the\n\
other's rounds. You do fire faster, though, but is it really enough to\n\
compensate for it?\n\
Yes, probably, but hey.",

"Hey, chaingun! The hell with respect! You are made of solid meat, and have a\n\
hefty 140 HP to stay up, although your running capabilities are kinda... bad.\n\
You won't need to do much running, though, since you have a heavy chaingun that\n\
can RIP AND TEAR through your enemies pretty effortlessly. Accuracy is only a\n\
problem at long ranges. Shame about the cost, though."};

global int 5:playerClassNums[];
int playerTeams[32];

function int isCooperative(void)
{
    int ret = (GetCVar("teamplay") == false) && (GetCVar("ctf") == false) &&
                (GetCVar("deathmatch") == false);

    return ret;
}


// hudmessage(text; HUDMSG_FADEOUT, id, color, (float)x, (float)y, (float)holdTime, (float)fadetime);


script TACDM_AUTO_COMMON (void)
{
    int pln = PlayerNumber();
    playerTeams[pln] = GetPlayerInfo(pln, PlAYERINFO_TEAM);

    if (PlayerIsBot(pln))
    {
        GiveInventory("IsBot", 1);
    }
}


script TACDM_AUTO_OPEN open
{
    int i;

    if (GetCVar("tacdm_varsexist") != 1)
    {
        ConsoleCommand("set tacdm_varsexist        1");
        ConsoleCommand("set tacdm_startcash        3000");
        ConsoleCommand("set tacdm_moneyperkill     125");
        ConsoleCommand("set tacdm_moneylostondeath 125");
        ConsoleCommand("archivecvar tacdm_varsexist");
        ConsoleCommand("archivecvar tacdm_startcash");
        ConsoleCommand("archivecvar tacdm_moneyperkill");
        ConsoleCommand("archivecvar tacdm_moneylostondeath");
        Log(s:"For any server hosts:\nCVars (all preceded by \"tacdm_\": startcash moneyperkill moneylostondeath");
    }

    if (!isCooperative())
    {
        for (i = 0; i < 5; i++)
        {
            teamCash[i] += GetCVar("tacdm_startcash");
        }
    }
}

script TACDM_AUTO_ENTER enter
{
    int pln    = PlayerNumber();
    int plc    = playerClassNums[pln];
    int isCoop = isCooperative();
    int choice = !isCoop || (plc == 0);

    ACS_ExecuteAlways(TACDM_AUTO_COMMON, 0, 0,0,0);

    if (choice == 1)
    {
        ACS_ExecuteAlways(TACDM_CHOOSECLASS, 0, 0,0,0);
    }

    ACS_ExecuteAlways(TACDM_HUD, 0, 0,0,0);
}


script TACDM_AUTO_RESPAWN respawn
{
    int pln    = PlayerNumber();
    int plc    = playerClassNums[pln];
    int bDown = GetPlayerInput(-1, INPUT_BUTTONS);

    ACS_ExecuteAlways(TACDM_AUTO_COMMON, 0, 0,0,0);

    if ((bDown & BT_ALTATTACK) || (plc == 0))
    {
        ACS_ExecuteAlways(TACDM_CHOOSECLASS,  0, plc,0,0);

        if (plc == 0)   // assume TACDM_AUTO_ENTER wasn't called
        {
            ACS_ExecuteAlways(TACDM_HUD, 0, 0,0,0);
        }
    }
    else
    {
        ACS_ExecuteAlways(TACDM_SETUPCLASS,  0, 0,0,0);
    }
}


script TACDM_AUTO_DEATH death
{
    SetHudSize(640, 480, 1);

    SetFont("SMALLFONT");
    HudMessage(s:"Hold \cf", k:"+altattack", s:"\cd when respawning to choose a new class";
    HUDMSG_FADEOUT | HUDMSG_COLORSTRING,  TACDM_CLASSPRINTOFFSET+1, "Green", 320.4, 460.1, 1.0, 2.0);
}

script TACDM_AUTO_DISCONNECT (int pln) disconnect
{
    playerClassNums[pln] = 0;
    playerTeams[pln] = -1;
}








script TACDM_CHOOSECLASS (int lastClass)
{
    int i;
    int choseClass;
    int classScrolled;
    int bDown; int bLast; int bOld; int bHeld; int bPressed; int bReleased;
    int firstGo = 1;

    int pln = PlayerNumber();
    int team = GetPlayerInfo(pln, PLAYERINFO_TEAM);
    int flash;

    ClearInventory();
    GiveInventory("SpawnProtection", 1);

    if (PlayerIsBot(pln))
    {
        classScrolled = Random(0, classCount-1);

        while (classCosts[classScrolled] > teamCash[team])
        {
            classScrolled = Random(0, classCount-1);
        }
    }
    else
    {
        while (PlayerInGame(pln))
        {
            flash = 0;
            bLast      = bDown;
            bDown      = GetPlayerInput(-1, INPUT_BUTTONS);
            bPressed   = (bDown ^ bLast) & bDown;
            // *  unnecessary for now
            // ~ bHeld      = bLast & bOld;
            // ~ bReleased  = (bDown ^ bLast) & bOld;

            if ((bPressed & BT_MOVERIGHT) || (bPressed & BT_BACK) || (bPressed & BT_ALTATTACK))
            {
                flash = -1;
                classScrolled = (classScrolled + 1) % classCount;
            }

            if ((bPressed & BT_MOVELEFT) || (bPressed & BT_FORWARD))
            {
                flash = -1;
                if (classScrolled == 0)
                {
                    classScrolled = classCount-1;
                }
                else
                {
                    classScrolled--;
                }
            }

            if ((firstGo == 0) && ((bPressed & BT_ATTACK) || (bPressed & BT_USE)))
            {
                if (classCosts[classScrolled] > teamCash[team])
                {
                    flash = 1;
                }
                else
                {
                    break;
                }
            }

            ACS_ExecuteAlways(TACDM_PRINTCLASSDESC, 0, classScrolled, flash, teamCash[team]);
            Delay(1);
            firstGo = 0;
        }
    }

    playerTeams[pln] = team;
    playerClassNums[pln] = classScrolled + 1;

    if (classScrolled + 1 != lastClass)
    {
        ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, -classCosts[classScrolled], 0);
    }

    ACS_ExecuteAlways(TACDM_SETUPCLASS,  0, 0,0,0);

    GiveInventory("EndSpawnProtection", 1);
}



script TACDM_PRINTCLASSDESC (int whichClass, int flashCost, int moneyAmount) clientside
{
    int i; int c;

    SetHudSize(320, 240, 1);

    SetFont("BIGFONT");
    HudMessage(s:classNames[whichClass]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
    TACDM_CLASSPRINTOFFSET, "Brick", 160.4, 12.1, 3.0, 2.0);

    SetHudSize(1024, 768, 1);

    SetFont(classImgs[whichClass]);
    HudMessage(s:"A"; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
    TACDM_CLASSPRINTOFFSET-1, "Untranslated", 0.1, 0.1, 1.0, 2.0);


    SetHudSize(640, 480, 1);

    SetFont("SMALLFONT");

    if (flashCost == -1)
    {
        HudMessage(s:""; HUDMSG_FADEOUT, TACDM_CLASSPRINTOFFSET-3, 0, 320.4, 75.1, 1.0, 2.0);
    }

    if (flashCost == 1)
    {
        HudMessage(s:"Cost: \cg$", d:classCosts[whichClass]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
        TACDM_CLASSPRINTOFFSET-3, "White", 320.4, 75.1, 1.0, 2.0);
    }
    else
    {
        HudMessage(s:"Cost: \cd$", d:classCosts[whichClass]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
        TACDM_CLASSPRINTOFFSET-2, "White", 320.4, 75.1, 1.0, 2.0);
    }

    HudMessage(s:classDescs[whichClass]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
    TACDM_CLASSPRINTOFFSET-4, "White", 320.4, 100.1, 1.0, 2.0);

    SetFont("SMALLFONT"); // yeah, these next lines run over D:
    HudMessage(s:"Press \cf", k:"+moveleft", s:"\cd or \cf", k:"+forward", s:"\cd to scroll up,",
               s:" \cf", k:"+moveright", s:"\cd, \cf", k:"+back", s:"\cd, or \cf", k:"+altattack", s:"\cd to scroll down,",
               s:" and \cf", k:"+attack", s:"\cd or \cf", k:"+use", s:"\cd to select";
    HUDMSG_FADEOUT | HUDMSG_COLORSTRING,  TACDM_CLASSPRINTOFFSET+1, "Green", 320.4, 440.1, 1.0, 2.0);

    for (i = 0; i < 5; i++)
    {
        c = whichClass + (i - 2);

        while (c < 0)
        {
            c += classCount;
        }

        while (c >= classCount)
        {
            c -= classCount;
        }

        if (i == 2)
        {
            if (classCosts[c] > moneyAmount)
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Red", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
            else
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Green", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
        }
        else
        {
            if (classCosts[c] > moneyAmount)
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Dark Red", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
            else
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Dark Green", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
        }
    }
}


script TACDM_SETUPCLASS (void)
{
    int pln = PlayerNumber();
    int classNum = playerClassNums[pln] - 1;

    ClearInventory();

    if (PlayerIsBot(pln))
    {
        GiveInventory(botPacks[classNum], 1);
    }
    else
    {
        GiveInventory(classPacks[classNum], 1);
    }

    SetActorProperty(0, APROP_HEALTH,      classHealths[classNum]);
    SetActorProperty(0, APROP_SPAWNHEALTH, classHealths[classNum]);
    SetActorProperty(0, APROP_SPEED,       classSpeeds[classNum]);
    SetActorProperty(0, APROP_JUMPZ,       classJumpZs[classNum]);
}


script TACDM_HUD (void)
{
    int i; int inc;
    int spacing = 10;
    int pln = PlayerNumber();
    int team;

    SetHudSize(640, 480, 1);

    while (PlayerInGame(pln))
    {
        team = playerTeams[pln];
        HudMessage(s:teamNames[team], s:" team's Cash: \cd$", d:teamCash[team]; HUDMSG_PLAIN | HUDMSG_COLORSTRING,
        TACDM_HUDPRINTOFFSET, teamNames[team], 630.2, 10.1, 1.0);

        for (i = 0; i < 5; i++)
        {
            if (teamNewCash[team][i] > 0)
            {
                inc = (i * spacing) << 16;
                HudMessage(s:"+$", d:teamNewCash[team][i]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_HUDPRINTOFFSET+1+i, "Green", 625.2, 25.1+inc, 0.1, 0.25);
            }
            else  if (teamNewCash[team][i] < 0)
            {
                inc = (i * spacing) << 16;
                HudMessage(s:"-$", d:-teamNewCash[team][i]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_HUDPRINTOFFSET+1+i, "Red", 625.2, 25.1+inc, 0.1, 0.25);
            }
        }

        Delay(1);
    }
}


script TACDM_ADDMONEY (int team, int amount, int maxAmount)
{
    int emptySlot;
    int addAmount;
    int addGap;

    if (amount == 0)
    {
        amount = GetCVar("tacdm_moneyperkill");
    }

    if (maxAmount == 0)
    {
        maxAmount = 0x7FFFFFFF;
    }

    addAmount = teamCash[team]+amount;
    addGap    = addAmount - min(addAmount, maxAmount);
    addAmount = amount - addGap;

    ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, addAmount, 0);
}


script TACDM_TAKEMONEY (int team, int amount, int minAmount)
{
    int emptySlot;
    int takeAmount;
    int takeGap;

    if (amount == 0)
    {
        amount = GetCVar("tacdm_moneylostondeath");
    }

    if (!minAmount)
    {
        minAmount = 0;
    }

    takeAmount = teamCash[team]-amount;
    takeGap    = max(takeAmount, minAmount) - takeAmount;
    takeAmount = -(amount - takeGap);

    ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, takeAmount, 0);
}


script TACDM_PAYFORKILL (void)
{
    int killedPln = PlayerNumber();
    int killedTeam = playerTeams[killedPln];

    SetActivatorToTarget(0);

    int killerPln = PlayerNumber();
    int killerTeam = playerTeams[killerPln];

    if (killedTeam == killerTeam)
    {
        if ((killedTeam == 4) && (killedPln != killerPln))
        {
            ACS_ExecuteAlways(TACDM_ADDMONEY, 0, 4, 0, 0);
        }
        else
        {
            ACS_ExecuteAlways(TACDM_TAKEMONEY, 0, killerTeam, 0, 0);
        }
    }
    else
    {
        ACS_ExecuteAlways(TACDM_ADDMONEY, 0, killerTeam, 0, 0);
    }
}

script TACDM_INTERNAL_CHANGEMONEY (int team, int amount)
{
    int i; int emptySlot;

    teamCash[team] += amount;

    for (i = 0; i < 5; i++)
    {
        if (teamNewCash[team][i] == 0)
        {
            emptySlot = i;
            break;
        }
    }

    teamNewCash[team][emptySlot] += amount;

    Delay(70);

    teamNewCash[team][emptySlot] -= amount;
}
