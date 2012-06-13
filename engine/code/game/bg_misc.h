/*
===========================================================================
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company.

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//
// bg_misc.h -- definitions shared by both the server game and client game modules

// because games can change separately from the main system version, we need a
// second version that must match between game and cgame

#define	GAME_VERSION		BASEGAME "-1"

#define	DEFAULT_GRAVITY		800
#ifndef NOTRATEDM // No gibs.
#define	GIB_HEALTH			-40
#endif
#ifndef TURTLEARENA // NOARMOR
#define	ARMOR_PROTECTION	0.66
#endif

#define	MAX_ITEMS			256

#define	RANK_TIED_FLAG		0x4000

#define DEFAULT_SHOTGUN_SPREAD	700
#define DEFAULT_SHOTGUN_COUNT	11

#define	ITEM_RADIUS			15		// item sizes are needed for client side pickup detection

#define	LIGHTNING_RANGE		768

#define	SCORE_NOT_PRESENT	-9999	// for the CS_SCORES[12] when only one player is present

#define	VOTE_TIME			30000	// 30 seconds before vote times out

#define	MINS_Z				-24
#define	DEFAULT_VIEWHEIGHT	26
#define CROUCH_VIEWHEIGHT	12
#define	DEAD_VIEWHEIGHT		-16

#ifdef TURTLEARENA // LOCKON
#define LOCKON_TIME 500 // Time it take to be fully facing target
#endif

//
// config strings are a general means of communicating variable length strings
// from the server to all connected clients.
//

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h
#define	CS_MUSIC				2
#define	CS_MESSAGE				3		// from the map worldspawn's message field
#define	CS_MOTD					4		// g_motd string for server message of the day
/* bg_public.h
	ZTM: NOTE: Server expects CS_WARMUP as 5.
*/
#define	CS_WARMUP				5		// server time when the match will be restarted
#define	CS_SCORES1				6
#define	CS_SCORES2				7
#define CS_VOTE_TIME			8
#define CS_VOTE_STRING			9
#define	CS_VOTE_YES				10
#define	CS_VOTE_NO				11

#define CS_TEAMVOTE_TIME		12
#define CS_TEAMVOTE_STRING		14
#define	CS_TEAMVOTE_YES			16
#define	CS_TEAMVOTE_NO			18

#define	CS_GAME_VERSION			20
#define	CS_LEVEL_START_TIME		21		// so the timer only shows the current level
#define	CS_INTERMISSION			22		// when 1, fraglimit/timelimit has been hit and intermission will start in a second or two
#define CS_FLAGSTATUS			23		// string indicating flag status in CTF
#define CS_SHADERSTATE			24
#define CS_BOTINFO				25

#define	CS_ITEMS				27		// string of 0's and 1's that tell which items are present
#ifdef TA_NPCSYS
#define	CS_NPCS					28
#endif

#define	CS_MODELS				32
#define	CS_SOUNDS				(CS_MODELS+MAX_MODELS)
#define	CS_PLAYERS				(CS_SOUNDS+MAX_SOUNDS)
#define CS_LOCATIONS			(CS_PLAYERS+MAX_CLIENTS)
#define CS_PARTICLES			(CS_LOCATIONS+MAX_LOCATIONS)
#ifdef TA_ENTSYS // MISC_OBJECT
#define MAX_STRINGS 256
#ifdef IOQ3ZTM // Particles
#define CS_STRINGS				(CS_PARTICLES+MAX_PARTICLES_AREAS)
#else
#define CS_STRINGS				(CS_LOCATIONS+MAX_LOCATIONS)
#endif
#endif

#ifdef TA_ENTSYS // MISC_OBJECT
#define CS_MAX					(CS_STRINGS+MAX_STRINGS)
#elif defined IOQ3ZTM // Particles
#define CS_MAX					(CS_PARTICLES+MAX_PARTICLES_AREAS)
#else
#define CS_MAX					(CS_PARTICLES+MAX_LOCATIONS)
#endif

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

typedef enum {
	GT_FFA,				// free for all
	GT_TOURNAMENT,		// one on one tournament
	GT_SINGLE_PLAYER,	// single player ffa

#if 0 // #ifdef TURTLEARENA
	GT_LMS,				// Last Man Standing
	GT_KOTH,			// King Of The Hill
	GT_KEEPAWAY,		// Keep Away
	// race, tag, and chaos?...
#endif

	//-- team games go after this --

	GT_TEAM,			// team deathmatch
#if 0 // #ifdef TURTLEARENA
	GT_LTEAMS,			// last team standing
#endif
	GT_CTF,				// capture the flag
	GT_1FCTF,
	GT_OBELISK,
#ifdef MISSIONPACK_HARVESTER
	GT_HARVESTER,
#endif
	GT_MAX_GAME_TYPE
} gametype_t;

typedef enum { GENDER_MALE, GENDER_FEMALE, GENDER_NEUTER } gender_t;

//===================================================================================

// player_state->stats[] indexes
// NOTE: may not have more than 16
typedef enum {
	STAT_HEALTH,
#ifndef TA_HOLDSYS
	STAT_HOLDABLE_ITEM,
#endif
#ifdef MISSIONPACK
	STAT_PERSISTANT_POWERUP,
#endif
#ifdef TA_WEAPSYS
	STAT_NEW_WEAPON_HANDS, // Set in PM_BeginWeaponHandsChange
	STAT_DEFAULTWEAPON, // default weapon
#endif
#ifdef TA_WEAPSYS_EX
	STAT_AMMO, // Ammo for current weapon

	STAT_PENDING_WEAPON, // Weapon to change to.
	STAT_PENDING_AMMO, // Ammo for pending weapon

	STAT_DROP_WEAPON, // Weapon to drop.
	STAT_DROP_AMMO, // Ammo for weapon to drop
#else
	STAT_WEAPONS,					// 16 bit fields
#endif
#ifndef TURTLEARENA // NOARMOR
	STAT_ARMOR,
#endif
#ifdef TURTLEARENA // NIGHTS_ITEMS
	STAT_STARS,
	STAT_SPHERES,
#endif
	STAT_DEAD_YAW,					// look this direction when dead (FIXME: get rid of?)
	STAT_CLIENTS_READY,				// bit mask of clients wishing to exit the intermission (FIXME: configstring?)
	STAT_MAX_HEALTH					// health / armor limit, changable by handicap
} statIndex_t;


// player_state->persistant[] indexes
// these fields are the only part of player_state that isn't
// cleared on respawn
// NOTE: may not have more than 16
typedef enum {
	PERS_SCORE,						// !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
	PERS_HITS,						// total points damage inflicted so damage beeps can sound on change
	PERS_RANK,						// player rank or team rank
	PERS_TEAM,						// player team
	PERS_SPAWN_COUNT,				// incremented every respawn
#if !defined NOTRATEDM || !defined TURTLEARENA // AWARDS
	PERS_PLAYEREVENTS,				// 16 bits that can be flipped for events
#endif
	PERS_ATTACKER,					// clientnum of last damage inflicter
#ifndef TURTLEARENA // NOARMOR
	PERS_ATTACKEE_ARMOR,			// health/armor of last person we attacked
#endif
	PERS_KILLED,					// count of the number of times you died
#ifdef TA_SP
	PERS_LIVES,
	PERS_CONTINUES,
#endif
	// player awards tracking
#ifndef TURTLEARENA // AWARDS
	PERS_IMPRESSIVE_COUNT,			// two railgun hits in a row
	PERS_EXCELLENT_COUNT,			// two successive kills in a short amount of time
#endif
	PERS_DEFEND_COUNT,				// defend awards
	PERS_ASSIST_COUNT,				// assist awards
#ifndef TURTLEARENA // AWARDS
	PERS_GAUNTLET_FRAG_COUNT,		// kills with the guantlet
#endif
	PERS_CAPTURES					// captures
} persEnum_t;


