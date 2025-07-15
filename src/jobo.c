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


/**********************************
 * All kinds of new stuff by Jobo *
 **********************************/

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

void do_clearstats2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ( obj->wear_loc != WEAR_NONE )
        {obj_from_char(obj); obj_to_char(obj,ch);}
    }

    while ( ch->last_affect )
      affect_remove( ch, ch->last_affect );
//  if ( IS_SET(ch->newbits, NEW_CUBEFORM))
//    REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
    if (IS_EXTRA(ch,EXTRA_DRAGON)) xREMOVE_BIT(ch->extra, EXTRA_DRAGON);
   
    ch->affected_by      = 0;
    ch->armor            = 100;
    ch->hit              = UMAX( 1, ch->hit  );
    ch->mana             = UMAX( 1, ch->mana );
    ch->move             = UMAX( 1, ch->move );
    ch->hitroll          = 0;
    ch->damroll          = 0;
    ch->saving_throw     = 0;
    ch->pcdata->mod_str  = 0;
    ch->pcdata->mod_int  = 0;
    ch->pcdata->mod_wis  = 0;
    ch->pcdata->mod_dex  = 0;
    ch->pcdata->mod_con  = 0;
    ch->pcdata->followers = 0;
    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    if (IS_ITEMAFF(ch, ITEMA_RAGER)) REMOVE_BIT(ch->itemaffect, ITEMA_RAGER);
    set_form(ch, get_normal_form(ch));
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
     stc("Your form has been cleared.  Please turn it back on.\n\r",ch);
    return;
}

/*
 *  If an item have a bad char in it's short/long/name desc, the mud
 *  will freeze when the char with that item tries to log on, so use
 *  this check on any renaming of items (glamour/reshape/etc).
 */
bool has_bad_chars(CHAR_DATA *ch, char *argument)
{
  int i;

  if ( argument[0] == '\0' ) return FALSE;
  for (i = 0; argument[i] != '\0' ; i++)
  {
    if (argument[i] == '~') return TRUE;
  }
  return FALSE;
}

void do_bountylist(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

sprintf(buf, 
"#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #CB#co#0u#nn#7t#Cy#cl#0i#ns#7t#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n\n\r");
stc(buf,ch);
  stc("  #CName               Bounty     Status     Generation     Legend#n\n\r",ch);
  for ( d = first_descriptor; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {
     
  if (!d->connected == CON_PLAYING) continue;
       if (d->character->level > 6) continue;
       if (!IS_HERO(d->character)) continue;
       if (d->character->pcdata->chobj != NULL) continue;
       if (IS_SET(d->character->special, SPC_NEWBIE)) continue;
       if (!can_see(ch, d->character)) continue;
       sprintf(buf, "  #G%-15s#7    %5d        %2d           %d             %d#n\n\r",
d->character->name, d->character->pcdata->bounty,
d->character->race,d->character->generation,d->character->lstatus);
       stc(buf,ch);
    }
  }
sprintf(buf, 
"#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #CB#co#0u#nn#7t#Cy#cl#0i#ns#7t#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n\n\r");  
stc(buf,ch);
  return;
}

void do_range(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

sprintf(buf,
"#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #CR#ca#0n#ng#7e#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n\n\r");
stc(buf,ch);
  stc("  #CName               Bounty     Status     Generation     Legend#n\n\r",ch);
  for ( d = first_descriptor; d != NULL; d = d->next )
  {
    if ( d->character != NULL )
    {

  if (!d->connected == CON_PLAYING) continue;
       if (d->character->level > 6) continue;
       if (!IS_HERO(d->character)) continue;
       if (d->character->pcdata->chobj != NULL) continue;
       if (!can_see(ch, d->character)) continue;
       if (is_range(ch,d->character)) continue;
       sprintf(buf, "  #G%-15s#7    %5d        %2d           %d             %d#n\n\r",
d->character->name, d->character->pcdata->bounty,
d->character->race,d->character->generation,d->character->lstatus);
       stc(buf,ch);
    }

  }
sprintf(buf,
"#0(#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#0]#n+#7+#n+#0+#n+#7+#n+#L|#c  #CR#ca#0n#ng#7e#L  |#n+#7+#n+#0+#n+#7+#n+#0[#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#R=#r=#0)#n\n\r");  
stc(buf,ch);
  return;
}

