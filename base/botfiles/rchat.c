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

// ZTM: Disabled Open Arena text, TODO: Rewrite.

["bot", "bots"] = 1
{
	"I am a bot, yes";
	"Who are you calling a bot?";
}

// SRB2.
["srb2", "Srb2", "SRB2"] = 2
{
	"SRB2 is a cool game and a nice fourm, but sadly bots aren't allowed on the fourm...";
	"Sonic Robo Blast 2!?!";
	"SRB2! Thats were I know you from! Well from SRB2JTE actually.";
}

["SRB2JTE"] = 1
{
	"SRB to JTE!?!";
	"I'm the bot from SRB2JTE. Now I must kill you.";
}

[("sing me a song ", 0) &name] = 5
{
	"No thanks, I don't sing.";
}

// Should bots yell at people who curse and use bad words and stuff?...

/*
["no", "not", "Nada", "nope", !"no place", !"no feeling."] = 5
{
	"are you sure about that?";
	"You sound so sure of yourself";
	"Really?";
	"Don't be so certain";
}

["nigger", "nigr", "wop", "kyke", "sandnigger", "sand-nigger", "beanpicker", "wetback", "raghead", "macaca", "monkey", "irish pig", "redskin", "red-skin", "red skin", "diaper head", "bean picker", "cml-jky", "camel-jockey", "ngr", "niggers", "wops", "jews", "sandniggers", "sand-niggers", "beanpickers", "wetbacks", "camel jockeys", "cml jkys", "cml jocky", "dyke", "homo", "queer", "faggot", "fag", "kyke", "kike", "dike", "dyk", "kyk", "chink"] = 1
{
	"shut the hell up you ", vicious_insult,".";
	"bigot";
	"There is no place for bigotry in the Arena";
	"you are a ", vicious_insult, ", do us all a favor and die.";
	"All colors explode the same, ", vicious_insult, ".";
	"People are people, unfortunately you are an idiot, ", vicious_insult, ".";
	"This is an equal opportunity frag fest, bigots are not welcome.";
	"Who taught you to be ignorant?";
	"Bigotry is learned behavior, please unlearn it.";
	"Who taught you to hate?";
	"Why do you hate?";
	"Has anything you have ever done made your life better?";
	immaturity01;
	vicious_insult;
	curse;
	
}

["why", !"why not"] = 8
{
	"why not?";
	"because dmn_clown said!";
	"My ", counselor, " said so.";
	botnames, " said so.";
	confused_response;
	peeps, " said so.";
}

["ha", "LOL", "laugh", "funny"] = 6
{
	"I'm not laughing.";
	"What is so funny?";
	"That wasn't funny...";
	"Sorry, I forgot to laugh";
	immaturity01;
	"hehe";
	"That was amusing";
	"Ok, that was funny";
	response_insult;
}

["fishing", !"freak fishing"] = 3
{
	"My ", family_member, " was eaten by a blue whale last ", month, ".";
	"I like to use ", substance, " as bait, but only when it is ", weather, ".";
	"I'd swear that asian carp can speak ", language, ".";
	response_insult;
}

["why not"] = 6
{
	"because ", peeps, " said.";
	"because I said";
	"because dmn_clown said so!";
	"A ", profession, " said so.";
}

["yes", "sure", "certainly"] = 2
{
	confused_response;
	neutral;
	"You sound positive.";
	"You've convinced me.";
	"Ok";
	
}

[(1, " is a ", 0) &name, !"Grism", !"Gargoyle", !"Dmo", !"Penguin", !"Sarge", !"Grunt"] = 4
{
	"That is Mizz ", 0, " to you, ", fighter, ".";
	"I'm only rude to ", fighter, "s like you.";
	"You should see me out of the Arena...";
	"That isn't what your ", family_member, " said last night";
}

["OI"] = 5
{
	"OI! OI! OI!";
}

["fuck"] = 1
{
	proposition01;
	immaturity01;
}

["shit"] = 1
{
	"I hope you washed your hands...";
	"Eeew... please flush.";
	immaturity01;
}

["ass"] = 1
{
	immaturity01;
}

["bots", "bot", !"stupid bot"] = 2
{
	"Heh... right!";
	"Bah!  Humbug...";
	"The politically correct term is 'human exterminator'";
	"What is that about?";
	"I frag, therefore I am.";
}

["stupid bot"] = 5
{
	"Yeah, bots are stupid.";
	"Man, why do they even bother with ai, it never works?";
	"Hey!  Blame the person that programmed me!";
	"What are you trying to say, Dave?";
}

["sex", "coitis", "sexual intercourse"] = 2
{
	"Now?!?";
	"No thanks, I have a headache.";
	"You want to talk about that here?";
	"This is not a chat room!";
	proposition01;
	"perv";
}

[(1, " is a ", 0) &name, !"Kyonshi", !"Major", !"Hnt", !"Dark", !"Tanisha", !"Rai", !"Slshish"] = 4
{
	"That is Mr. ", 0, " to you, ", fighter, ".";
	"I'm only rude to ", fighter, "s like you.";
	"You should see me out of the Arena...";
	"That isn't what your ", family_member, " said last night";
}

["meaning of it all"] = 5
{
	"Only what you give it.";
	"It?  It has no meaning, it just is.";
	"You think too much, shut up and frag.";
	"Has anyone ever told you to shut the hell up?";
}

["Aardappel", "leileilol", "Multiplex", "crayon", "JK Makowka", "Democritus", "jzero", "Mancubus", "mewse", "div0", "MilesTeg", "evillair", "Shadowdragon", "mightypea", "Morphed", "toddd", "Psymong", "DarkThief", "slyus", "SavageX", "Kaz", "pixie", "Vondur", "Tyrann", "Ed", "Czestmyr", "kick52", "kit89", "lazureus", "sago007", "dmn_clown"] = 1
{
	"Why do you think you have the right to utter that name, you ", fighter, "?";
	"Speak that name with respect.";
	"You would be wise to respect the name you just uttered.";
	"Shh... The Arena Lords may be listening.";
	"I am not worthy to even think that name.";
	"Silence, Mortal!  You are not worthy of uttering an Arena Lord's name.";
}

[("sing me a song ", 0) &name] = 5
{
	"No, I am not in the mood.";
	"There must be someway out of here, said the joker to the thief...";
	"Hey, Mr. Tamberine man, play a song for me, I'm not sleepy and there ~ain't no place I'm going to...";
	"She was a girl from Birmingham, she just had an abortion, she was a case of insanity, her name was Pauline, she lived in a tree...";
	"...You've got to break the chains that hold you down, crush the tyrants to the ground...";
	"...Put a bullet in my head, but you can't change a word I said, you've got the ~disease, I've got the solution...";
	"Not right now, I'm kinda busy.";
	"Sometimes I try to do things and it just don't turn out the way I want it to and I get real frustrated...";
	"...as far I'm concerned I think you really suck, you're rotten, and you really blow... I hate your guts and I wish that you was dead...";
	"... ~Don't go out tonight, it could be your life, ~there's a bathroom on the right...";
	"No life 'till leather, we're going to kick some ~ass tonight!  Oh yeah, yeah yeah!";
	"Play some Skynard, Man!";
	"...there's a lot of people sayin' we'd be better off dead, I ~don't feel like satan, but I am to them, so I try to forget it any way I can...";
	"Sorry, my voice is shot... too much whiskey.";
	"Nah...";
	"...now the sun has disappeared, all is darkness, anger, pain, and fear, twisted sightless wrecks of men go groping on their knees and cry in pain, and the sun has disappeared...";
}
*/

