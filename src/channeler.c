/********************************************************************
 * This code was written originally by Alstor (cristofv@icqmail.com *
 * Use of this code is prohibited unless with written permission by *
 * Alstor. This acknowledgement may not be removed from this code.  *
 * Channelers, and other names for this class, are trademarks of    *
 * the Wheel of Time series by Robert Jordan, at Tor Books.         *
 * I have no affiliation with Robert Jordan or Tor Books.           *
 * © 2001 Alstor and Malastar from Chaotic Destinies                *
 ********************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "player.h"

bool is_channeler (CHAR_DATA *ch)
{
	if ( !IS_CLASS(ch, CLASS_CHANNELER))
	{
		do_rand_typo(ch, "");
		return FALSE;
	}
	return TRUE;
}

void do_cresearch ( CHAR_DATA *ch, char *argument )
{
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	char buf1 [MAX_STRING_LENGTH];
	int cost, mmax, smax;

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);

	if (IS_NPC(ch)) return;

        if (!is_channeler(ch)) return;
	
	if (arg1[0] == '\0'  && arg2[0] == '\0' )
	{
		switch( ch->pcdata->powers[CCLAN] )
		{
			case ASHAMAN:
			{
				mmax = 200;
				smax = 100;
			}
                        case DEDICATED:
                        {
                                mmax = 150;
                                smax = 150;
                        }
                        case SOLDIER:
                        {
                                mmax = 100;
                                smax = 200;
                        }
                        case AESSEDAI:
                        {
                                mmax = 200;
                                smax = 100;
                        }
                        case ACCEPTED:
                        {
                                mmax = 150;
                                smax = 150;
                        }
                        case HUNTRESS:
                        {
                                mmax = 100;
                                smax = 200;
                        }
                        default:
                        {
                                mmax = 200;
                                smax = 200;
                        }
		}
/*                if (arg1[0] == '\0' && arg2[0] == '\0' )
                {
                	stc("Channeler Discoveries#0:#n\n\r", ch);
			sprintf(buf1,"Earth #0(#7%d#0/#7%d#0)#n Air #0(#7%d#0/#7%d#0)#n Fire #0(#7%d#0/#7%d#0)#n Water #0(#7%d#0/#7%d#0)#n Spirit #0(#7%d#0/#7%d#0)#n Weaponry #0(#7%d#0/#7%d#0)#n\n\r",
			ch->pcdata->powers[CHANNELER_EARTH], mmax, ch->pcdata->powers[CHANNELER_AIR], mmax, ch->pcdata->powers[CHANNELER_FIRE], mmax, ch->pcdata->powers[CHANNELER_WATER], mmax, ch->pcdata->powers[CHANNELER_SPIRIT], mmax, ch->pcdata->powers[CHANNELER_WEAPONRY], smax );
			stc(buf1, ch);
                        return;
                } */

/*                if (arg1 != NULL && arg2[0] == '\0')
                { */
                	if (!str_cmp(arg1, "earth"))
                        {
                        	stc("Earth Powers:\n\r", ch);
                                return;
                        }
                        if (!str_cmp(arg1, "air"))
                        {
                                stc("Air Powers:\n\r", ch);
                                return;
                        }
                        if (!str_cmp(arg1, "fire"))
                        {
                                stc("Fire Powers:\n\r", ch);
                                return;
                        }
                        if (!str_cmp(arg1, "water"))
                        {
                                stc("Water Powers:\n\r", ch);
                                return;
                        }
                        if (!str_cmp(arg1, "spirit"))
                        {
                                stc("Spirit Powers:\n\r", ch);
                                return;
                        }
                        if (!str_cmp(arg1, "weaponry"))
                        {
                                stc("Weaponry Techniques:\n\r", ch);
                                return;
                        }
                        else
                        {
                                stc("Syntax: Cresearch <Earth/Air/Fire/Water/Spirit/Weaponry> <Advance>\n\r", ch);
                                return;
                        }
//                }
                if (arg1 != NULL && arg2 != NULL)
                {
                        if ((!str_cmp(arg1, "earth") && (!str_cmp(arg2, "improve") || !str_cmp(arg2, "advance") || !str_cmp(arg2, "learn") || !str_cmp(arg2, "gain"))))
                        {
                        	cost = 500;

                                arg1[0] = UPPER(arg1[0]);

                                if (IS_SET(ch->pcdata->powers[CHANNELER_POWERS], CHANNELER_EARTH))
                                {
                                	stc("You have already begun training on the earth element!\n\r", ch);
                                        return;
                                }

                                if ( cost > ch->practice )
                                {
                                        sprintf( buf1, "You need %d more primal to gain the earth element!\n\r", cost - ch->practice );
                                        stc( buf1, ch);
                                        return;
                                }

                                if ( ch->practice >= cost )
                                {
                                        SET_BIT(ch->pcdata->powers[CHANNELER_EARTH], CHANNELER_EARTH);
                                        ch->practice -= cost;
                                        stc( "You have begun training the earth element!\n\r", ch);
                                        return;
                                }
                        }
                        else
                        stc("Blah.\n\r", ch);
                        return;
                }
                return;
	}
        return;
}

