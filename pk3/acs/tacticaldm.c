#include "zcommon.acs"

#library "tacticaldm"

#include "tactFunctions.h"
#include "tactConstants.h"

#define classCount 8

// I'll ignore the 80-char line thing just for this

int classNames[classCount]   = {"Soldier",        "Scout",        "Shotgunner",        "Auto-Soldier",       "Chaingunner",        "Rocketeer",        "Minigunner",        "Det-Man"};
int classImgs[classCount]    = {"SOLDIERD",       "SCOUTD",       "SSGERD",            "SOLDIERD",           "SOLDIERD",           "SSGERD",           "SOLDIERD",          "SSGERD"};
int classPacks[classCount]   = {"SoldierPack",    "ScoutPack",    "ShotgunnerPack",    "AutoSoldierPack",    "ChaingunnerPack",    "RocketeerPack",    "MinigunnerPack",    "DetManPack"};
int botPacks[classCount]     = {"SoldierPackBot", "ScoutPackBot", "ShotgunnerPackBot", "AutoSoldierPackBot", "ChaingunnerPackBot", "RocketeerPackBot", "MinigunnerPackBot", "DetManPackBot"};
int classHealths[classCount] = {100,              85,             90,                  100,                  140,                  100,                140,                 100};
int classSpeeds[classCount]  = {0.9,              1.1,            1.0,                 0.85,                 0.6,                  1.0,                0.6,                 1.0};
int classJumpZs[classCount]  = {7.0,              8.0,            8.0,                 7.0,                  6.0,                  8.0,                6.0,                 8.0};
int classCosts[classCount]   = {0,                0,              500,                 500,                  5000,                 5000,               20000,               20000};


int teamNames[5] = {"Blue", "Red", "Green", "Gold", "No"};

int canSwitch[32];
int teamNewCash[5][5];
int teamNewCashReason[5][5];

int classDescs[classCount]   = {
"Well, at least this is free. Being the grunt of the army, you don't really get\n\
much in the way of benefits. You get an assault rifle, a pistol, some grenades,\n\
and that's it. You can't even take buildings down, for christ's sake! No one\n\
would really want to be this guy when the war starts raging, but at the\n\
beginning, you have to take a bullet for the team. Or multiple. You might not\n\
be the only one doing this either..\n\
\n\
At least it kills infantry well enough. Even that's a bit lacking, though.",

"Pestering is your job, and for you, your life. All you do is run around, taking\n\
potshots at your enemies before they can react. You run /really/ light, not even\n\
holding any grenades on you by default - only a pistol and shotgun for you!\n\
Luckily, this means you can run faster than most of the people you'll be fighting - \n\
the people who can run faster than you are also the sort of people who have wide-\n\
angle laser beams for fingers, and you really shouldn't fight them, unless you\n\
have a fetish for \caDYING ALL THE TIME\c-.",

"You just came straight from a deathmatch, didn't you. Well, you'll feel right\n\
at home with this guy - he has both shotguns and you probably won't be running\n\
out of ammo any time soon. However, he's a glass cannon, with only 90 health.\n\
His primary weapon, the SSG, also can't fire long-range worth crap. If your target\n\
is far away, it's probably just a good idea to hide - they most likely CAN hit you\n\
from long range.\n\
However, after a while, you may have the deepest and healthiest respect for your\n\
shotgun out of every object in the universe. Don't go crazy, though.",

"Other than the fact that you have an automatic assault rifle, this guy isn't\n\
really different from a soldier at all. It's a good enough reason, though. While\n\
primary fire isn't anywhere near as accurate as the normal assault rifle, you\n\
have to remember - \caIT'S AUTOMATIC.\c- Since medium range will still have most\n\
of your bullets hit, you'll still take down targets faster than the normal soldier.\n\
Considering the game you're playing doesn't really have long-range... yeah. If you\n\
do need accuracy, the burst fire on this thing is more accurate than on the soldier.\n\
\n\
In case you haven't guessed, you're the all-arounder. Just take it easy.",

"Hey, chaingun! The hell with respect! You are made of solid meat, and have a\n\
hefty 140 HP to stay up, although your running capabilities are kinda... bad.\n\
You won't need to do much running, though, since you have a heavy chaingun that\n\
can RIP AND TEAR through your enemies pretty effortlessly. Accuracy is only a\n\
problem at long ranges. Shame about the cost, though.\n\
Oh, and you take explosions harder. That grenade bouncing around the corner?",

"Why settle with killing them? Coming straight from the realm of Quake, you\n\
have a rocket launcher and you're not afraid to use it. Unfortunately, you lost\n\
your lightning gun somewhere, and found a pistol on the road. You had the\n\
foresight to at least bring a shotgun with you, as backup.\n\
Now you're in this weird land, where there's chainguns that mince people up in\n\
less than a second, and super shotguns that take more than about a second to \n\
fire. But your rocket launcher is more powerful, and damages you less!\n\
\n\
Yeah, they're pretty much screwed.\n\
One thing though. Bullets... kinda hurt a helluvalot more. 50% more. Beware\n\
of chainguns.",

"Hey, minigun! The hell with chainguns! You might as well be a chaingunner,\n\
except that you have a minigun and shotgun, instead of a chaingun. Is that\n\
worth a $2000 price increase?\n\
\cgHELL YEAH IT IS.\c-\n\
The minigun, at the very least, fires 50% faster than the chaingun, and at full\n\
auto, fires three times as fast as it, and does the same damage-per-bullet. The\n\
only thing is, the minigun is /big/. That thing firing will slow you down to no\n\
end. If you're chain-firing, kiss mobility goodbye.\n\
But hey, everything you see pretty much dies instantly. It's what you don't see...\n\
Be very wary of explosives. The sitting duck tends to attract them.",

"Things. Must. \caEXPLODE.\c- That's your motto, and you stick to it. With enough\n\
money to get armor that protects against explosions without letting bullet in,\n\
you are a force to be reckoned with. A grenade and rocket launcher are at your\n\
disposal, along with a shotgun that fires faster than the normal shotgun, and does\n\
slightly more damage! Your mobility is the same as the rocketeer's, and you're\n\
better than him in every way. Oh, and you can carry more explosives. Now the rocket\n\
launcher's alt-fire might actually be practical...",};

