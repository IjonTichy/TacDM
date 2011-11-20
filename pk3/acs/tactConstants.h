
#define DEBUG                           1
#define TACDM_NAMESPACING               10


#define TACDM_BASEOFFSET                732
#define TACDM_CLASSPRINTOFFSET          635
#define TACDM_HUDPRINTOFFSET            435

#define TACDM_AUTO_COMMON               TACDM_BASEOFFSET-200

#define TACDM_AUTO_OPEN                 TACDM_BASEOFFSET-100
#define TACDM_AUTO_OPEN_CLIENT          TACDM_BASEOFFSET-101
#define TACDM_AUTO_ENTER                TACDM_BASEOFFSET-102
#define TACDM_AUTO_DEATH                TACDM_BASEOFFSET-103
#define TACDM_AUTO_RESPAWN              TACDM_BASEOFFSET-104
#define TACDM_AUTO_DISCONNECT           TACDM_BASEOFFSET-105

#define TACDM_ITEM_CLASS                TACDM_BASEOFFSET+1
#define TACDM_CHOOSECLASS               TACDM_BASEOFFSET+2
#define TACDM_PRINTCLASSDESC            TACDM_BASEOFFSET+3
#define TACDM_SETUPCLASS                TACDM_BASEOFFSET+4
#define TACDM_HUD                       TACDM_BASEOFFSET+5
#define TACDM_SCALESPEED                TACDM_BASEOFFSET+6
#define TACDM_LETTHEREBESWITCH          TACDM_BASEOFFSET+7

#define TACDM_CLIENT_PRINTMONEY         TACDM_BASEOFFSET+100
#define TACDM_CLIENT_PRINTNEWMONEY      TACDM_BASEOFFSET+101
#define TACDM_CLIENT_PRINTCLASSES       TACDM_BASEOFFSET+102

#define TACDM_ADDMONEY                  TACDM_BASEOFFSET+21
#define TACDM_TAKEMONEY                 TACDM_BASEOFFSET+22
#define TACDM_PAYFORKILL                TACDM_BASEOFFSET+23

#define TACDM_INTERNAL_CHANGEMONEY      TACDM_BASEOFFSET+51

#define HUD_UPDATERATE                  3500

#define HUD_ASPECT1_W                   640
#define HUD_ASPECT1_H                   480

#define HUD_TEAMMONEY_X                 630.2
#define HUD_TEAMMONEY_Y                 10.1

#define HUD_NEWMONEY_X                  625.2
#define HUD_NEWMONEY_Y                  25.1



#define HUD_ASPECT2_W                   640
#define HUD_ASPECT2_H                   480

#define HUD_NEWCLASS_X                  2.3
#define HUD_NEWCLASS_Y                  240.1

#define HUD_SPACING_X                   120
#define HUD_SPACING_Y                   12
#define HUD_MAX                         10

#define SWITCH_TIME                     105

#define REASON_GOTKILL                  0
#define REASON_SUICIDE                  1
#define REASON_BOUGHTCLASS              2
#define REASON_SOLDCLASS                3

int reasonColors[4] = {"Green", "Red", "Yellow", "Gold"};