void do_followpath ( CHAR_DATA *ch, char *argument )
{
	char arg1 [MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg1 );

	if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_CHANNELER))
	{
		do_rand_typo(ch, "");
		return;
	}

        if (IS_SET(ch->pcdata->powers[CCLAN], ASHAMAN) || IS_SET(ch->pcdata->powers[CCLAN], DEDICATED) ||
        IS_SET(ch->pcdata->powers[CCLAN], SOLDIER) || IS_SET(ch->pcdata->powers[CCLAN], AESSEDAI) ||
        IS_SET(ch->pcdata->powers[CCLAN], ACCEPTED) || IS_SET(ch->pcdata->powers[CCLAN], HUNTRESS))
        {
                stc("You are already following a path.\n\r", ch);
                return;
        }

	if ((arg1[0] == '\0') && (ch->sex == SEX_MALE))
	{
		stc("Syntax: Followpath#0:#n\n\r", ch);
		stc("        Asha'man  #0:#n Strong Magical.\n\r", ch);
		stc("        Dedicated #0:#n Medium Magical and Physical.\n\r", ch);
		stc("        Soldier   #0:#n Strong Physical.\n\r", ch);
		return;
	}
        if ((arg1[0] = '\0') && (ch->sex != SEX_MALE))
	{
		stc("Syntax: Followpath#0:#n\n\r", ch);
		stc("        Aes Sedai #0:#n Strong Magical.\n\r", ch);
		stc("        Accepted  #0:#n Medium Magical and Physical.\n\r", ch);
		stc("        Huntress  #0:#n Strong Physical.\n\r", ch);
		return;
	}

	if (ch->sex == SEX_MALE && (arg1 != NULL))
	{
		if (!str_cmp(arg1, "ashaman") || !str_cmp(arg1, "asha'man"))
		{
			stc("You have chosen the path of the Asha'man.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], ASHAMAN);
			return;
		}

		if (!str_cmp(arg1, "dedicated"))
		{
			stc("You have chosen the path of the Dedicated.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], DEDICATED);
			return;
		}

		if (!str_cmp(arg1, "soldier"))
		{
			stc("You have chosen the path of the Soldier.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], SOLDIER);
			return;
		}

		else
		{
			stc("Syntax: Followpath#0:#n\n\r", ch);
			stc("        Asha'man  #0:#n Strong Magical.\n\r", ch);
			stc("        Dedicated #0:#n Medium Magical and Physical.\n\r", ch);
			stc("        Soldier   #0:#n Strong Physical.\n\r", ch);
			return;
		}
	}

	else if (ch->sex != SEX_MALE)
	{
		if (!str_cmp(arg1, "aes sedai") || !str_cmp(arg1, "aessedai") || !str_cmp(arg1, "aes_sedai"))
		{
			stc("You have chosen the path of the Aes Sedai.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], AESSEDAI);
			return;
		}

	        if (!str_cmp(arg1, "accepted"))
		{
			stc("You have chosen the path of the Accepted.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], ACCEPTED);
			return;
		}

		if (!str_cmp(arg1, "huntress"))
		{
			stc("You have chosen the path of the Huntress.\n\r", ch);
			SET_BIT(ch->pcdata->powers[CCLAN], HUNTRESS);
			return;
		}

		else
		{
			stc("Syntax: Followpath#0:#n\n\r", ch);
			stc("        Aes Sedai #0:#n Strong Magical.\n\r", ch);
			stc("        Accepted  #0:#n Medium Magical and Physical.\n\r", ch);
			stc("        Huntress  #0:#n Strong Physical.\n\r", ch);
			return;
		}
	}
	return;
}

void do_sourcestrength ( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) ) return;

  if ( !is_channeler(ch) ) return;

  centre_text("The One Power", ch);
  sprintf(buf,"You currently possess %d points of The One Power.\n\r", ch->pcdata->powers[CHANNELER_POWERS], ONE_CURRENT);
  stc(buf,ch);
  return;
}