global int 4:teamCash[];
global int 5:playerClassNums[];
int playerTeams[32];

function int isCooperative(void)
{
    int ret = (GetCVar("teamplay") == false) && (GetCVar("ctf") == false) &&
                (GetCVar("deathmatch") == false);

    return ret;
}

function int getTeam(int pln)
{
    int team = playerTeams[pln];

    if (team == 0)
    {
        return 4;
    }

    return team - 1;
}

function int getClass(int pln)
{
    return playerClassNums[pln] - 1;
}

function int getTeamCash(int team)
{
    return teamCash[team*2];
}

function void setTeamCash(int team, int amount)
{
    teamCash[team*2] = max(0, amount);
}

function void changeTeamCash(int team, int amount)
{
    teamCash[team*2] = max(0, teamCash[team*2]+amount);
}


// hudmessage(text; HUDMSG_FADEOUT, id, color, (float)x, (float)y, (float)holdTime, (float)fadetime);


script TACDM_AUTO_COMMON (void)
{
    int pln = PlayerNumber();
    int plc = playerClassNums[pln];
    playerTeams[pln] = GetPlayerInfo(pln, PlAYERINFO_TEAM) + 1;

    TakeInventory("ClassSwitcherNoInvuln", 1);

    if (PlayerIsBot(pln))
    {
        GiveInventory("IsBot", 1);
    }
    else if (plc == 0)   // assume TACDM_AUTO_ENTER wasn't called
    {
        ACS_ExecuteAlways(TACDM_HUD, 0, 0,0,0);
    }
}


script TACDM_AUTO_OPEN open
{
    int i;

    if (GetCVar("tacdm_varsexist") != 3)
    {
        ConsoleCommand("set tacdm_varsexist        3");
        ConsoleCommand("set tacdm_startcash        3000");
        ConsoleCommand("set tacdm_moneyperkill     50");
        ConsoleCommand("set tacdm_moneylostondeath 50");
        ConsoleCommand("set tacdm_monstermult      2");

        ConsoleCommand("archivecvar tacdm_varsexist");
        ConsoleCommand("archivecvar tacdm_startcash");
        ConsoleCommand("archivecvar tacdm_moneyperkill");
        ConsoleCommand("archivecvar tacdm_moneylostondeath");
        ConsoleCommand("archivecvar tacdm_monstermult");

        Log(s:"For any server hosts:\nCVars (all preceded by \"tacdm_\": startcash moneyperkill moneylostondeath monstermult");
    }

    for (i = 0; i < 5; i++)
    {
        if (isCooperative())
        {
            if (teamCash[(i*2)+1] == 1)
            {
                continue;
            }
            else
            {
                teamCash[(i*2)+1] = 1;
            }
        }

        setTeamCash(i, GetCVar("tacdm_startcash"));
    }
}


