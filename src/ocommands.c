
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/


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

char *const pos_table[]=
{
"dead", "mortal", "incap", "stun", "sleep", "rest", "sit", "fight", "stand",
NULL
};

char *const eq_table[]=
{
"light", "finger_l", "finger_r", "neck_1", "neck_2", "torso", "head", "legs",
"feet", "hands", "arms", "shield", "body", "waist", "wrist_l", "wrist_r",
"wield", "hold", "float", NULL
};

void do_smite( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];  /* Lot of arguments */
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char arg4 [MAX_INPUT_LENGTH];
    char arg5 [MAX_INPUT_LENGTH];
    char arg6 [MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *SmittenEQ;
    const int MAX_SMITE_FRACTION = 95;  /* You can change this if you want */
    int hp_percent = 0;
    int mana_percent = 0;
    int move_percent = 0;
    int pos = 0;
    int eq = 0;

    argument = one_argument( argument, arg1 );  /* Combine the arguments */
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );
    argument = one_argument( argument, arg6 );

    if ( IS_NPC(ch) )  /* NPCs may get to be smited, but switch immortals can't */
        return;

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
        send_to_char( "Smite <victim> <hp> <mana> <move> <position> <equipment>\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They are saved only through their abscence.\n\r",ch);
	return;
    }

    if (!IS_NPC(victim) && victim->level >= get_trust(ch))
    {
        send_to_char("Your reach exceeds your grasp.\n\r",ch);
        return;
    }

    if ( arg2[0] != '\0' )
        hp_percent = atoi( arg2 );
    else
        hp_percent = 50;

    if ( hp_percent > MAX_SMITE_FRACTION || hp_percent < 0 )
    {
        send_to_char("Hp percent must be between 0 and 95.\n\r", ch );
        return;
    }
 
    if ( arg3[0] != '\0' )
        mana_percent = atoi( arg3 );
    else
        mana_percent = 0;

    if ( mana_percent > MAX_SMITE_FRACTION || mana_percent < 0 )
    {
        send_to_char("Mana percent must be between 0 and 95.\n\r", ch );
        return;
    }
    
    if ( arg4[0] != '\0' )
        move_percent = atoi( arg4 );
    else
        move_percent = 0;

    if ( move_percent > MAX_SMITE_FRACTION || move_percent < 0 )
    {
        send_to_char("Move percent must be between 0 and 95.\n\r", ch );
        return;
    }
    
    /* Customize stuff by alignment */

    if (ch->alignment > 300)
    {                                                                                                                                   
        act("Your actions have brought the holy power of $n upon you!",ch,NULL,victim,TO_VICT, FALSE);
        act("$N has brought the holy power of $n upon themselves!",ch,NULL,victim,TO_NOTVICT, FALSE);
    }
    if (ch->alignment > -301 && ch->alignment < 301)
    {
        act("Your actions have called the divine fury of $n upon you!",ch,NULL,victim,TO_VICT, FALSE);
        act("$N has called the divine fury of $n upon themselves!",ch,NULL,victim,TO_NOTVICT, FALSE);
    }
    if (ch->alignment < -300)
    {
        act("You are struck down by the infernal power of $n!!",ch,NULL,victim,TO_VICT, FALSE);
        act("The hellspawned, infernal power of $n has struck down $N!!",ch,NULL,victim,TO_NOTVICT, FALSE);
    }


    /* If it REALLY hurt */

    if ( hp_percent > 75 && victim->hit > victim->max_hit / 4 )
        send_to_char( "#1That really did HURT!#n\n\r", victim );

    /* Let's see if equipment needs to be 'blown away' */

    for ( eq = 0; eq_table[eq] != NULL; eq++ )
    {
        if ( !str_prefix( eq_table[eq], arg6 ) )
        {
            if ( ( SmittenEQ = get_eq_char( victim, eq ) ) != NULL )
            {
                sprintf( buf, "$n's %s is blown away!", SmittenEQ->short_descr);
                act( buf, victim, NULL, NULL, TO_ROOM, FALSE);
                sprintf( buf, "Your %s is blown away!\n\r", SmittenEQ->short_descr);
                send_to_char( buf, victim );

                unequip_char( victim, SmittenEQ );
                obj_from_char( SmittenEQ );
                obj_to_room( SmittenEQ, victim->in_room );
            }
            break;
        }
    }

    /* Let's see what position to put the victim in */

    for ( pos = 0; pos_table[pos] != NULL; pos++ )
    {
        if ( !str_prefix( pos_table[pos], arg5 ) )
        {
            victim->position = pos;                 /* This only works because of the way the pos_table
is arranged. */
            if ( victim->position == POS_FIGHTING ) 
                victim->position = POS_STANDING;     /* POS_FIGHTING is bad */

            if ( victim->position < POS_STUNNED )
                victim->position = POS_STUNNED;

            if ( victim->position == POS_STUNNED )
            {
                act( "$n is stunned, but will probably recover.",victim, NULL, NULL, TO_ROOM, FALSE );
                send_to_char("You are stunned, but will probably recover.\n\r",victim );
            }    

            if ( victim->position == POS_RESTING || victim->position == POS_SITTING )
            {
                act("$n is knocked onto $s butt!", victim, NULL, NULL, TO_ROOM, FALSE );
                send_to_char("You are knocked onto your butt!\n\r", victim );
            }
            break;
        }
    }

    /* Calculate total hp loss */
    victim->max_hit -= ( ( victim->max_hit * hp_percent ) / 100 );
    if ( victim->max_hit < 1 )
        victim->max_hit = 1;

    /* Calculate total mana loss */
    victim->max_mana -= ( ( victim->max_mana * mana_percent ) / 100 );
    if ( victim->max_mana < 1 )
       victim->max_mana = 1;

    /* Calculate total move loss */
    victim->max_move -= ( ( victim->max_move * move_percent ) / 100 );
    if ( victim->max_move < 1 )
        victim->max_move = 1;

    send_to_char("Your will is done, your power felt.\n\r",ch);
    return;
}