// entityState_t->eFlags
#define	EF_DEAD				0x00000001		// don't draw a foe marker over players with EF_DEAD
#ifdef NIGHTSMODE
#define	EF_BONUS			0x00000002		// NiGHTS mode overtime
#endif
#if (defined MISSIONPACK || defined TA_WEAPSYS) && !defined TURTLEARENA // WEAPONS
#define EF_TICKING			0x00000002		// used to make players play the prox mine ticking sound
#endif
#define	EF_TELEPORT_BIT		0x00000004		// toggled every time the origin abruptly changes
#ifndef TURTLEARENA // AWARDS
#define	EF_AWARD_EXCELLENT	0x00000008		// draw an excellent sprite
#endif
#define EF_PLAYER_EVENT		0x00000010
#define	EF_BOUNCE			0x00000010		// for missiles
#define	EF_BOUNCE_HALF		0x00000020		// for missiles
#ifndef TURTLEARENA // AWARDS
#define	EF_AWARD_GAUNTLET	0x00000040		// draw a gauntlet sprite
#endif
#define	EF_NODRAW			0x00000080		// may have an event, but no model (unspawned items)
#define	EF_FIRING			0x00000100		// for lightning gun
#ifndef TURTLEARENA // NO_KAMIKAZE_ITEM
#define	EF_KAMIKAZE			0x00000200
#endif
#ifdef TA_PATHSYS // 2DMODE
#define	EF_PATHMODE			0x00000200		// EF_KAMIKAZE
#endif
#define	EF_MOVER_STOP		0x00000400		// will push otherwise
#define EF_AWARD_CAP		0x00000800		// draw the capture sprite
#define	EF_TALK				0x00001000		// draw a talk balloon
#define	EF_CONNECTION		0x00002000		// draw a connection trouble sprite
#define	EF_VOTED			0x00004000		// already cast a vote
#ifdef TURTLEARENA // PLAYERS
#define	EF_PLAYER_WAITING	0x00008000
#endif
#ifndef TURTLEARENA // AWARDS
#define	EF_AWARD_IMPRESSIVE	0x00008000		// draw an impressive sprite
#endif
#define	EF_AWARD_DEFEND		0x00010000		// draw a defend sprite
#define	EF_AWARD_ASSIST		0x00020000		// draw a assist sprite
#ifndef IOQ3ZTM // UNUSED
#define EF_AWARD_DENIED		0x00040000		// denied
#endif
#ifdef TA_WEAPSYS
#define	EF_PRIMARY_HAND		0x00040000		// player flag for primary hand only
#endif
#define EF_TEAMVOTED		0x00080000		// already cast a team vote
#ifdef TA_SP
#define EF_FINISHED			0x00100000		// Finished level
#endif
#ifdef TURTLEARENA // LOCKON
#define EF_LOCKON			0x00200000
#endif
#ifdef IOQ3ZTM // RENDERFLAGS
#define EF_ONLY_MIRROR		0x00400000 // Only render in mirrors
#define EF_NOT_MIRROR		0x00800000 // Do not render in mirrors
#endif
#ifdef TA_ENTSYS // FUNC_USE
#define EF_USE_ENT			0x01000000 // Use entity not holdable item
#endif
#ifdef TA_PATHSYS
#define EF_TRAINBACKWARD	0x02000000
#endif
#ifdef TA_PLAYERSYS // LADDER
#define EF_LADDER			0x04000000 // On ladder
#endif

#ifdef IOQ3ZTM
#ifdef TURTLEARENA // AWARDS
// Removed EF_AWARD_GAUNTLET, EF_AWARD_IMPRESSIVE, and EF_AWARD_EXCELLENT
#define EF_AWARD_BITS ( EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP )
#else
#define EF_AWARD_BITS ( EF_AWARD_GAUNTLET | EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP )
#endif
#endif

// NOTE: may not have more than 16
typedef enum {
	PW_NONE,
#ifdef TURTLEARENA // POWERS
	// ZTM: TODO: Limited time version of PW_AMMOREGEN?
	// pw_infinity, // Green [White in TMNT3] crystal: Unlimited Shuriken for a limited period.

	// Limited time powerups

	PW_QUAD, // PW_STRENGTH: Do g_quadfactor * damge
	PW_BATTLESUIT, // PW_DEFENSE: Take half damage
	PW_HASTE, // PW_SPEED: More Speed
	PW_INVIS, // Foot Tech powerup?

	PW_FLIGHT,		// Allow player to fly around the level.

	PW_INVUL,		// Invulerrability
	PW_FLASHING,	// Given on spawn/teleport/dead, take no damge and glow/flash blue.
					// (Named after SRB2's pw_flashing.)

	PW_REDFLAG,		// Player has red flag
	PW_BLUEFLAG,	//     ...has blue...
	PW_NEUTRALFLAG,	//     ...has neutral...

	// Persistant powers.
	PW_SCOUT,		// Speed
	PW_GUARD,		// 200 health, health items give 2x health.
	PW_DOUBLER,		// Double attack power
	PW_AMMOREGEN,	// Regen ammo and shurikens --What about Melee weapons?
					//    Give melee weapons limited uses? So that ammo give no limit?
					//    Give melee weapons time-to-live? So that ammo give no limit?
	//PW_INVULNERABILITY, // TURTLEARENA Disables MISSIONPACK's PW_INVULNERABILITY
#else
	PW_QUAD,
	PW_BATTLESUIT,
	PW_HASTE,
	PW_INVIS,
	PW_REGEN,
	PW_FLIGHT,

	PW_REDFLAG,
	PW_BLUEFLAG,
	PW_NEUTRALFLAG,

	PW_SCOUT,
	PW_GUARD,
	PW_DOUBLER,
	PW_AMMOREGEN,
	PW_INVULNERABILITY,
#endif

#ifdef TURTLEARENA // DROWNING
	PW_AIR,
#endif

	PW_NUM_POWERUPS

} powerup_t;

#ifdef TA_HOLDSYS
/*
	No item in bg_itemlist can use the same HI_* tag.
	There can be a max of 16 holdable items, see MAX_HOLDABLE.
	~ZTM Nov 3, 2008
*/
#endif
typedef enum {
	HI_NONE,

#ifndef TURTLEARENA // no q3 teleprter
	HI_TELEPORTER,
#elif !defined TA_HOLDSYS
	HI_TELEPORTER_REMOVED, // Q3 want them in this order in "game" qvm
#endif
	HI_MEDKIT,
#ifndef TURTLEARENA // NO_KAMIKAZE_ITEM
	HI_KAMIKAZE,
#elif !defined TA_HOLDSYS
	HI_KAMIKAZE_REMOVED, // Q3 want them in this order in "game" qvm
#endif
#ifndef TA_HOLDSYS
	HI_PORTAL,
#endif
#ifndef TURTLEARENA // POWERS
	HI_INVULNERABILITY,
#elif !defined TA_HOLDSYS
	HI_INVULNERABILITY_REMOVED, // Q3 want them in this order in "game" qvm
#endif

#ifdef TURTLEARENA // HOLD_SHURIKEN
	// Shurikens
	HI_SHURIKEN,
	HI_ELECTRICSHURIKEN,
	HI_FIRESHURIKEN,
	HI_LASERSHURIKEN,

	// ZTM: TODO?: Make the grapple a holdable item?
	//       So that players can use the grapple and a weapon at the same time?
	// 20090316: Have the grapple in the player's secondary hand like the flag?
	// 20090409:   and use "use item" to fire?
#endif

#ifdef TA_HOLDSYS // ZTM: Moved "out of the way"
	HI_PORTAL,
#endif

	HI_NUM_HOLDABLE

#ifdef TA_HOLDSYS/*2*/
	// cg.holdableSelect is passed to server (game) in Pmove if it
	//  is *not* HI_NO_SELECT the player's holdableIndex is set to it.
	,HI_NO_SELECT = 255 // Used by cgame to disable overriding Pmove change
#endif
} holdable_t;

#ifdef TA_HOLDSYS
// Hold a max of 99 of each shuriken type. (Or any other holdable).
#define MAX_SHURIKENS 99
#endif

#ifndef TA_WEAPSYS
typedef enum {
	WP_NONE,

	WP_GAUNTLET,
	WP_MACHINEGUN,
	WP_SHOTGUN,
	WP_GRENADE_LAUNCHER,
	WP_ROCKET_LAUNCHER,
	WP_LIGHTNING,
	WP_RAILGUN,
	WP_PLASMAGUN,
	WP_BFG,
	WP_GRAPPLING_HOOK,
#ifdef MISSIONPACK
	WP_NAILGUN,
	WP_PROX_LAUNCHER,
	WP_CHAINGUN,
#endif

	WP_NUM_WEAPONS
} weapon_t;
#endif