script TACDM_AUTO_OPEN_CLIENT open clientside
{
    if (GetCVar("tacdm_client_varsexist") != 1)
    {
        ConsoleCommand("set tacdm_client_varsexist   1");
        ConsoleCommand("set tacdm_client_showclasses 1");

        ConsoleCommand("archivecvar tacdm_client_varsexist");
        ConsoleCommand("archivecvar tacdm_client_showclasses");

        Log(s:"CVars (all preceded by \"tacdm_client_\": showclasses");
    }
}

script TACDM_AUTO_ENTER enter
{
    int pln    = PlayerNumber();
    int plc    = playerClassNums[pln];
    int isCoop = isCooperative();

    ACS_ExecuteAlways(TACDM_AUTO_COMMON, 0, 0,0,0);

    if (!isCoop || plc == 0)
    {
        ACS_ExecuteAlways(TACDM_CHOOSECLASS, 0, 0,0,0);
    }
}


script TACDM_AUTO_RESPAWN respawn
{
    int pln    = PlayerNumber();
    int plc    = playerClassNums[pln];
    int bDown = GetPlayerInput(-1, INPUT_BUTTONS);

    ACS_ExecuteAlways(TACDM_AUTO_COMMON, 0, 0,0,0);

    if ((bDown & BT_ALTATTACK) || (plc == 0) || (PlayerIsBot(pln) && random(GetCVar("skill"), 4) == 4))
    {
        ACS_ExecuteAlways(TACDM_CHOOSECLASS,  0, plc,0,0);
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
    int team = getTeam(pln);

    ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, classCosts[playerClassNums[pln] - 1],  REASON_SOLDCLASS);

    playerClassNums[pln] = 0;
    playerTeams[pln] = 0;
}







script TACDM_SCALESPEED (int argInt, int argFrac, int which)
{
    int arg = (argInt << 16) + ((argFrac << 16) / 100);

    SetActorProperty(0, APROP_Speed, arg);
}

script TACDM_ITEM_CLASS (int noProtect)
{
    int plc = playerClassNums[PlayerNumber()];

    ACS_ExecuteAlways(TACDM_CHOOSECLASS, 0, plc, noProtect);
}



script TACDM_CHOOSECLASS (int lastClass, int noProtect)
{
    int i; int j;
    int choseClass;
    int bDown; int bLast; int bOld; int bHeld; int bPressed; int bReleased;
    int firstGo = 1;

    int pln = PlayerNumber();
    int myClass = playerClassNums[pln];
    int team = GetPlayerInfo(pln, PLAYERINFO_TEAM);
    int classScrolled = max(0, lastClass - 1);
    int flash; int noSelect;

    ClearInventory();

    GiveInventory("SpawnStop", 1);
    if (!noProtect)
    {
        GiveInventory("SpawnProtection", 1);
    }

    if (PlayerIsBot(pln))
    {
        classScrolled = classCount-1;
        while (classCosts[classScrolled] > getTeamCash(team))
        {
            classScrolled--;

            if (classCosts[myClass - 1] > classCosts[classScrolled])
            {
                classScrolled = max(0, lastClass - 1);  // last ditch as soldier
                noSelect = 1;
                break;
            }
        }

        if (!noSelect)
        {
        i = classCosts[classScrolled];

            for (j = classScrolled - 1; j > 0; j--)
            {
                if (classCosts[j] != i)
                {
                     break;
                }
            }
            classScrolled = random(j, classScrolled);
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
                if (classScrolled == lastClass - 1)
                {
                    break;
                }

                if (classCosts[classScrolled] > getTeamCash(team))
                {
                    flash = 1;
                }
                else
                {
                    break;
                }
            }

            ACS_ExecuteAlways(TACDM_PRINTCLASSDESC, 0, (lastClass << 16) + classScrolled, flash, getTeamCash(team));
            Delay(1);
            firstGo = 0;
        }
    }

    playerTeams[pln] = team + 1;
    playerClassNums[pln] = classScrolled + 1;

    if (classScrolled + 1 != lastClass)
    {
        if (lastClass != 0)
        {
            ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, classCosts[lastClass - 1],  REASON_SOLDCLASS);
        }

        ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, -classCosts[classScrolled], REASON_BOUGHTCLASS);
    }

    ACS_ExecuteAlways(TACDM_SETUPCLASS,  0, 0,0,0);

    GiveInventory("EndSpawnProtection", 1);
}