void do_beep ( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
	return;

    argument = one_argument( argument, arg );

    if  ( arg[0] == '\0' )
    {
	send_to_char( "Beep who?\n\r", ch );
	return;
    }

    if ( !( victim = get_char_world( ch, arg ) ) )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }


    if ( IS_NPC(victim))
    {
	send_to_char( "They are not beepable.\n\r", ch );
	return;
    }

    sprintf( buf, "\aYou beep %s.\n\r", victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "\a%s has beeped you.\n\r", ch->name );
    send_to_char( buf, victim );
    return;
}

void do_sever( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       send_to_char("Who are you going to rip in half?\n\r",ch);
       return; 
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL ) 
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if(victim == ch)
    {
	send_to_char("You reach your neck fine, but cant' get your ankles.\n\r",ch);
	return;
    }

    if(IS_SET(victim->affected_by2,AFF_SEVERED))
    {
	send_to_char("They have no legs to rip off.\n\r",ch);
	return;
    }

    SET_BIT(victim->affected_by2,AFF_SEVERED);
    if(!IS_NPC(victim))
    act("$n picks you up and rips you in half! Oh no!",ch,NULL,victim,TO_VICT, FALSE);
    act("$n picks up $N and rips $S legs off!",ch,NULL,victim,TO_NOTVICT, FALSE);
    send_to_char("You rip them in half!\n\r",ch);

    obj = create_object( get_obj_index(OBJ_VNUM_LEGS ), 0 );

   
if(IS_NPC(victim))
    sprintf(buf,"A pair of %s's legs are here, twitching.",victim->short_descr);
else
    sprintf(buf,"A pair of %s's legs are here, twitching.",victim->name);	
    free_string(obj->description);
    obj->description = str_dup( buf );


if(IS_NPC(victim))
    sprintf(buf,"A pair of %s's legs",victim->short_descr);
else
    sprintf(buf,"A pair of %s's legs",victim->name);    
    free_string(obj->short_descr);
    obj->short_descr = str_dup( buf );

    obj_to_char(obj,ch);

}