// gitem_t->type
typedef enum {
	IT_BAD,
	IT_WEAPON,				// EFX: rotate + upscale + minlight
	IT_AMMO,				// EFX: rotate
#ifdef TURTLEARENA // NIGHTS_ITEMS
	IT_SCORE,				// EFX: rotate + no bob + no pickup message
#endif
#ifndef TURTLEARENA // NOARMOR
	IT_ARMOR,				// EFX: rotate + minlight
#endif
	IT_HEALTH,				// EFX: static external sphere + rotating internal
	IT_POWERUP,				// instant on, timer based
							// EFX: rotate + external ring that rotates
	IT_HOLDABLE,			// single use, holdable item
							// EFX: rotate + bob
	IT_PERSISTANT_POWERUP,
	IT_TEAM
} itemType_t;

#define MAX_ITEM_MODELS 4

#ifdef TA_ITEMSYS
typedef struct bg_iteminfo_s {
	char		classname[MAX_QPATH];	// spawning name
	char		pickup_sound[MAX_QPATH];
	char		world_model[MAX_ITEM_MODELS][MAX_QPATH];

	char		icon[MAX_QPATH];
	char		pickup_name[MAX_QPATH];	// for printing on pickup

	int			quantity;		// for ammo how much, or duration of powerup
	itemType_t  giType;			// IT_* flags

	int			giTag;

#ifdef IOQ3ZTM // FLAG_MODEL
	char		skin[MAX_QPATH];			// So flags don't need multiple models.
#endif

	// ZTM: NOTE: precaches is unused
	//char		precaches[MAX_STRING_CHARS];		// string of all models and images this item will use

	char		sounds[MAX_STRING_CHARS];		// string of all sounds this item will use
} bg_iteminfo_t;

extern bg_iteminfo_t bg_iteminfo[MAX_ITEMS];

int BG_ItemNumForItem( bg_iteminfo_t *item );
bg_iteminfo_t *BG_ItemForItemNum( int itemNum );
int BG_ItemIndexForName(const char *classname);
int BG_NumItems(void);
int BG_NumHoldableItems(void);

void BG_InitItemInfo(void);

#define gitem_s bg_iteminfo_s
#define gitem_t bg_iteminfo_t
#else
typedef struct gitem_s {
	char		*classname;	// spawning name
	char		*pickup_sound;
	char		*world_model[MAX_ITEM_MODELS];

	char		*icon;
	char		*pickup_name;	// for printing on pickup

	int			quantity;		// for ammo how much, or duration of powerup
	itemType_t  giType;			// IT_* flags

	int			giTag;

#ifdef IOQ3ZTM // FLAG_MODEL
	char		*skin;			// So flags don't need multiple models.
#else
	char		*precaches;		// string of all models and images this item will use
#endif
	char		*sounds;		// string of all sounds this item will use
} gitem_t;
#endif

#ifdef TA_WEAPSYS
// Currently only support two hands
typedef enum
{
	HAND_PRIMARY,
	HAND_SECONDARY,
#if 0
	HAND_THIRD,
	HAND_FOURTH,
#endif
	MAX_HANDS
} hand_e;

// hand bits, usally used in weaponHands
typedef enum
{
	HB_NONE			= 0,
	HB_PRIMARY		= 1,
	HB_SECONDARY	= 2,
	HB_BOTH			= 3, // (HB_PRIMARY|HB_SECONDARY)
#if 0 // ZTM: NOTE: May have to update weaponHands sent bits over network, add more MOD_WEAPON_*, and stuff.
	HB_THIRD		= 4,
	HB_FOURTH		= 8,
	HB_ALL			= 15, // (HB_PRIMARY|HB_SECONDARY|HB_THIRD|HB_FOURTH)
	HB_MAX			= 16 // (HB_PRIMARY|HB_SECONDARY|HB_THIRD|HB_FOURTH)+1
#else
	HB_MAX			= 4 // (HB_PRIMARY|HB_SECONDARY)+1
#endif
} handbits_e;

#define HAND_TO_HB(hand) (1<<(hand))
//#define HB_TO_HAND(handbit) (1>>(handbit))

// handSide
#define HS_CENTER 0
#define HS_RIGHT 1
#define HS_LEFT 2

//projectile trail types
#define PT_NONE 0
#define PT_PLASMA 1
#define PT_ROCKET 2
#define PT_GRENADE 3
#define PT_GRAPPLE 4
#define PT_NAIL 5
#define PT_LIGHTNING 6
#define PT_RAIL 7
#define PT_BULLET 8
#define PT_SPARKS 9 // Laser Shuriken
//projectile death types
#define PD_NONE 0
#define PD_PLASMA 1
#define PD_ROCKET 2
#define PD_GRENADE 3
#define PD_BULLET 4
#define PD_RAIL 5
#define PD_BFG 6
#define PD_LIGHTNING 7
#define PD_ROCKET_SMALL 8
#define PD_BULLET_COLORIZE 9
#define PD_NONE_EXP_PLAYER 10 // ZTM: NOTE: hacky fix to allow nailgun to explode on players in CG_MissileHitPlayer
//projectile explosion types
#define PE_NORMAL 0 // default (explode after timetolive)
#define PE_NONE 1
#define PE_PROX 2 // (stickOnImpact prox mine)
//projectile spin types
#define PS_ROLL 0 // default
#define PS_NONE 1
#define PS_PITCH 2 // ...Shurikens!
#define PS_YAW 3
//projectile bounce types
#define PB_NONE 0
#define PB_HALF 1
#define PB_FULL 2
//projectile flags
#define PF_HITMARK_FADE_ALPHA 1
#define PF_HITMARK_COLORIZE 2
#define PF_USE_GRAVITY 4
#define PF_IMPACTMARK_FADE_ALPHA 8
#define PF_IMPACTMARK_COLORIZE 16
//projectile stickOnImpact
#define PSOI_NONE 0
#define PSOI_KEEP_ANGLES 1 // Used by shurikens
#define PSOI_ANGLE_180 2
#define PSOI_ANGLE_90 3
#define PSOI_ANGLE_0 4
#define PSOI_ANGLE_270 5
// missile stuff (func, model, etc)
typedef struct
{
	char name[MAX_QPATH];
	int damage;
	int splashDamage;
	int splashRadius;
	int speed; // range for instant damage projectiles
	int flags;
	int trailType;
	char trailShaderName[2][MAX_QPATH]; // PT_RAIL uses two shaders.
	int trailRadius;
	int trailTime;
	int deathType;
	// fire_nail nailgun
	int spdRndAdd; // Speed random add
	int spread;
	int numProjectiles;

	// Hit marks and sounds
	char hitMarkName[MAX_QPATH];
	int  hitMarkRadius;
	char hitSoundName[3][MAX_QPATH];
	char hitPlayerSoundName[MAX_QPATH];
	char hitMetalSoundName[MAX_QPATH];

	// Missile (non-instant) only variables
	char model[MAX_QPATH];
	char modelBlue[MAX_QPATH];
	char modelRed[MAX_QPATH];
	int timetolive;
	int explosionType;
	qboolean shootable;
	int missileDlight;
	vec3_t missileDlightColor;
	char missileSoundName[MAX_QPATH];
	char sprite[MAX_QPATH];
	int spriteRadius;
	int spinType;
	int bounceType;
	int maxBounces;
	int stickOnImpact;
	qboolean fallToGround;
	qboolean grappling;
	int homing;
	// if "homing" > 0 then projectile is a homing missile,
	//   homing is also used as the delay (in msec) before homing in on a player.
	qboolean damageAttacker; // If true, the missile can damage the player who fired it

	char triggerSoundName[MAX_QPATH]; // PE_PROX trigger sound
	char tickSoundName[MAX_QPATH]; // PE_PROX tick sound

	// Impact mark and sounds
	char impactMarkName[MAX_QPATH];
	int impactMarkRadius;
	char impactSoundName[3][MAX_QPATH];
	char impactPlayerSoundName[MAX_QPATH];
	char impactMetalSoundName[MAX_QPATH];

	// Instant damage and instant only variables
	qboolean instantDamage;
	int maxHits; // Projectiles can go through players... currently instantDamage only

} bg_projectileinfo_t;

// Weapon type
typedef enum
{
    WT_NONE, // Dummy type

	WT_GAUNTLET,
    WT_GUN,
	WT_MELEE,
    WT_MAX

} weapontype_t;

#define TRAIL_NONE 0
#define TRAIL_DEFAULT 1
typedef struct
{
	int damage;			///< Damage to give
	vec3_t origin;		///< Origin of the blade
	vec3_t tip;			///< Tip of the blade
	int trailStyle;		///< TRAIL_* defines
} bg_bladeinfo_t;