script TACDM_PRINTCLASSDESC (int whichClass, int flashCost, int moneyAmount) clientside
{
    int i; int c;

    int lastClass = whichClass >> 16;
    whichClass    = whichClass & 65535;

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
            if (c == (lastClass - 1))
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Gold", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
            else if (classCosts[c] > moneyAmount)
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
            if (c == (lastClass - 1))
            {
                HudMessage(s:classNames[c]; HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
                TACDM_CLASSPRINTOFFSET-5-i, "Yellow", 60.4, 20.1+((i*TACDM_NAMESPACING) << 16), 1.0, 0.5);
            }
            else if (classCosts[c] > moneyAmount)
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
    int tic;
    int i;
    int pln = PlayerNumber();
    int newCash; int reason; int color;

    int team    = getTeam(pln);
    int oldTeam = team;
    int cash    = getTeamCash(team);
    int oldCash = cash;


    while (PlayerInGame(pln))
    {
        team = getTeam(pln);
        cash = getTeamCash(team);

        if (oldCash != cash || oldTeam != team || !(tic % HUD_UPDATERATE))
        {
            tic = 0;


            ACS_ExecuteAlways(TACDM_CLIENT_PRINTMONEY, 0, team, cash);

            for (i = 0; i < 5; i++)
            {
                newCash = teamNewCash[team][i];
                reason  = teamNewCashReason[team][i];
                color   = reasonColors[reason];

                ACS_ExecuteAlways(TACDM_CLIENT_PRINTNEWMONEY, 0, i, newCash, color);
            }

            for (i = 0; i < classCount; i++)
            {
                ACS_ExecuteAlways(TACDM_CLIENT_PRINTCLASSES, 0, i, cash, classCosts[i]);
            }
        }

        oldTeam = team;
        oldCash = cash;
        tic++;

        Delay(1);
    }
}

script TACDM_CLIENT_PRINTMONEY (int team, int cash) clientside
{
    int name = teamNames[team];
    int rate = ((HUD_UPDATERATE << 16) / 35) + 0.5;

    SetHudSize(HUD_ASPECT1_W, HUD_ASPECT1_H, 1);

    HudMessage(s:name, s:" team's Cash: \cd$", d:cash;
        HUDMSG_PLAIN | HUDMSG_COLORSTRING,
        TACDM_HUDPRINTOFFSET, name,
        HUD_TEAMMONEY_X, HUD_TEAMMONEY_Y, rate);
}


script TACDM_CLIENT_PRINTNEWMONEY (int i, int value, int color) clientside
{
    int incX; int incY;
    SetHudSize(HUD_ASPECT1_W, HUD_ASPECT1_H, 1);

    if (value > 0)
    {
        incY = (i * HUD_SPACING_Y) << 16;
        HudMessage(s:"+$", d:value;
            HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
            TACDM_HUDPRINTOFFSET+1+i, color,
            HUD_NEWMONEY_X, HUD_NEWMONEY_Y+incY, 2.0, 0.25);
    }
    else if (value < 0)
    {
        incY = (i * HUD_SPACING_Y) << 16;
        HudMessage(s:"-$", d:-value;
            HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
            TACDM_HUDPRINTOFFSET+1+i, color,
            HUD_NEWMONEY_X, HUD_NEWMONEY_Y+incY, 2.0, 0.25);
    }
}

script TACDM_CLIENT_PRINTCLASSES (int i, int cash, int cost) clientside
{
    if (GetCVar("tacdm_client_showclasses") == 0)
    {
        terminate;
    }

    int incX; int incY;
    int rate = ((HUD_UPDATERATE << 16) / 35) + 0.5;
    SetHudSize(HUD_ASPECT2_W, HUD_ASPECT2_H, 1);

    if (cost == 0)
    {
        incX = ((i / HUD_MAX) * HUD_SPACING_X) << 16;
        incY = ((i % HUD_MAX) * HUD_SPACING_Y) << 16;
        HudMessage(s:classNames[i];
            HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
            TACDM_HUDPRINTOFFSET-39-i, "Yellow",
            HUD_NEWCLASS_X + incX, HUD_NEWCLASS_Y + incY, rate, 0.5);
    }
    else if (cash >= cost)
    {
        incX = ((i / HUD_MAX) * HUD_SPACING_X) << 16;
        incY = ((i % HUD_MAX) * HUD_SPACING_Y) << 16;
        HudMessage(s:classNames[i];
            HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
            TACDM_HUDPRINTOFFSET-39-i, "Green",
            HUD_NEWCLASS_X + incX, HUD_NEWCLASS_Y + incY, rate, 0.5);
    }
    else
    {
        incX = ((i / HUD_MAX) * HUD_SPACING_X) << 16;
        incY = ((i % HUD_MAX) * HUD_SPACING_Y) << 16;
        HudMessage( s:classNames[i];
            HUDMSG_FADEOUT | HUDMSG_COLORSTRING,
            TACDM_HUDPRINTOFFSET-39-i, "Black",
            HUD_NEWCLASS_X + incX, HUD_NEWCLASS_Y + incY, rate, 0.5);
    }
}





