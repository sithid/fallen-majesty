CC      = gcc
PROF    = -g3
NOCRYPT =
#Uncomment the next line if you want request support -DMEMWATCH
DBUGFLG = -DREQUESTS
C_FLAGS = -Wall $(PROF) $(NOCRYPT) $(DBUGFLG) -D_GNU_SOURCE
L_FLAGS = $(PROF)

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o angel.o bit.o          \
	board.o build.o changes.o clan.o class_sav.o comm.o const.o copyover.o        \
	cprintf.o daemon.o desc_map.o dom.o db.o darkheart.o data_base.o demon.o      \
	drow.o fight.o freeze.o garou.o handler.o hashstr.o hedit.o highlander.o      \
	interp.o inquisitor.o jason.o jobo.o kav_wiz.o kav_fight.o  kav_info.o        \
	kingdom.o mage.o magic.o mem.o memwatch.o monk.o monk2.o newclans.o ninja.o   \
	ocommands.o olc_act.o olc_save.o olc.o paladin.o  powerl.o psionist.o 	      \
	quote.o rot_wiz.o save.o shapeshifter.o social-edit.o special.o 	      \
	spiderdroid.o string.o tanarri.o todo.o undead_knight.o update.o 	      \
	upgrade.o vamp.o vampie.o vladdonly.o wizutil.o ww.o 			      \
	 
merc: $(O_FILES)
	rm -f dystopia
	$(CC) $(L_FLAGS) -o dystopia $(O_FILES) -lcrypt
	chmod g+w dystopia
	chmod g+w $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f *.o ; rm -f dystopia ; find ../log -type f -name '*' -delete 2>/dev/null || true