// Melee-specific
#define WIF_IMPACTMARK_FADE_ALPHA	1
#define WIF_IMPACTMARK_COLORIZE		2
#define WIF_ALWAYS_DAMAGE			4 // Do damage even while not attacking, like a lightsaber.
#define WIF_CUTS					8 // Swords can cuts things that a bo, nunchunk, and sai can not.
// Guns-specific
#define WIF_CONTINUOUS_FLASH		16
// Non-specific
// Eject "brass" functions
#define WIF_EJECT_BRASS				32		// CG_MachineGunEjectBrass
#define WIF_EJECT_BRASS2			64		// CG_ShotgunEjectBrass
#define WIF_EJECT_SMOKE				128		// CG_NailgunEjectBrass
#define WIF_EJECT_SMOKE2			256		// Shotgun smoke
//
#define WIF_BARREL_IDLE_USE_GRAVITY	512
#define WIF_INITIAL_EFFECT_ONLY		1024	// when holding the attack button the flash sound
											// and brass eject would happen only on the first press.

// Barrel Spin
#define BS_PITCH PITCH // 0
#define BS_YAW YAW // 1
#define BS_ROLL ROLL // 2
#define BS_NONE 3
#define BS_MAX 4

#define MAX_WEAPON_BLADES 5 // Katana use 1, Bo use 2, Sai use 3, etc
typedef struct
{
	char name[MAX_QPATH];
	char model[MAX_QPATH];

	// shared by guns and melee weapons
	weapontype_t weapontype;
	int mod;			///< Means of Death (MOD_* enum)
	int attackDelay;
	int barrelSpin; // BS_*
	vec3_t flashColor;

	// sounds
	char flashSoundName[4][MAX_QPATH];

	// bit flags
	int flags; // WIF_ *Weapon Info Flag*

	// gun only
	int splashMod;
	bg_projectileinfo_t *proj;
	int projnum; // bg_projectileinfo[projnum]
	vec3_t aimOffset; // aimOffset is the weapon aim offset for bots

	// melee only
	bg_bladeinfo_t blades[MAX_WEAPON_BLADES];
	char impactMarkName[MAX_QPATH];
	int  impactMarkRadius;
	char impactSoundName[3][MAX_QPATH];
	char impactPlayerSoundName[MAX_QPATH];
	char impactMetalSoundName[MAX_QPATH];

} bg_weaponinfo_t;

#define MAX_WG_ATK_ANIMS 5 // Max Weapon Group attack animations
typedef struct
{
	int standAnim;
	int attackAnim[MAX_WG_ATK_ANIMS];
	unsigned int numAttackAnims;
} bg_weapongroup_anims_t;

typedef struct
{
	char name[MAX_QPATH]; // Example; "wp_none"

	qboolean randomSpawn; // If qtrue (default) spawn in weapon_random

	// Item info
	bg_iteminfo_t *item;

	// Models
	char handsModelName[MAX_QPATH]; // Model's tags are used to position weapons in first person

	// Sounds
	char readySoundName[MAX_QPATH];
	char firingSoundName[MAX_QPATH];
	char firingStoppedSoundName[MAX_QPATH];

	// Weapon pointers
	bg_weaponinfo_t *weapon[MAX_HANDS];
	int weaponnum[MAX_HANDS];

	// Animations
	bg_weapongroup_anims_t normalAnims; // Normal set of animations
	bg_weapongroup_anims_t primaryAnims; // Set of animations while holding flag

} bg_weapongroupinfo_t;

// Default weapon if animation.cfg doesn't set one.
#ifdef TURTLEARENA // WEAPONS
#define DEFAULT_DEFAULT_WEAPON "WP_FISTS"
#else
#define DEFAULT_DEFAULT_WEAPON "WP_GAUNTLET" // "WP_MACHINEGUN"
#endif

// WP_DEFAULT will need to be remapped to the default weapon.
#define WP_DEFAULT	-1
#define WP_NONE		0
#define weapon_t	int

#ifdef TA_WEAPSYS_EX
#define MAX_BG_PROJ 64
#define MAX_BG_WEAPONS 64
#define MAX_BG_WEAPON_GROUPS 32
#else
#define MAX_BG_PROJ 32
#define MAX_BG_WEAPONS 32
#define MAX_BG_WEAPON_GROUPS 16 // ZTM: WONTFIX: Player's are limited to 16 weapons.
#endif
extern bg_projectileinfo_t bg_projectileinfo[MAX_BG_PROJ];
extern bg_weaponinfo_t bg_weaponinfo[MAX_BG_WEAPONS];
extern bg_weapongroupinfo_t bg_weapongroupinfo[MAX_BG_WEAPON_GROUPS];
#ifdef TURTLEARENA // HOLD_SHURIKEN
int BG_ProjectileIndexForHoldable(int holdable);
#endif
int BG_ProjectileIndexForName(const char *name);
int BG_WeaponIndexForName(const char *name);
int BG_WeaponGroupIndexForName(const char *name);
int BG_NumProjectiles(void);
int BG_NumWeapons(void);
int BG_NumWeaponGroups(void);

qboolean BG_PlayerRunning(vec3_t velocity);
int BG_MaxAttackIndex(playerState_t *ps);
int BG_AttackIndexForPlayerState(playerState_t *ps);

int BG_WeaponHandsForPlayerState(playerState_t *ps);
int BG_WeaponHandsForWeaponNum(weapon_t weaponnum);
qboolean BG_WeapTypeIsMelee(weapontype_t wt);
qboolean BG_WeaponHasMelee(weapon_t weaponnum);
qboolean BG_WeaponHasType(weapon_t weaponnum, weapontype_t wt);
qboolean BG_WeapUseAmmo(weapon_t w);
int BG_WeaponGroupTotalDamage(int weaponGroup);
#endif
#ifdef TA_HOLDSYS
int BG_ItemNumForHoldableNum(holdable_t holdablenum);
#endif

// reward sounds (stored in ps->persistant[PERS_PLAYEREVENTS])
#ifndef TURTLEARENA // AWARDS
#define	PLAYEREVENT_DENIEDREWARD		0x0001
#define	PLAYEREVENT_GAUNTLETREWARD		0x0002
#endif
#ifndef NOTRATEDM // Disable strong lang.
#define PLAYEREVENT_HOLYSHIT			0x0004
#endif

// entityState_t->event values
// entity events are for effects that take place reletive
// to an existing entities origin.  Very network efficient.

// two bits at the top of the entityState->event field
// will be incremented with each change in the event so
// that an identical event started twice in a row can
// be distinguished.  And off the value with ~EV_EVENT_BITS
// to retrieve the actual event number
#define	EV_EVENT_BIT1		0x00000100
#define	EV_EVENT_BIT2		0x00000200
#define	EV_EVENT_BITS		(EV_EVENT_BIT1|EV_EVENT_BIT2)

#define	EVENT_VALID_MSEC	300