script TACDM_ADDMONEY (int team, int amount, int maxAmount)
{
    int emptySlot;
    int addAmount;
    int addGap;
    int reason = REASON_GOTKILL;

    if (amount < 0)
    {
        reason = REASON_SUICIDE;
    }

    if (amount == 0)
    {
        amount = GetCVar("tacdm_moneyperkill");
    }

    if (maxAmount == 0)
    {
        maxAmount = 0x7FFFFFFF;
    }

    addAmount = getTeamCash(team)+amount;
    addGap    = addAmount - min(addAmount, maxAmount);
    addAmount = amount - addGap;

    ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, addAmount, reason);
}


script TACDM_TAKEMONEY (int team, int amount, int minAmount)
{
    int emptySlot;
    int takeAmount;
    int takeGap;
    int ret;
    int reason = REASON_SUICIDE;

    if (amount < 0)
    {
        reason = REASON_GOTKILL;
    }

    if (amount == 0)
    {
        amount = GetCVar("tacdm_moneylostondeath");
    }

    takeAmount = getTeamCash(team)-amount;
    takeGap    = max(takeAmount, minAmount) - takeAmount;
    ret        = -(amount - takeGap);

    ACS_ExecuteAlways(TACDM_INTERNAL_CHANGEMONEY, 0, team, ret, reason);
}


script TACDM_PAYFORKILL (int isMonster, int amount)
{
    int killedHP = GetActorProperty(0, APROP_SpawnHealth);  // in case monster
    int killedPln = PlayerNumber();
    int killedTeam = getTeam(killedPln);

    SetActivatorToTarget(0);

    int killerPln = PlayerNumber();
    int killerTeam = getTeam(killerPln);

    if (!amount)
    {
        amount = GetCVar("tacdm_moneyperkill");
    }

    int newAmount;

    if (isMonster)
    {
        amount = (killedHP / 10) * GetCVar("tacdm_monstermult");
        ACS_ExecuteAlways(TACDM_ADDMONEY, 0, killerTeam, amount, 0);
        ACS_ExecuteAlways(TACDM_LETTHEREBESWITCH, 0, 105,0,0);
    }
    else
    {
        newAmount = amount + (classCosts[getClass(killedPln)] / 4);

        if (killedTeam == killerTeam)
        {
            if ((killedTeam == 4) && (killedPln != killerPln))
            {
                ACS_ExecuteAlways(TACDM_ADDMONEY, 0, killerTeam, newAmount, 0);
                ACS_ExecuteAlways(TACDM_LETTHEREBESWITCH, 0, SWITCH_TIME,0,0);
            }
            else
            {
                ACS_ExecuteAlways(TACDM_TAKEMONEY, 0, killerTeam, amount, 0);
            }
        }
        else
        {
            ACS_ExecuteAlways(TACDM_ADDMONEY, 0, killerTeam, newAmount, 0);
            ACS_ExecuteAlways(TACDM_LETTHEREBESWITCH, 0, SWITCH_TIME,0,0);
        }
    }
}

script TACDM_INTERNAL_CHANGEMONEY (int team, int amount, int reason)
{
    SetActivator(0);
    int i; int emptySlot = 4;

    changeTeamCash(team, amount);

    for (i = 0; i < 5; i++)
    {
        if (teamNewCash[team][i] == 0)
        {
            emptySlot = i;
            break;
        }
    }

    teamNewCash[team][emptySlot] += amount;
    teamNewCashReason[team][emptySlot] = reason;

    Delay(70);

    teamNewCash[team][emptySlot] -= amount;
}


script TACDM_LETTHEREBESWITCH (int time)
{
    int pln = PlayerNumber();

    GiveInventory("ClassSwitcherNoInvuln", 1);
    canSwitch[pln] += 1;

    Delay(time);
    canSwitch[pln] -= 1;

    if (!canSwitch[pln])
    {
        TakeInventory("ClassSwitcherNoInvuln", 1);
    }
}



script TACDM_CLASSBONUSES (int choice)
{
    switch (choice)
    {
    case 1:
        SetAmmoCapacity("TacDMRocketAmmo", 40);
        SetAmmoCapacity("PineappleGrenade", 10);
        GiveInventory("TacDMRocketAmmo", 40);
        GiveInventory("PineappleGrenade", 10);
        break;
    }
}