void do_version( CHAR_DATA *ch, char *argument) 
{
  cent_to_char("#0-=================================================================-#n", ch);
  cent_to_char("This MUD started out as Fallen Majesty coded by Jank, Rand and Snard.", ch);
  cent_to_char("Based on Dystopia v1.0.6 by Jobo", ch);
  cent_to_char("#0-=================================================================-#n", ch);
  return;
}

/* The costy healer */
void do_healme(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->in_room != NULL && ch->in_room->vnum != ROOM_VNUM_ALTAR)
  {
    send_to_char("Only at the Altar of Sacrifice!\n\r",ch);
    return;
  }
  if (ch->fight_timer > 0)
  {
    send_to_char("Not with a fighttimer.\n\r",ch);
    return;
  }
  if (ch->exp < 10000000000)
  {
    send_to_char("The healer demands 10000000000 exp as payment for his service.\n\r",ch);
    return;
  }
  ch->hit  = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  send_to_char("The healer heals your wounds and takes 10000000000 exp as payment.\n\r",ch);
  ch->exp -= 10000000000;
  WAIT_STATE(ch, 72);
  return;
}

/* MUDSTAT command */
void do_mudstat( CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *gch;  
  int paladin_count=0, mage_count=0, lich_count=0, drow_count=0, drider_count=0, ww_count=0, shape_count=0, monk_count=0, angel_count=0;
  int psionist_count=0, vampire_count=0, knight_count=0, ninja_count=0, sam_count=0, demon_count=0, tanarri_count=0, newbie_count=0, total_count=0;
  extern char str_boot_time[];

  if (IS_NPC(ch)) return;

  for ( d = first_descriptor; d != NULL; d = d->next )
  {
    if ( d->character != NULL ) gch = d->character;
    else continue;
    if (!d->connected == CON_PLAYING) continue;
    if (gch->level > 6) continue;
    switch (gch->class)
    {
      case 0    : newbie_count++;total_count++;break;
      case 1    : demon_count++;total_count++;break;
      case 2    : mage_count++;total_count++;break;
      case 4    : ww_count++;total_count++;break;
      case 8    : vampire_count++;total_count++;break;
      case 16   : sam_count++;total_count++;break;
      case 32   : drow_count++;total_count++;break;
      case 64   : monk_count++;total_count++;break;
      case 128  : ninja_count++;total_count++;break;
      case 256  : lich_count++;total_count++;break;
      case 512  : shape_count++;total_count++;break;
      case 1024 : tanarri_count++;total_count++;break;
      case 2048 : angel_count++;total_count++;break;
      case 4096 : knight_count++;total_count++;break;
      case 8192 : drider_count++;total_count++;break;
      case 16384: paladin_count++;total_count++;break;
      case 32768: psionist_count++;total_count++;break;
    }
  }

  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R== [#y^^#R]  MUD STATS  [#y^^#R] ==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--#n\n\r\n\r",ch);
  send_to_char("#LOnline Players by Class :#n\n\r\n\r",ch);
  sprintf(buf, "#GMages#n     : %-2d            #GLichs#n      : %-2d       #GDrows#n       : %-2d     #GDriders#n :       %-2d\n\r",
    mage_count, lich_count, drow_count, drider_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GDemons#n    : %-2d            #GTanar'ris#n  : %-2d       #GWerewolfs#n   : %-2d     #GShapies#n :       %-2d\n\r",
    demon_count, tanarri_count, ww_count, shape_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GNinjas#n    : %-2d            #GSamurais#n   : %-2d       #GVampires#n    : %-2d     #GKnights#n :       %-2d\n\r",
    ninja_count, sam_count, vampire_count, knight_count);
  send_to_char(buf,ch);
  sprintf(buf, "#GMonks#n     : %-2d            #GAngels#n     : %-2d       #GPaladins#n    : %-2d     #GPsionist#n:       %-2d\n\r",monk_count, angel_count, paladin_count, psionist_count);
  stc(buf,ch);
  sprintf(buf, "#GNewbies#n   : %-2d            #GTotal#n      : %-2d#n\n\r",newbie_count,total_count);
  send_to_char(buf,ch);
  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==      OTHER STATS     ==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--#n\n\r\n\r",ch);
  if (sysdata.exp_timer >= 0)
  {
    sprintf(buf,"#LDouble exp is #yON #Lfor #y%d #Lmins#n\n\r", sysdata.exp_timer/60);
    send_to_char(buf, ch);
  }
  else
    send_to_char("#LDouble exp is off.\n\r",ch);

/*  if (sysdata.qp_timer >= 0)
  {
    sprintf(buf,"#LDouble qp is #yON #Lfor #y%d #Lmins#n\n\r", sysdata.qp_timer/60);
    send_to_char(buf, ch);
  }
  else
    send_to_char("#LDouble qp is off.\n\r",ch);
*/
  if (sysdata.ffa_timer >= 0)
  {
    sprintf(buf,"#0The #RFREE #yFOR #RALL#0 is #yON #0for #y%d #0mins#n\n\r\n\r", sysdata.ffa_timer/60);
    send_to_char(buf, ch);
  }
  else
    send_to_char("#0The #RFREE #yFOR #RALL#0 is off.#n\n\r\n\r",ch);

  sprintf(buf, "#RNumber of players connected since last copyover/restart :#C %d\n\r#n", players_logged);
  send_to_char(buf,ch);
  sprintf(buf, "#RNumber of players beheaded since last copyover/restart  :#C %d\n\r#n", players_decap);
  send_to_char(buf,ch);
  sprintf(buf, "#RAmount of generation stolen since last copyover/restart :#C %d\n\r\n\r#n", players_gstolen);
  send_to_char(buf,ch);
//  sprintf(buf, "#RHighest number of players connected ever :#C %d - %s\n\r\n\r#n", sysdata.alltimemax, sysdata.time_of_max );
//  send_to_char(buf,ch);
  sprintf( buf, "Dystopia was last (re)started at : %s\rThe system time is currently     : %s\n\r", str_boot_time, (char *) ctime( &current_time));
  send_to_char( buf, ch );
  send_to_char("#R--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==#L**#R==--==--#n\n\r\n\r",ch);
  return;
}

/* show linkdeads - code by Marlow, not Jobo */

void do_linkdead(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool found = FALSE;

  for (gch = char_list; gch != NULL; gch = gch->next)
  {
    if (IS_NPC(gch) || gch->desc) continue;
    found = TRUE;
    sprintf(buf, "Name: %12s. (Room: %5d)\n\r", gch->name, gch->in_room == NULL ? : gch->in_room->vnum);
    send_to_char(buf,ch);
  }
  if(!found) send_to_char("No Linkdead Players found\n\r",ch);
  return;
}

/* Pk powers, status and pk-record based */

void do_pkscry(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  char buf[MSL];
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;

  if (ch->race < 10)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Scry on whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is scrying you.\n\r",victim);
    return;
  }
  if (IS_SET(victim->immune, IMM_SCRY))
  {
    sprintf(buf, "You feel %s's eyes on your back.\n\r",ch->name);
    stc(buf, victim);
  }

  chroom = ch->in_room;
  victimroom = victim->in_room;
  char_from_room(ch);
  char_to_room(ch,victimroom);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  { 
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"auto");
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
  }
  else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  {
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"auto");
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
  } 
  else
    do_look(ch,"auto");
  char_from_room(ch);
  char_to_room(ch,chroom);
  if (ch->level < 7) ch->fight_timer += 3;
  return;
}