typedef enum {
	EV_NONE,

	EV_FOOTSTEP,
	EV_FOOTSTEP_METAL,
	EV_FOOTSPLASH,
	EV_FOOTWADE,
	EV_SWIM,

	EV_STEP_4,
	EV_STEP_8,
	EV_STEP_12,
	EV_STEP_16,

	EV_FALL_SHORT,
	EV_FALL_MEDIUM,
	EV_FALL_FAR,

	EV_JUMP_PAD,			// boing sound at origin, jump sound on player

	EV_JUMP,
	EV_WATER_TOUCH,	// foot touches
	EV_WATER_LEAVE,	// foot leaves
	EV_WATER_UNDER,	// head touches
	EV_WATER_CLEAR,	// head leaves

	EV_ITEM_PICKUP,			// normal item pickups are predictable
	EV_GLOBAL_ITEM_PICKUP,	// powerup / team sounds are broadcast to everyone

#ifdef TA_WEAPSYS_EX
	EV_DROP_WEAPON,
#else
	EV_NOAMMO,
#endif
	EV_CHANGE_WEAPON,
	EV_FIRE_WEAPON,

	EV_USE_ITEM0,
	EV_USE_ITEM1,
	EV_USE_ITEM2,
	EV_USE_ITEM3,
	EV_USE_ITEM4,
	EV_USE_ITEM5,
	EV_USE_ITEM6,
	EV_USE_ITEM7,
	EV_USE_ITEM8,
	EV_USE_ITEM9,
	EV_USE_ITEM10,
	EV_USE_ITEM11,
	EV_USE_ITEM12,
	EV_USE_ITEM13,
	EV_USE_ITEM14,
	EV_USE_ITEM15,

	EV_ITEM_RESPAWN,
	EV_ITEM_POP,
	EV_PLAYER_TELEPORT_IN,
	EV_PLAYER_TELEPORT_OUT,

#ifdef TA_WEAPSYS
	EV_PROJECTILE_BOUNCE,
	EV_PROJECTILE_STICK,
	EV_PROJECTILE_TRIGGER,
#else
	EV_GRENADE_BOUNCE,		// eventParm will be the soundindex
#endif

	EV_GENERAL_SOUND,
	EV_GLOBAL_SOUND,		// no attenuation
	EV_GLOBAL_TEAM_SOUND,

#ifndef TA_WEAPSYS
	EV_BULLET_HIT_FLESH,
	EV_BULLET_HIT_WALL,
#endif

	EV_MISSILE_HIT,
	EV_MISSILE_MISS,
	EV_MISSILE_MISS_METAL,
	EV_RAILTRAIL,
#ifndef TA_WEAPSYS
	EV_SHOTGUN,
#endif

#ifdef TA_WEAPSYS
	EV_WEAPON_HIT,
	EV_WEAPON_MISS,
	EV_WEAPON_MISS_METAL,
#endif

	EV_PAIN,
	EV_DEATH1,
	EV_DEATH2,
	EV_DEATH3,
	EV_OBITUARY,

	EV_POWERUP_QUAD,
	EV_POWERUP_BATTLESUIT,
	EV_POWERUP_REGEN,
#ifdef TURTLEARENA // POWERS
	EV_POWERUP_INVUL,
#endif

#ifndef NOTRATEDM // No gibs.
	EV_GIB_PLAYER,			// gib a previously living player
#endif
	EV_SCOREPLUM,			// score plum
#ifdef TURTLEARENA // NIGHTS_ITEMS
	EV_CHAINPLUM,
#endif
#ifdef TA_ENTSYS // BREAKABLE MISC_OBJECT
	EV_SPAWN_DEBRIS,
	EV_EXPLOSION,
#endif

//#ifdef MISSIONPACK
#ifndef TA_WEAPSYS
	EV_PROXIMITY_MINE_STICK,
	EV_PROXIMITY_MINE_TRIGGER,
#endif
#ifndef TURTLEARENA // NO_KAMIKAZE_ITEM
	EV_KAMIKAZE,			// kamikaze explodes
#endif
	EV_OBELISKEXPLODE,		// obelisk explodes
	EV_OBELISKPAIN,			// obelisk is in pain
#ifndef TURTLEARENA  // POWERS
	EV_INVUL_IMPACT,		// invulnerability sphere impact
	EV_JUICED,				// invulnerability juiced effect
	EV_LIGHTNINGBOLT,		// lightning bolt bounced of invulnerability sphere
#endif
//#endif

	EV_DEBUG_LINE,
#ifdef IOQ3ZTM // DEBUG_ORIGIN
	EV_DEBUG_ORIGIN,
#endif
	EV_STOPLOOPINGSOUND,
	EV_TAUNT,
	EV_TAUNT_YES,
	EV_TAUNT_NO,
	EV_TAUNT_FOLLOWME,
	EV_TAUNT_GETFLAG,
	EV_TAUNT_GUARDBASE,
	EV_TAUNT_PATROL,

#ifdef IOQ3ZTM
	EV_MAX
#endif

} entity_event_t;


typedef enum {
	GTS_RED_CAPTURE,
	GTS_BLUE_CAPTURE,
	GTS_RED_RETURN,
	GTS_BLUE_RETURN,
	GTS_RED_TAKEN,
	GTS_BLUE_TAKEN,
	GTS_REDOBELISK_ATTACKED,
	GTS_BLUEOBELISK_ATTACKED,
	GTS_REDTEAM_SCORED,
	GTS_BLUETEAM_SCORED,
	GTS_REDTEAM_TOOK_LEAD,
	GTS_BLUETEAM_TOOK_LEAD,
	GTS_TEAMS_ARE_TIED,
#ifndef TURTLEARENA // NO_KAMIKAZE_ITEM
	GTS_KAMIKAZE
#endif
} global_team_sound_t;

// animations
#ifdef TURTLEARENA // PLAYERS
// ZTM: NOTE: In animation.cfg I call some animations by other names;
// * TORSO_ATTACK_GUN is TORSO_ATTACK
// * TORSO_ATTACK_GAUNTLET is TORSO_ATTACK2
// * TORSO_STAND_GUN is TORSO_STAND
// * TORSO_STAND_GAUNTLET is TORSO_STAND2
#endif
#ifdef TA_WEAPSYS
// ZTM: NOTE: All of the STAND and ATTACK animations can be TORSO_* or BOTH_*.
//            Attacking while running only animates torso.
#endif
typedef enum {
	BOTH_DEATH1,
	BOTH_DEAD1,
	BOTH_DEATH2,
	BOTH_DEAD2,
	BOTH_DEATH3,
	BOTH_DEAD3,

	TORSO_GESTURE,

	TORSO_ATTACK,
	TORSO_ATTACK2,

	TORSO_DROP,
	TORSO_RAISE,

	TORSO_STAND,
	TORSO_STAND2,

	LEGS_WALKCR,
	LEGS_WALK,
	LEGS_RUN,
	LEGS_BACK,
	LEGS_SWIM,

	LEGS_JUMP,
	LEGS_LAND,

	LEGS_JUMPB,
	LEGS_LANDB,

	LEGS_IDLE,
	LEGS_IDLECR,

	LEGS_TURN,

	TORSO_GETFLAG,
	TORSO_GUARDBASE,
	TORSO_PATROL,
	TORSO_FOLLOWME,
	TORSO_AFFIRMATIVE,
	TORSO_NEGATIVE,

#ifdef TURTLEARENA // PLAYERS
	// Place default weapons somewhere on there person while there not used.
	// TORSO_***DEFAULT_SECONDARY for Don should be
	//  switching to/from two handed Bo to using one hand.
	// ZTM: NOTE: Currently first half to going to weapon spot, second half is moving back.
	TORSO_PUTDEFAULT_BOTH,
	TORSO_PUTDEFAULT_PRIMARY,
	TORSO_PUTDEFAULT_SECONDARY,
	TORSO_GETDEFAULT_BOTH,
	TORSO_GETDEFAULT_PRIMARY,
	TORSO_GETDEFAULT_SECONDARY,

	// Pickup weapons should support CTF flags too?
	// TORSO_PUT_SECONDARY,
	// TORSO_GET_SECONDARY,

	// Gun-type standing animations
	//TORSO_STAND_GAUNTLET, // Would be the same as TORSO_STAND2...
    //TORSO_STAND_GUN, // Would be the same as TORSO_STAND...
    TORSO_STAND_GUN_PRIMARY, // I could reuse TORSO_STAND2 even though its for the gauntlet...

	// Melee weapon standing animations
    TORSO_STAND_SWORD1_BOTH,
    TORSO_STAND_SWORD1_PRIMARY,

    TORSO_STAND_SWORD2,
    TORSO_STAND_DAISHO,

    TORSO_STAND_SAI2,
    TORSO_STAND_SAI1_PRIMARY,

    TORSO_STAND_BO,
    TORSO_STAND_BO_PRIMARY,

    TORSO_STAND_HAMMER,
    TORSO_STAND_HAMMER_PRIMARY,

    TORSO_STAND_NUNCHUCKS,
    TORSO_STAND_NUNCHUCKS1_PRIMARY,

	// Gun attacks
    //TORSO_ATTACK_GAUNTLET, // Would be the same as TORSO_ATTACK2...
    //TORSO_ATTACK_GUN, // Would be the same as TORSO_ATTACK...
    TORSO_ATTACK_GUN_PRIMARY, // Can't reuse TORSO_ATTACK2 needs a new animation.

	// Melee attacks
	// * Standing attacks A B A C
	// * Running attacks A B (repeat)

    TORSO_ATTACK_SWORD1_BOTH_A,
    TORSO_ATTACK_SWORD1_BOTH_B,
    TORSO_ATTACK_SWORD1_BOTH_C,

    TORSO_ATTACK_SWORD1_PRIMARY_A,
    TORSO_ATTACK_SWORD1_PRIMARY_B,
    TORSO_ATTACK_SWORD1_PRIMARY_C,

    TORSO_ATTACK_SWORD2_A,
    TORSO_ATTACK_SWORD2_B,
    TORSO_ATTACK_SWORD2_C,

    TORSO_ATTACK_DAISHO_A,
    TORSO_ATTACK_DAISHO_B,
    TORSO_ATTACK_DAISHO_C,

    TORSO_ATTACK_SAI2_A,
    TORSO_ATTACK_SAI2_B,
    TORSO_ATTACK_SAI2_C,
    TORSO_ATTACK_SAI1_PRIMARY_A,
    TORSO_ATTACK_SAI1_PRIMARY_B,
    TORSO_ATTACK_SAI1_PRIMARY_C,

    TORSO_ATTACK_BO_A,
    TORSO_ATTACK_BO_B,
    TORSO_ATTACK_BO_C,
    TORSO_ATTACK_BO_PRIMARY_A,
    TORSO_ATTACK_BO_PRIMARY_B,
    TORSO_ATTACK_BO_PRIMARY_C,

    // Hammer is special. ( If changed update BG_SetDefaultAnimation ! )
    TORSO_ATTACK_HAMMER_A,
    TORSO_ATTACK_HAMMER_PRIMARY_A,

    TORSO_ATTACK_NUNCHUCKS_A,
    TORSO_ATTACK_NUNCHUCKS_B,
    TORSO_ATTACK_NUNCHUCKS_C,
    TORSO_ATTACK_NUNCHUCKS1_PRIMARY_A,
    TORSO_ATTACK_NUNCHUCKS1_PRIMARY_B,
    TORSO_ATTACK_NUNCHUCKS1_PRIMARY_C,
    // NOTE: If more animations are added update BG_PlayerAttackAnim

	BOTH_LADDER_STAND,
	BOTH_LADDER_UP,
	BOTH_LADDER_DOWN,

	LEGS_JUMPB_LOCKON,
	LEGS_LANDB_LOCKON,

	// If player idles long enough, switch to waiting animation.
	BOTH_WAITING,
#endif

	MAX_ANIMATIONS,

#ifdef IOQ3ZTM // ZTM: There is no "MAX_ANIMATIONS" animation
	LEGS_BACKCR = MAX_ANIMATIONS,
#else
	LEGS_BACKCR,
#endif
	LEGS_BACKWALK,
#ifndef IOQ3ZTM // FLAG_ANIMATIONS
	FLAG_RUN,
	FLAG_STAND,
	FLAG_STAND2RUN,
#endif

	MAX_TOTALANIMATIONS
} animNumber_t;


