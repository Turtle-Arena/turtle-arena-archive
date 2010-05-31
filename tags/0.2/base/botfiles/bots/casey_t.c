/*
===========================================================================
Copyright (C) 2006 Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com))
Copyright (C) 2010 Zack "ZTurtleMan" Middleton

This file is part of Turtle Arena and is based upon botfiles by
Dmn_clown (aka: Bob Isaac (rjisaac@gmail.com)) from Open Arena which
are based on Mr. Elusive's fuzzy logic system found in Quake 3 Arena.

Turtle Arena is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Turtle Arena is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Turtle Arena; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

chat "casey"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Time to bust up this joint!";
		"Hey, let do this!";
		//
	}

	type "game_exit"
	{
		"So long losers.";
		"Gotta run.";
		"Oops, it quitin' time.";
		"See ya' later.";
	}

	type "level_start"
	{
		"Woo Hoo!";
		//
		"Oh yeah, time to bust up this joint!";
	}

	type "level_end"
	{
		"Is it over already?";
		//"Boy, smashing that guy was fun!";
		//
		//"Aww, it was just gettin' fun!";
	}

	type "level_end_victory"
	{
		"Goongala! Goongala!";
		"Take that ya' wimps!";
		"Yeah!";
	}

	type "level_end_lose"
	{
		"Gaaa";
		"Aww come on that guy musta' cheated.";
	}

	type "hit_talking"
	{
		"Punk!";
		"Time for some head bashing!";
		"What is wrong with you, you Bonehead!";
	}

	type "hit_nodeath"
	{
		"Gaaa";
		"Just die!?";
	}

	// 
	// Below is raph chats
	//

	type "hit_nokill"
	{
		"Maybe you should try using a real weapon!";
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
		"I need a new travel agent.";
		"Grr... I hate burn marks on my shell!";
	}

	type "death_slime"
	{
		"I hope this slime doesn't cause a secondary mutation!"; // Like what happened to Donny!
		"Grr... who put this toxic waste here!";
		"Man, it feels gross in here.";
		"Yuck.";
	}

	type "death_drown"
	{
		"Goodbye water world";
		"I need to get me an air tank or somethin'...";
	}

	type "death_suicide"
	{
		"I am never going hear the end of this.";
		"Gr... that did not just happen.";
	}

	type "death_insult"
	{
		"All you accomplished is to tick me off!";
		"Well it sure took you long enough.";
		"Bonehead!";
		//curse;
	}

	type "death_praise"
	{
		"Nice job.";
		"Not bad.";
		"Good one.";
		"I think your getting the hang of this.";
	}

	type "kill_telefrag"
	{
		"Oh you were standing there?  I am now.";
		"Why don't you stay down!";
		"If ya can't beat'em, squash'em.";
		//TELEFRAGGED0;
		//TELEFRAGGED1;
	}

	type "kill_suicide"
	{
		TAUNT1;
		TAUNT0;
	}

	type "kill_insult"
	{
		"Wimp!";
		"Bonehead!";
		"Oh yeah!";
		"Now thats what I'm talkin' about.";
		//KILL_EXTREME_INSULT;
		//curse;
	}

	type "kill_praise"
	{
		"Not bad.";
		"It seems your getting better.";
		"You know, your not bad for a wimp.";
		//D_PRAISE1;
	}

	type "random_insult"
	{
		"Geez, your worse than Mikey.";
		"You know, you are such a bonehead!";
		"Bonehead!";
		"Wuss!";
	}

	type "random_misc"
	{
		"I need to work on the shell cycle.";
		"Anyone seen a TV remote around here?";
		"Anyone want to order some takeout?";
		"Was that a monkey?";
		"Have you seen a monkey around here?";
	}
}