void do_pkportal (CHAR_DATA *ch, char *argument)
{  
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  one_argument (argument, arg);
    
  if (IS_NPC(ch)) return;
  if (ch->race < 10)
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Walk the silverpath to whom?\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->fight_timer == 0)
  {
    send_to_char("They are not engaged in pk.\n\r",ch);
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
  {
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
  {
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }

  location = victim->in_room;
  act("You step onto the silverpath.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n steps into thin air.", ch, NULL, NULL, TO_ROOM, FALSE);
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You leave the silverpath.", ch, NULL, NULL, TO_CHAR, FALSE);
  act("$n steps out of thin air.", ch, NULL, NULL, TO_ROOM, FALSE);
  if (ch->race < 30) { WAIT_STATE (ch, ch->race / 30 +4); return;}  
  WAIT_STATE(ch, 4);
  return;  
}

void do_pkaura (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char      arg [MAX_INPUT_LENGTH];
  char      buf [MAX_INPUT_LENGTH];
   
  argument = one_argument( argument, arg );
   
  if (IS_NPC(ch)) return;
  if (ch->race < 15)
  {
    send_to_char("huh?\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char("Read the aura of who?\n\r",ch);
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    return;
  }
  if (victim->trust > 6)
  {
    send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
    send_to_char("Someone is trying to read your aura.\n\r",victim);
    return;
  }
  if (ch->trust < 7) send_to_char("You are being examined.\n\r",victim);

  sprintf(buf,"#RHp:#y%d#R/#y%d#R, Mana:#y%d#R/#y%d#R, Move:#y%d#R/#y%d#R.#n\n\r",
    victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
  send_to_char(buf,ch);
  sprintf(buf,"#RHitroll:#y%d#R, Damroll:#y%d#R, AC:#y%d#R.#n\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
  send_to_char(buf,ch);
  sprintf(buf,"#RStatus:#y%d#R, Generation:#y%d#R, Primal:#y%d#R.#n\n\r",victim->race,victim->generation,victim->practice);
  send_to_char(buf,ch);
  sprintf(buf,"#RSaving Spell: #y%d#R.#n\n\r",victim->saving_throw);
  stc(buf,ch);
  if (ch->level < 7) ch->fight_timer += 3;
  return;
}

void do_pkpowers(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (ch->race < 10)
    send_to_char("You suck, get lost.\n\r",ch);
  if (ch->race > 9)
    send_to_char("10. eaglesight   - Scry on players even through shield.\n\r",ch);
//    send_to_char("10. Silverpath   - Lag Based Travel to those with FT.\n\r",ch);
  if (ch->race > 14)
    send_to_char("15. lifesense    - Global readaura.\n\r",ch);
  if (ch->race > 19)
    send_to_char("20. awareness    - Spiritguardian type power for all scry type powers.\n\r",ch);
  if (ch->race > 24)
    send_to_char("25. clairvoyance - Scry on players even through class invis.\n\r",ch);
  if (ch->race > 30) 
    send_to_char("30. oversee      - See where peoples are, globally.\n\r",ch);
 // if (UMIN(ch->race, (ch->pkill - ch->pdeath)) > 49)
 //   send_to_char("ged5 - Does nothing\n\r",ch);
  return;
}

/* Genstealing is in :-) */

void do_gensteal(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *location;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH]; 
  
  one_argument( argument, arg);
  if (IS_NPC(ch)) return;
  if (arg[0] == '\0')
  {
    send_to_char("Gensteal whom?\n\r",ch);
    return;
  }
  if ((victim=get_char_room(ch,arg))==NULL)
  {
    send_to_char("They aren't here.\\r",ch);
    return;
  }
  if (ch == victim)
  {
    send_to_char("How stupid are you?!?\n\r",ch);
    return;
  }
    if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
  {
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
  {
    stc( "The arena prevents you from doing this.\n\r",ch);
    return;
  }

  if (IS_NPC(victim))
  {
    send_to_char("Hmmmm, no!\n\r",ch);
    return;
  }
  if (!CAN_PK(ch))
  {
    send_to_char("Maybe you should join the ranks of playerkillers first.\n\r",ch);
    return;
  }
  if (!CAN_PK(victim))
  {
    send_to_char("Stop messing with the little people.\n\r",ch);
    return;
  }
/*  if (ch->class != victim->class)
  {
    send_to_char("Only on people of the same class as yourself.\n\r",ch);
    return;
  }*/
//  if ((get_age(ch)-17) < 2)
  if (IS_SET(ch->special, SPC_NEWBIE))
  {
    send_to_char("Your still a newbie.\n\r",ch);
    return;
  }
//  if ((get_age(victim)-17) < 2)
  if (IS_SET(victim->special, SPC_NEWBIE))
  {
    send_to_char("Not on newbies.\b\r",ch);
    return;
  }
  if (victim->position > 1)
  {
    send_to_char("He resists your attempt to gensteal.\n\r",ch);
    return;
  }
  if (ch->generation < victim->generation)
  {
    send_to_char("Sorry, you are already a higher generation than they are.\n\r",ch);
    return;
  }
  if (victim->generation > 7)
  {
    send_to_char("They are worthless.\n\r",ch);
    return;
  }
if (ch->generation == 0)
  {
    send_to_char("You are the highest possibly generation.\n\r",ch);
    return;
  }
  if (str_cmp(victim->pcdata->killedby, ch->pcdata->switchname))
  {
    send_to_char( "You did not tie them, you cannot gensteal them.\n\r",ch);
    return;
  }

  if (is_safe(ch,victim)) return;
  act("Lightning flows through your body as you steal the powers of $N.\n\r", ch, NULL, victim, TO_CHAR, FALSE);
  act("$n puts his hands on $N's head and lightning cowers his body.\n\r", ch, NULL, victim, TO_NOTVICT, FALSE);
  send_to_char("Your generation has been stolen.\n\r",victim);
  sprintf(buf, "#C%s #0has beaten #P%s #0in #Rcombat#0, and has stolen %s generation!#n",ch->pcdata->switchname,    victim->pcdata->switchname,ch->sex == SEX_MALE ? "his" : ch->sex == SEX_FEMALE ? "her" : "its");
  do_update(ch,buf);
  ch->hit = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;
  ch->generation -= 1;
  victim->generation += 1;
  victim->hit = 1;
  update_pos(victim);
  WAIT_STATE(ch, 12);
  if ((location = get_room_index(ROOM_VNUM_NECROPOLIS)) == NULL)
    return;
  char_from_room(victim);
  char_to_room(victim, location);
  victim->fight_timer = 0;
  players_gstolen++;
  return;
}

/* superstances are in :-) */

void do_setstance(CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  int currentstance;
  char buf[MAX_STRING_LENGTH];
  int cost = 0;
  int count = 0;
  int max_supreme = 0;
  int max_greater = 0;
  int max_lesser = 0; 
  int min_cost = 0;   
  int max_cost = 0;   
  int current_supreme = 0;
  int current_greater = 0;
  int current_lesser = 0; 
  
  argument = one_argument( argument, arg1 );
  strcpy(arg2, argument);
  
  if (ch->stance[STANCE_TIGER] < 200 || ch->stance[STANCE_SWALLOW] < 200 || ch->stance[STANCE_MONKEY] < 200
  || ch->stance[STANCE_MANTIS] < 200 || ch->stance[STANCE_DRAGON] < 200)
  {
    send_to_char("You need to max your normal stances first.\n\r",ch);
    return;
  }
  if (ch->stance[19] == -1) {max_lesser = 3; currentstance = 19; min_cost=40; max_cost=240;}
  else if (ch->stance[20] == -1) {max_lesser = 4; max_greater = 1; currentstance = 20; min_cost = 80; max_cost = 280;}
  else if (ch->stance[21] == -1) {max_lesser = 4; max_greater = 2; currentstance = 21; min_cost = 120; max_cost = 300;}
  else if (ch->stance[22] == -1) {currentstance = 22; max_lesser = 4; max_greater = 4; max_supreme = 1; min_cost = 160; max_cost = 360;}
  else if (ch->stance[23] == -1) {currentstance = 23; max_lesser = 4; max_greater = 4; max_supreme = 2; min_cost = 200; max_cost = 400;}
  else
  {
    send_to_char("You already have all your superstances.\n\r",ch);
    return;
  }
  if (currentstance > 19 && ch->stance[currentstance-7] < 200)
  {
    send_to_char("Maybe you should max your current superstance first.\n\r",ch);
    return;
  }

  if (IS_SET(ch->stance[18], STANCEPOWER_DODGE)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_PARRY)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_SPEED)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS)) {count++;cost += count*20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme++;cost += 60;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser++;cost += 20;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater++;cost += 40;}
  if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme++;cost += 60;}
   
  if (arg1[0] == '\0')
  {
    send_to_char(" #r***************************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#C                               Superstance Editor v0.2 by Jobo                                   #r*#n\n\r",ch);
    send_to_char(" #r***************************************************************************************************#n\n\r",ch);
    send_to_char(" #r*#n [DAMCAP] Increase your damcap.                                                                  #r*#n\n\r",ch);
    send_to_char(" #r*#n [REV_DC] Lower your opponents damcap.                                                           #r*#n\n\r",ch);
    send_to_char(" #r*#n [DAMAGE] Increase your damage.                                                                  #r*#n\n\r",ch);
    send_to_char(" #r*#n [RESIST] Resist more damage.                                                                    #r*#n\n\r",ch);
    send_to_char(" #r*-------------------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n The above powers comes in lesser, greater and supreme versions. costing (20/40/60 mill exp)     #r*#n\n\r",ch);
    if (currentstance == 19)
      send_to_char(" #r*#C You may choose up to three lesser powers.                                                       #r*#n\n\r",ch);
    else if (currentstance == 20)
      send_to_char(" #r*#C You may choose up to one greater power.                                                         #r*#n\n\r",ch);
    else if (currentstance == 21)
      send_to_char(" #r*#C You may choose up to two greater powers.                                                        #r*#n\n\r",ch);
    else if (currentstance == 22)
      send_to_char(" #r*#C You may choose up to one supreme power.                                                         #r*#n\n\r",ch);
    else if (currentstance == 23)
      send_to_char(" #r*#C You may choose up to two supreme powers.                                                        #r*#n\n\r",ch);
    if (currentstance > 19)
      send_to_char(" #r*#n There are no maximum on powers of lower level than that.                                        #r*#n\n\r",ch);
    send_to_char(" #r*-------------------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Currently you have chosen the following for your next superstance :                             #r*#n\n\r",ch);
    if (ch->stance[18] == 0)
      send_to_char(" #r*#n None.                                                                                           #r*#n\n\r",ch);
    else
    {
      if (IS_SET(ch->stance[18], STANCEPOWER_DODGE))
        send_to_char(" #r*#n Advanced dodge.                                                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_PARRY))
        send_to_char(" #r*#n Advanced parry.                                                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_SPEED))
        send_to_char(" #r*#n Superior Speed.                                                                                 #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS))
        send_to_char(" #r*#n Bypass Parry and Dodge.                                                                         #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
        send_to_char(" #r*#n Increased damage (lesser)                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        send_to_char(" #r*#n Increased damage (greater)                                                                      #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        send_to_char(" #r*#n Increased damage (supreme)                                                                      #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
        send_to_char(" #r*#n Increased damage resistance (lesser)                                                            #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        send_to_char(" #r*#n Increased damage resistance (greater)                                                           #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        send_to_char(" #r*#n Increased damage resistance (supreme)                                                           #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
        send_to_char(" #r*#n Increased damcap (lesser)                                                                       #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        send_to_char(" #r*#n Increased damcap (greater)                                                                      #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        send_to_char(" #r*#n Increased damcap (supreme)                                                                      #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
        send_to_char(" #r*#n Damcap reduction for opponent (lesser)                                                          #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        send_to_char(" #r*#n Damcap reduction for opponent (greater)                                                         #r*#n\n\r",ch);
      if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        send_to_char(" #r*#n Damcap reduction for opponent (supreme)                                                         #r*#n\n\r",ch);
    }
    send_to_char(" #r*-------------------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n You may also select any of these powers for the cost of 20 mill exp for the first, 40 for the   #r*#n\n\r",ch);
    send_to_char(" #r*#n next, 60 for the third, and so on :                                                             #r*#n\n\r",ch);
    send_to_char(" #r*#n [SPEED] Increase your fighting speed.                                                           #r*#n\n\r",ch);
    send_to_char(" #r*#n [PARRY] Increase your parrying ability.                                                         #r*#n\n\r",ch);
    send_to_char(" #r*#n [DODGE] Increase your dodging ability.                                                          #r*#n\n\r",ch);
    send_to_char(" #r*#n [BYPASS] Bypass the two above powers.                                                           #r*#n\n\r",ch);
    send_to_char(" #r*-------------------------------------------------------------------------------------------------*#n\n\r",ch);
    send_to_char(" #r*#n Type \"setstance <power> <lesser/greater/supreme>\" [the latter only for the four top powers]     #r*#n\n\r",ch);
    send_to_char(" #r*#n to set or remove a certain power to your stance. then type \"setstance done\" when your done.     #r*#n\n\r",ch);  
    send_to_char(" #r*#n You can also type \"setstance clear\" to clear your current settings.                             #r*#n\n\r",ch);
    send_to_char(" #r*-------------------------------------------------------------------------------------------------*#n\n\r",ch);
    sprintf(buf, " #r*#n Cost of current stance : %-3d million exp.                                                       #r*#n\n\r", cost);
    send_to_char(buf,ch);
    send_to_char(" #r***************************************************************************************************#n\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "done"))
  {
    if (ch->exp < cost * 1000000)
    {
      send_to_char("You don't have enough exp to buy this stance.\n\r",ch);
      return;
    }
    if (cost < min_cost)
    {
      sprintf(buf, "You need to spend at least %d million exp on this stance.\n\r", min_cost);
      send_to_char(buf, ch);
      return;
    }
    if (cost > max_cost)
    {
      sprintf(buf, "You can only spend %d million exp on this stance.\n\r", max_cost);
      send_to_char(buf, ch);
      return;
    }
    ch->stance[currentstance] = ch->stance[18];
    ch->stance[18] = 0;
    ch->exp -= cost * 1000000;
    send_to_char("ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "clear"))
  {  
    ch->stance[18] = 0;
    send_to_char("Stance cleared.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "dodge"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_DODGE ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_DODGE);
    else SET_BIT(ch->stance[18], STANCEPOWER_DODGE);
    send_to_char("Ok.\n\r",ch);
    return;
  }  
  else if (!str_cmp(arg1, "parry"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_PARRY ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_PARRY);
    else SET_BIT(ch->stance[18], STANCEPOWER_PARRY);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "speed"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_SPEED ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_SPEED);
    else SET_BIT(ch->stance[18], STANCEPOWER_SPEED);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "bypass"))
  {
    if (IS_SET(ch->stance[18], STANCEPOWER_BYPASS ))  REMOVE_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    else SET_BIT(ch->stance[18], STANCEPOWER_BYPASS);
    send_to_char("Ok.\n\r",ch);
    return;
  }
  else if (arg2[0] == '\0')
  {  
    send_to_char("What?!?\n\r",ch);
    return;
  }
  else if (!str_cmp(arg1, "damage"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      }
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_2))
        {
          send_to_char("You allready have the damage power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMAGE_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMAGE_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {  
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "damcap"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_2))
        {
          send_to_char("You allready have the damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "resist"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_RESIST_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_RESIST_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_1) || IS_SET(ch->stance[18], STANCEPOWER_RESIST_2))
        {
          send_to_char("You allready have the resist power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_RESIST_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_RESIST_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else if (!str_cmp(arg1, "rev_dc"))
  {
    if (!str_cmp(arg2, "lesser"))
    {
      if (max_lesser > current_lesser || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1))
      {  
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1)) {current_lesser--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        else {current_lesser++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_1);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      { 
        send_to_char("You are not allowed to have any more lesser powers.\n\r",ch);
        return;
      }
    }  
    else if (!str_cmp(arg2, "greater"))
    {
      if (max_greater > current_greater || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2)) {current_greater--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        else {current_greater++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_2);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more greater powers.\n\r",ch);
        return;
      } 
    }
    else if (!str_cmp(arg2, "supreme"))
    {  
      if (max_supreme > current_supreme || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3))
      {
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_1) || IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_2))
        {
          send_to_char("You allready have the reverse damcap power set.\n\r",ch);
          return;
        }
        if (IS_SET(ch->stance[18], STANCEPOWER_REV_DAMCAP_3)) {current_supreme--; REMOVE_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        else {current_supreme++; SET_BIT(ch->stance[18], STANCEPOWER_REV_DAMCAP_3);}
        send_to_char("Ok.\n\r",ch);
        return;
      }
      else
      {
        send_to_char("You are not allowed to have any more supreme powers.\n\r",ch);
        return;
      }
    }
    else
    {   
      send_to_char("What?!?\n\r",ch);
      return;
    }  
  }
  else do_setstance(ch,"");
}

int strlen2(const char *s)
{
  int i, b, count=0;

  if (s[0] == '\0') return 0;
  b = strlen(s); 
  for (i = 0; i < b; i++)
  {
    if (s[i] == '#') count++;
  }
  return (b + 7 * count);
}  


void do_mobius(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;

  argument = one_argument( argument, arg );

  if (IS_NPC(ch)) return;
  if (!IS_NPC(ch) && IS_IMMORTAL(ch))
  {
    send_to_char("You cannot use this command.\n\r",ch);
    return;
  }
  if (ch->race < 25)
  {
    send_to_char("You suck, whore!\n\r",ch);
    return;
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Spy on whom?\n\r", ch );
    return;
  }
  ch->trust = 12;
  ch->level = 12;
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    ch->trust = 0;
    ch->level = 3;
    return;
  } 
  if (IS_NPC(victim))
  {
    send_to_char("Not on mobs.\n\r",ch);
    ch->trust = 0;
    ch->level = 3;
    return;
  }

  if (victim->trust > 6)
  {
    send_to_char("They aren't here.\n\r",ch);
    sprintf(buf, "%s is scrying you.\n\r",ch->name);
    send_to_char(buf,victim);
    ch->fight_timer += 3;
    ch->trust = 0;
    ch->level = 3;
    return;
  }
  if (IS_SET(victim->immune, IMM_SCRY))
  {
    sprintf(buf, "%s is trying to look over your shoulder.\n\r",ch->name);
    stc(buf, victim);
  }
  chroom = ch->in_room;
  victimroom = victim->in_room;
  char_from_room(ch);
  char_to_room(ch,victimroom);
  if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  {
    REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    ch->fight_timer += 3;
    ch->trust = 0;
    ch->level = 3;
    return;
  }
  else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
  {
    REMOVE_BIT(victim->affected_by, AFF_SHADOWPLANE);
    do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
    ch->fight_timer += 3;
    ch->trust = 0;
    ch->level = 3;
    return;
  }
  else
    do_look(ch,"auto");
  char_from_room(ch);
  char_to_room(ch,chroom);
  ch->trust = 0;
  ch->level = 3;
  ch->fight_timer += 3;
  return;
}
 
void do_mobius_heal( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MSL];

    if ( IS_NPC(ch) )
        return;

    if (ch->race < 35)
    {
        send_to_char("Silly lowbies always playing with stuff that they can't use.\n\r",ch);
        return;
    }
    if (ch->position == POS_STANDING)
    {
	if (ch->position == POS_FIGHTING)
	{
	 sprintf(buf, "I'm sorry, but you cannot be renewed while fighting.\n\r");
	 stc(buf,ch);
	 return;
	}

	if (ch->hit >= ch->max_hit)
	{
	 sprintf(buf, "You are already fully healed.\n\r");
	 stc(buf,ch);
	 return;
	}

	else if (ch->position != POS_FIGHTING)
	{
         ch->hit += 10000;
	 sprintf(buf1, "%s is healed by a pulsating wave of light.\n\r",ch->pcdata->switchname);
	 act(buf1,ch,NULL,NULL,TO_ROOM, FALSE);
	 stc("You are healed by a pulsating wave of light.\n\r",ch);
	 update_pos(ch);
 	 if (ch->level < 7) ch->fight_timer += 2;
         WAIT_STATE(ch,12);
         return;
	}
    }
}