#ifdef TA_MISC // MATERIALS
typedef enum
{
	MT_NONE,
	MT_DIRT,
	MT_GRASS,
	MT_WOOD,
	MT_STONE,
	MT_METAL,
	MT_SPARKS,
	MT_GLASS,

	NUM_MATERIAL_TYPES

} materialType_t;

typedef struct
{
	char	*name;
	int		surfaceFlag;
} materialInfo_t;

extern materialInfo_t materialInfo[NUM_MATERIAL_TYPES];
#endif

#ifdef TA_ENTSYS // MISC_OBJECT
// TA_NPCSYS: Have Misc_Objects and NPCs use the same animations
// Keep in sycn with bg_misc's objectAnimationDefs
typedef enum
{
	OBJECT_NONE = -1, // Just one frame, 0.

	// Misc_object: Undamaged.
	// NPC: Standing; Unaware of player(s)
	OBJECT_IDLE,

	// Misc_object: Damage levels and random dead animation
	// NPC: Death and dead.
	OBJECT_DEATH1,
	OBJECT_DEATH2,
	OBJECT_DEATH3,
	OBJECT_DEAD1,
	OBJECT_DEAD2,
	OBJECT_DEAD3,

	OBJECT_LAND, // ZTM: TODO: misc_object hit ground.
	OBJECT_PAIN, // ZTM: TODO: "Pain" animation of misc_object with no health (Dead).

#ifdef TA_NPCSYS
	// Animations only used by NPCs
	OBJECT_TAUNT,
	OBJECT_ATTACK_FAR,
	OBJECT_ATTACK_MELEE,
	OBJECT_STANDING_ACTIVE,
	OBJECT_WALK,
	OBJECT_RUN,
	OBJECT_BACKPEDAL,
	OBJECT_JUMP,
#endif

	MAX_MISC_OBJECT_ANIMATIONS
} miscObjectAnim_t;

// Only cgame and ui need this, game doesn't use sounds. (See Sounds_Parse)
#define MAX_RAND_SOUNDS 5
typedef struct
{
	int anim; // Such as value of BOTH_DEATH1
	int frame; // frame in the animation
	int numSounds; // play sounds[rand()%numSounds]
	char name[MAX_QPATH]; // base name of sound(s), if it has a "%d" it will be replaced for rand
	int start; // random sound index start number
	int end; // random sound index end number
	int chance; // 0 = always, 100 = never { if (rand()%100 >= chance; play sound; }

	int prefixType; // prefixType_e
	sfxHandle_t sounds[MAX_RAND_SOUNDS]; // Play random sound
} bg_sounddef_t;

// Currently not used by players...
#define MAX_BG_SOUNDS MAX_TOTALANIMATIONS
//#define MAX_BG_SOUNDS MAX_MISC_OBJECT_ANIMATIONS*2
typedef struct
{
	int numSounds;
	bg_sounddef_t sounddefs[MAX_BG_SOUNDS];
} bg_sounds_t;
#endif


#ifdef TA_PLAYERSYS
typedef enum
{
	AP_NONE = 0,
	AP_TORSO = 1,
	AP_LEGS = 2,
	AP_BOTH = (AP_TORSO|AP_LEGS),
	AP_OBJECT = 4 // NPCs and misc_objects
} prefixType_e;
#endif

typedef struct animation_s {
	int		firstFrame;
	int		numFrames;
	int		loopFrames;			// 0 to numFrames
	int		frameLerp;			// msec between frames
	int		initialLerp;		// msec to get to first frame
	int		reversed;			// true if animation is reversed
	int		flipflop;			// true if animation should flipflop back to base
#ifdef TA_PLAYERSYS
	int		prefixType;
#endif
} animation_t;

// flip the togglebit every time an animation
// changes so a restart of the same anim can be detected
#define	ANIM_TOGGLEBIT		128

#ifdef IOQ3ZTM // LERP_FRAME_CLIENT_LESS
// ZTM: This was moved to BG as TA_WEAPSYS_1 needs it in game

// when changing animation, set animationTime to frameTime + lerping time
// The current lerp will finish out, then it will lerp to the new animation
typedef struct {
	int			oldFrame;
	int			oldFrameTime;		// time when ->oldFrame was exactly on

	int			frame;
	int			frameTime;			// time when ->frame will be exactly on

	float		backlerp;

	float		yawAngle;
	qboolean	yawing;
	float		pitchAngle;
	qboolean	pitching;

	int			animationNumber;	// may include ANIM_TOGGLEBIT
	animation_t	*animation;
	int			animationTime;		// time when the first frame of the animation will be exact
} lerpFrame_t;

// ZTM: for time use;
// * cgame - cg.time
// * ui - dp_realtime
// * game - level.time
void BG_ClearLerpFrame(lerpFrame_t * lf, animation_t *animations, int animationNumber, int time );
void BG_SetLerpFrameAnimation( lerpFrame_t *lf, animation_t *animations, int newAnimation );
void BG_RunLerpFrame( lerpFrame_t *lf, animation_t *animations, int newAnimation, int time, float speedScale );
#endif
#ifdef IOQ3ZTM // BG_SWING_ANGLES
#define BG_SWINGSPEED 0.3f // default swing speed // cg_swingSpeed.value
void BG_SwingAngles( float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging, int frametime );
#endif

#ifdef TA_PLAYERSYS
#ifdef TURTLEARENA // DEFAULT_PLAYER
#define	DEFAULT_MODEL			"raph"
#define DEFAULT_MODEL_FEMALE	"raph" // ZTM: TODO: Add female model/sounds
#define	DEFAULT_TEAM_MODEL		"raph"
#define	DEFAULT_TEAM_HEAD		"raph"
#define DEFAULT_TEAM_MODEL_FEMALE	"raph" // ZTM: TODO: Add female model/sounds
#else // Q3
#define	DEFAULT_MODEL			"sarge"
#define	DEFAULT_MODEL_FEMALE	"crash"
#ifdef MISSIONPACK
#define	DEFAULT_TEAM_MODEL		"james"
#define	DEFAULT_TEAM_HEAD		"*james"
#define	DEFAULT_TEAM_MODEL_FEMALE	"janet"
#else
#define	DEFAULT_TEAM_MODEL		"sarge"
#define	DEFAULT_TEAM_HEAD		"sarge"
#define	DEFAULT_TEAM_MODEL_FEMALE	"crash"
#endif
#endif // TURTLEARENA

