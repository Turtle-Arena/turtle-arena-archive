/*
===========================================================================
Copyright (C) 2006 Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com))

This file is part of Open Arena and is based upon Mr. Elusive's fuzzy logic
system found in Quake 3 Arena.

Open Arena is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Open Arena is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// Based on Open Arena's "gargoyle_t.c", but of the chat text was rewritten.

chat "raph"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"I am so going to enjoy this.";
		"I'm here, how the game can really start!";
		"Hey guys.";
	}

	type "game_exit"
	{
		"I need a break, I've been playing all day long.";
		"Later guys, I've got some things to do in the 'real world.'";
		"Time to go.";
		"There are better thing to do than beating up guys in video game you know. Like doing it in real life! Later.";
	}

	type "level_start"
	{
		//4,", yeah... its a okay I guess";
		//4," ...it feels like I just played this map.";
		"Let the butt kickin' begin!";
		4,", is this the map Mikey made? 'cause if it is, I not playing.";
		"Oh yeah, time to bust some heads!";
	}

	type "level_end"
	{
		"Geez, I though it never end.";
		"Boy, smashing that guy was fun!";
		//"Is this it? is it finally over?";
		"Is this it, is it over already? It was just gettin' fun!";
	}

	type "level_end_victory"
	{
		"Oh yeah, I win again!";
		"Ha, you thought you can beat me?";
		"I'd like to see you do this good! ...well not really.";
	}

	type "level_end_lose"
	{
		"How 'bout a rematch?";
		"Next time you won't be so lucky.";
		"No comment...";
		"Guess I've got some training to do.";
	}

	type "hit_talking"
	{
		"Excuse me, I was talking!";
		"You are so going to paid for that!";
		"Bonehead.";
		"Is there a sign over my head that reads 'shoot me'?";
	}

	type "hit_nodeath"
	{
		"You just don't die do you...";
		"I guess I should've trained more.";
		"How could you live through that!?";
		"I need a better weapon.";
	}

	type "hit_nokill"
	{
		"Maybe you should try using a real weapon!";
		// TODO: 1 = weapon name (fists - sai - katana - bo - ect)
		//"Do you even know how to use your ", 1, "?";
		"Nice try!";
		"Try again, I dear you!";
		"Are you even trying?";
	}

	type "death_telefrag"
	{
		"What you couldn't wait for me to move or somethin'?";
		"Grr... you stupid ", fighter ,"s!";
		"Hey bonehead, I was standing there!";
		"Grr... you wait right there buddy...";
	}

	type "death_cratered"
	{
		"Crud.";
		"Ouch, that hurt.";
		"Gr... why'd it have to be so far down.";
		"Wheres a health item when you need one?";
	}

	type "death_lava"
	{
		"Man is it hot in here!";
		"I guess swimming in lava isn't the smartist thing I've every done.";
		"I need a new travial agent.";
		"Grr... I hate burn marks on my shell!";
	}

	type "death_slime"
	{
		"I hope this slime doesn't cause a secondary mutation!"; // like what happened to donny!
		"...and who forgot to label this toxic?!";
		"How was I suppost to know it was toxic?";
		"Grr... who put this toxic waste here!";
	}

	type "death_drown"
	{
		"Sometimes I wish I was a amphibian...";
		"I need to get me an air tank or somethin'...";
		"I was just proving that turtles don't have gills.";
	}

	type "death_suicide"
	{
		"I am never going hear the end of this.";
		"Gr... that did not just happen.";
		//"...";

		// 1987 Raph...
		//"Kids don't try this at home, were trained profesionals.";
	}

/* Turtle Man: No longer used.
	type "death_gauntlet"
	{
		"...";
	}

	type "death_rail"
	{
		"...";
	}

	type "death_bfg"
	{
		"...";
	}
*/

	type "death_insult"
	{
		"All you accomplished is to tick me off!";
		"Well it sure took you long enough.";
		"I've seen Mikey do better than that.";
		curse;
	}

	type "death_praise"
	{
		"Nice job.";
		"Not bad.";
		"Good one.";
		"I think your getting the hang of this.";
	}

/* Turtle Man: No longer used.
	type "kill_rail"
	{
		"Take that!";
	}

	type "kill_gauntlet"
	{
		"Take that!";
		KILL_GAUNTLET1;
		KILL_GAUNTLET0;
	}
*/
	type "kill_telefrag"
	{
		"Oh you were standing there? -Well I am now.";
		"Why don't you stay down!";
		"Got ya!";
		TELEFRAGGED0;
		TELEFRAGGED1;
	}

	type "kill_suicide"
	{
		TAUNT1;
		TAUNT0;
	}

	type "kill_insult"
	{
		"wimp"
		"bonehead";
		//"...";
		//"...";
		KILL_EXTREME_INSULT;
		curse;
	}

	type "kill_praise"
	{
		"Not bad.";
		"It seems you getting better.";
		"You know, your pretty good for a wimp.";
		D_PRAISE1;
	}

	type "random_insult"
	{
		"Geez, your worse than Mikey.";
		"You know, you are such a bonehead!";
		"Bonehead.";
	}

	type "random_misc"
	{
		"I need to work on the shell cycle.";
		"Anyone seen a TV remote around here?";
		"Anyone want to order some pizza?";
		"Was that a monkey?";
	}
}
