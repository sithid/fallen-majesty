#define CLAN_DICTATOR 1
#define CLAN_COUNCIL 2
#define CLAN_RANKED 3

#define MAX_CLANROOMS 20
#define MAX_RANKS 20

#define REQ_HP		0
#define REQ_MANA	1
#define REQ_MOVE	2
#define REQ_LEGEND	3
#define REQ_STATUS	4
#define REQ_MAX		5

void load_clan( FILE *fp );

typedef enum
{
   CL_LEADER, CL_INDUCT, CL_OUTCAST, CL_BANK, CL_FINANCE, CL_BESTOW,
   CL_SECRET, CL_KNOWLEDGE, CL_ALLOW, CL_INDUCTEE, CL_REJECTED, CL_ACCEPTED
} clan_flags;