#ifdef TURTLEARENA // DEFAULT_TEAMS
#define DEFAULT_REDTEAM_NAME		"Clover"
#define DEFAULT_BLUETEAM_NAME		"Shell"
#else
#define DEFAULT_REDTEAM_NAME		"Stroggs"
#define DEFAULT_BLUETEAM_NAME		"Pagans"
#endif

// Moved footstep_t to bg_misc.h from cg_local.h
typedef enum {
	FOOTSTEP_NORMAL,
	FOOTSTEP_BOOT,
	FOOTSTEP_FLESH,
	FOOTSTEP_MECH,
	FOOTSTEP_ENERGY,
	FOOTSTEP_METAL,
	FOOTSTEP_SPLASH,

	FOOTSTEP_TOTAL
} footstep_t;

typedef enum {
	ABILITY_NONE,
	ABILITY_TECH,
	ABILITY_STRENGTH,
	ABILITY_MAX
} ablitiy_t;

typedef struct bg_playercfg_s
{
    char model[MAX_QPATH]; // "model[/skin]"  NOTE: In cgame/ui it is only model, in game has skin
    char headModel[MAX_QPATH]; // "headmodel[/skin]"  NOTE: In cgame/ui it is only model, in game has skin

	// Animation data
	animation_t		animations[MAX_TOTALANIMATIONS];

	qboolean		fixedlegs;		// true if legs yaw is always the same as torso yaw
	qboolean		fixedtorso;		// true if torso never changes yaw

	vec3_t			headOffset;		// move head in icon views
	footstep_t		footsteps;
	gender_t		gender;			// from model

	// Elite Force support
	char soundpath[MAX_QPATH];

	// Allow player models to have data that changes what happens in game.

#ifdef TA_WEAPSYS
    weapon_t default_weapon;
	int handSide[MAX_HANDS];
#endif

	// Player's boundingbox
	vec3_t bbmins;
	vec3_t bbmaxs;
	int deadmax; // Use deathmax instead of bbmax[2] when dead

	// Speed control, some characters are faster then others.
	int   max_speed;
	float accelerate_speed; // Replaces pm_accelerate; default 10.0f
	float accelstart;

	float jumpMult; // Jump velocity multiplier

	ablitiy_t ability;

	// Character's color preference
	unsigned int prefcolor1;
	unsigned int prefcolor2;

} bg_playercfg_t;

typedef struct
{
	int legSkip;
	int firstTorsoFrame;
} frameSkip_t;

int BG_AnimationTime(animation_t *anim);
int BG_LoadAnimation(char **text_p, int i, animation_t *animations, frameSkip_t *skip, int prefixType);
qboolean BG_LoadPlayerCFGFile(bg_playercfg_t *playercfg, const char *model, const char *headModel);

#ifdef TA_WEAPSYS
// For bg/game/cgame
animNumber_t BG_TorsoStandForPlayerState(playerState_t *ps, bg_playercfg_t *playercfg);
animNumber_t BG_TorsoAttackForPlayerState(playerState_t *ps);
animNumber_t BG_LegsStandForPlayerState(playerState_t *ps, bg_playercfg_t *playercfg);
animNumber_t BG_LegsAttackForPlayerState(playerState_t *ps, bg_playercfg_t *playercfg);
// For ui/q3_ui
animNumber_t BG_TorsoStandForWeapon(weapon_t weaponnum);
animNumber_t BG_TorsoAttackForWeapon(weapon_t weaponnum, unsigned int atkIndex);
animNumber_t BG_LegsStandForWeapon(bg_playercfg_t *playercfg, weapon_t weaponnum);
animNumber_t BG_LegsAttackForWeapon(bg_playercfg_t *playercfg, weapon_t weaponnum, unsigned int atkIndex);

qboolean BG_PlayerAttackAnim(animNumber_t aa);
qboolean BG_PlayerStandAnim(bg_playercfg_t *playercfg, int prefixBit, animNumber_t aa);
#endif

#ifdef TA_ENTSYS
typedef struct
{
	char filename[MAX_QPATH];

	// Object's boundingbox (some objects use func_voodoo instead)
	vec3_t bbmins;
	vec3_t bbmaxs;

	// Read from entity as well as config
	int health;
	int wait;
	float speed;
	qboolean knockback;
	qboolean pushable;
	qboolean heavy;

	// config only
	animation_t	animations[MAX_MISC_OBJECT_ANIMATIONS];
	qboolean	unsolidOnDeath;
	qboolean	invisibleUnsolidDeath;
	qboolean	lerpframes; // Use raw frames, don't interperate them.
	float		scale; // Uniform scale
	bg_sounds_t	sounds;
	int			explosionDamage;
	float		explosionRadius;
	int			deathDelay;

	char		skin[MAX_QPATH];

	// ZTM: TODO: For NPCs
	// Speed control, some characters are faster then others.
	//int   max_speed;
	//float accelerate_speed; // Replaces pm_accelerate; default 10.0f
	//float accelstart;

} bg_objectcfg_t;

void BG_InitObjectConfig(void);
bg_objectcfg_t *BG_DefaultObjectCFG(void);
bg_objectcfg_t *BG_ParseObjectCFGFile(const char *filename);
#endif
#endif

/*
===================================================================================

PMOVE MODULE

The pmove code takes a player_state_t and a usercmd_t and generates a new player_state_t
and some other output data.  Used for local prediction on the client game and true
movement on the server game.
===================================================================================
*/

typedef enum {
	PM_NORMAL,		// can accelerate and turn
	PM_NOCLIP,		// noclip movement
	PM_SPECTATOR,	// still run into walls
	PM_DEAD,		// no acceleration or turning, but free falling
	PM_FREEZE,		// stuck in place with no control
	/* bg_public.h
		ZTM: NOTE: Client (not cgame) expects PM_INTERMISSION as 5 and
			PM_SPINTERMISSION as 6.
	*/
	PM_INTERMISSION,	// no movement or status bar
	PM_SPINTERMISSION	// no movement or status bar
} pmtype_t;

typedef enum {
	WEAPON_READY,
	WEAPON_RAISING,
	WEAPON_DROPPING,
	WEAPON_FIRING
#ifdef TA_PLAYERSYS // WEAPONS
	,WEAPON_HAND_CHANGE
#endif
} weaponstate_t;

#ifdef TA_NPCSYS
// ZTM: Flags for general NPC effects.
typedef enum
{
	NPCF_NODROPWEAPON	= 1, // Don't drop weapon when killed.
	NPCF_ALLY			= 2, // Not a baddy, NPC on the same side don't attack each other.

#if 1 //
	NPCF_WALKANDFLY		= 4, // Can't run, but can fly
	NPCF_FLYONLY		= 8, // Can't walk or run, only fly -fast and slow
#endif

	NPCF_LAST // dummy flag

} npcflag_e;

// ZTM: General death types.
typedef enum
{
	NPCD_NONE, // Stays there doing nothing. Forever.
	NPCD_SINK, // Sink into the ground and removes the ent.
	NPCD_EXPLODE, // Starts explotion and removes the ent. For robots.
	NPCD_SMOKE, // Starts smoke and removes the ent.

	NPCD_MAX
} npcDeath_e;

#define MAX_NPCNAME 32
typedef struct
{
	char classname[MAX_NPCNAME];
	char model[MAX_QPATH];
	int weaponGroup; // ZTM: weapon group to hold/use
	int flags; // see npcflag_e
	int deathType; // see npcDeath_e
	int health;
	int viewheight;
	vec3_t mins, maxs;
	int deadmax; // Use deathmax instead of max[2] when dead
	animation_t animations[MAX_MISC_OBJECT_ANIMATIONS];
	int handSide[MAX_HANDS];

} bg_npcinfo_t;

#define MAX_NPCS 64 // max npc infos
extern bg_npcinfo_t bg_npcinfo[MAX_NPCS];
int BG_NPCIndexForName(const char *name);
int BG_NumNPCs(void);
void BG_InitNPCInfo(void);

typedef enum
{
	NACT_IDLE,
	NACT_FOLLOW, // Follow friend or enimy
	NACT_ATTACK,
	NACT_GO_TO,
	NACT_MAX

	//NACT_WAIT,
	//NACT_SEARCH,

} npcAction_e;

typedef struct
{
	bg_npcinfo_t *info;
	playerState_t npc_ps;

	npcAction_e action; // Current action
	int			actionTime; // Time action started.
	vec3_t		actionPos; // Position used by action

} bg_npc_t;
#endif

// pmove->pm_flags
#define	PMF_DUCKED			1
#define	PMF_JUMP_HELD		2
#ifdef IOQ3ZTM
#define PMF_FIRE_HELD		4		// set when attack has been started
#endif
#define	PMF_BACKWARDS_JUMP	8		// go into backwards land
#define	PMF_BACKWARDS_RUN	16		// coast down to backwards run
#define	PMF_TIME_LAND		32		// pm_time is time before rejump
#define	PMF_TIME_KNOCKBACK	64		// pm_time is an air-accelerate only time
#define PMF_LOCAL_HIDE		128
#define	PMF_TIME_WATERJUMP	256		// pm_time is waterjump
#define	PMF_RESPAWNED		512		// clear after attack and jump buttons come up
#define	PMF_USE_ITEM_HELD	1024
#define PMF_GRAPPLE_PULL	2048	// pull towards grapple location
#define PMF_FOLLOW			4096	// spectate following another player
#define PMF_SCOREBOARD		8192	// spectate as a scoreboard
#ifndef TURTLEARENA // POWERS
#define PMF_INVULEXPAND		16384	// invulnerability sphere set to full size
#endif

#define	PMF_ALL_TIMES	(PMF_TIME_WATERJUMP|PMF_TIME_LAND|PMF_TIME_KNOCKBACK)

#define	MAXTOUCH	32
typedef struct {
	// state (in / out)
	playerState_t	*ps;
#ifdef TA_PLAYERSYS
	bg_playercfg_t	*playercfg;
#endif
#ifdef TA_NPCSYS
	bg_npc_t		*npc;
#endif

	// command (in)
	usercmd_t	cmd;
	int			tracemask;			// collide against these types of surfaces
	int			debugLevel;			// if set, diagnostic output will be printed
	qboolean	noFootsteps;		// if the game is setup for no footsteps by the server
	qboolean	gauntletHit;		// true if a gauntlet attack would actually hit something

	int			framecount;

	// results (out)
	int			numtouch;
	int			touchents[MAXTOUCH];

	int			watertype;
	int			waterlevel;

	float		xyspeed;

	// for fixed msec Pmove
	int			pmove_fixed;
	int			pmove_msec;

	// callbacks to test the world
	// these will be different functions during game and cgame
	void		(*trace)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask );
	int			(*pointcontents)( const vec3_t point, int passEntityNum );
} pmove_t;

// if a full pmove isn't done on the client, you can just update the angles
void PM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd );
void Pmove (pmove_t *pmove);

//===================================================================================

typedef enum {
	TEAM_FREE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_SPECTATOR,

	TEAM_NUM_TEAMS
} team_t;

// Time between location updates
#define TEAM_LOCATION_UPDATE_TIME		1000

// How many players on the overlay
#define TEAM_MAXOVERLAY		32

//team task
typedef enum {
	TEAMTASK_NONE,
	TEAMTASK_OFFENSE,
	TEAMTASK_DEFENSE,
	TEAMTASK_PATROL,
	TEAMTASK_FOLLOW,
	TEAMTASK_RETRIEVE,
	TEAMTASK_ESCORT,
	TEAMTASK_CAMP
} teamtask_t;

// means of death
typedef enum {
	MOD_UNKNOWN,
#ifndef TURTLEARENA // MOD
	MOD_SHOTGUN,
	MOD_GAUNTLET,
	MOD_MACHINEGUN,
	MOD_GRENADE,
	MOD_GRENADE_SPLASH,
	MOD_ROCKET,
	MOD_ROCKET_SPLASH,
	MOD_PLASMA,
	MOD_PLASMA_SPLASH,
	MOD_RAILGUN,
	MOD_LIGHTNING,
	MOD_BFG,
	MOD_BFG_SPLASH,
#endif
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
#ifdef IOQ3ZTM // SWITCH_TEAM
	MOD_SUICIDE_TEAM_CHANGE,
#endif
	MOD_TARGET_LASER,
	MOD_TRIGGER_HURT,
#ifdef TA_ENTSYS
	MOD_EXPLOSION,
#endif
#ifdef MISSIONPACK
#ifndef TURTLEARENA // MOD NO_KAMIKAZE_ITEM POWERS
	MOD_NAIL,
	MOD_CHAINGUN,
	MOD_PROXIMITY_MINE,

	MOD_KAMIKAZE,
	MOD_JUICED,
#endif
#endif
	MOD_GRAPPLE,
#ifdef TA_WEAPSYS
	MOD_PROJECTILE,
	MOD_PROJECTILE_EXPLOSION,
	MOD_WEAPON_PRIMARY,
	MOD_WEAPON_SECONDARY,
#endif
	MOD_MAX
} meansOfDeath_t;

extern char	*modNames[MOD_MAX];
extern int modNamesSize;

//---------------------------------------------------------

#ifndef TA_ITEMSYS
// included in both the game dll and the client
extern	gitem_t	bg_itemlist[];
extern	int		bg_numItems;
#endif

gitem_t	*BG_FindItem( const char *pickupName );
gitem_t	*BG_FindItemForWeapon( weapon_t weapon );
gitem_t	*BG_FindItemForPowerup( powerup_t pw );
gitem_t	*BG_FindItemForHoldable( holdable_t pw );
#ifdef IOQ3ZTM
gitem_t	*BG_FindItemForClassname( const char *classname );
#endif
#ifdef TA_ITEMSYS
#define	ITEM_INDEX(x) (BG_ItemNumForItem(x))
#else
#define	ITEM_INDEX(x) ((x)-bg_itemlist)
#endif

qboolean	BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps );


// g_dmflags->integer flags
#define	DF_NO_FALLING			8
#define DF_FIXED_FOV			16
#define	DF_NO_FOOTSTEPS			32

// content masks
#define	MASK_ALL				(-1)
#define	MASK_SOLID				(CONTENTS_SOLID)
#ifdef TURTLEARENA // NO_BODY_TRACE
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#else
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#endif
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#ifdef TA_WEAPSYS // XREAL
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE|CONTENTS_SHOOTABLE)
#else
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)
#endif


//
// entityState_t->eType
//
typedef enum {
	/* bg_public.h
		ZTM: NOTE: Botlib expects these five in this order.
	*/
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,

	ET_BEAM,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_INVISIBLE,
	ET_GRAPPLE,				// grapple hooked on wall
	ET_TEAM,
#ifdef TA_ENTSYS // MISC_OBJECT
	ET_MISCOBJECT,
#endif
#ifdef TA_NPCSYS
	ET_NPC,
#endif

	ET_EVENTS				// any of the EV_* events can be added freestanding
							// by setting eType to ET_EVENTS + eventNum
							// this avoids having to set eFlags and eventNum
} entityType_t;



void	BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result );
void	BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result );

void	BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps );

void	BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad );

void	BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap );
void	BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap );

qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime );


#ifdef TA_SP
#define ARENAS_PER_TIER		2
#else
#define ARENAS_PER_TIER		4
#endif
#define MAX_ARENAS			1024
#define	MAX_ARENAS_TEXT		8192

#define MAX_BOTS			1024
#define MAX_BOTS_TEXT		8192


// Kamikaze

// 1st shockwave times
#define KAMI_SHOCKWAVE_STARTTIME		0
#define KAMI_SHOCKWAVEFADE_STARTTIME	1500
#define KAMI_SHOCKWAVE_ENDTIME			2000
// explosion/implosion times
#define KAMI_EXPLODE_STARTTIME			250
#define KAMI_IMPLODE_STARTTIME			2000
#define KAMI_IMPLODE_ENDTIME			2250
// 2nd shockwave times
#define KAMI_SHOCKWAVE2_STARTTIME		2000
#define KAMI_SHOCKWAVE2FADE_STARTTIME	2500
#define KAMI_SHOCKWAVE2_ENDTIME			3000
// radius of the models without scaling
#define KAMI_SHOCKWAVEMODEL_RADIUS		88
#define KAMI_BOOMSPHEREMODEL_RADIUS		72
// maximum radius of the models during the effect
#define KAMI_SHOCKWAVE_MAXRADIUS		1320
#define KAMI_BOOMSPHERE_MAXRADIUS		720
#define KAMI_SHOCKWAVE2_MAXRADIUS		704

#ifdef TA_SP
// Set as defaults for g_saveVersions so server can tell if can load savefile.
// They are setup in q3_ui/ui and game
#define BG_SAVE_VERSIONS "6" // Example: "0;1;2;3"
#endif
