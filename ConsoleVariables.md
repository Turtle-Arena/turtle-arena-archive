List of **Console variables** (also called Cvars) that have been added and removed from ioquake3 Team Arena. The list is incomplete and based on the latest changes to the SVN.

# Added #

  * scorelimit (Renamed from fraglimit)

  * ui\_ffa\_scorelimit (Renamed from ui\_ffa\_fraglimit)
  * ui\_tourney\_scorelimit (Renamed from ui\_tourney\_fraglimit)
  * ui\_team\_scorelimit (Renamed from ui\_team\_fraglimit)
  * ui\_1flag\_capturelimit
  * ui\_1flag\_timelimit
  * ui\_1flag\_friendly
  * ui\_obelisk\_capturelimit
  * ui\_obelisk\_timelimit
  * ui\_obelisk\_friendly
  * ui\_harvester\_capturelimit
  * ui\_harvester\_timelimit
  * ui\_harvester\_friendly
  * ui\_spStage
  * ui\_publicServer

  * g\_spSaveData
  * g\_saveVersions
  * g\_saveTypes
  * g\_teleportFluxTime - Time in seconds to be non-solid after teleporting

  * cg\_drawSpeed - Draw player's speed on the HUD
  * cg\_drawMeleeWeaponTrails - Draw (unfinished) melee weapon trails
  * cg\_impactDebris - Set to 0 to disable impact particles. Set to 0.5 to cut in number half. Set to 2 to double number.

  * fs\_pure - Read only cvar: fs\_pure prevents sv\_pure from being set to true when the default Pk3 files are missing or modified.

  * sv\_public
    * 1: Advertise server on Internet, server will appear in the in-game server list. Technical: Server sends heartbeats to master servers.
    * 0: Doesn't list on master server, but replies to direct status requests (like if server is added as a favorite). Technical: Server doesn't send heartbeats to master servers.
    * -1: Don't replay to status requests, server will not show in the in-game server list. Other programs such as XQF it will show the server, but think the server is down (as it doesn't reply to status requests).
    * -2: Don't allow clients to join the game.
    * -3: (game/q3\_ui/ui) Offline multiplayer mode, uses q3\_ui/ui postgame.
  * Note: If the server had "sv\_public" set to 1 (public) and then is change to 0/-1/-2 (private) it will still be sent to clients by dpmaster master servers for up to 15 more minutes. Turtle Arena's MasterServer solves this issue, server will be sent out only 2 more seconds instead of upto 15 minutes.

  * cl\_thirdPersonAnalog - Move player relative to camera.

# Removed #

  * com\_blood

  * ui\_cdkeychecked

  * cg\_autoswitch
  * cg\_zoomfov
  * cg\_gibs
  * cg\_drawAmmoWarning
  * cg\_animSpeed
  * cg\_debuganim
  * cg\_forceModel


# Splitscreen Players #
Console variables and commands for splitscreen players begin "2", "3", or "4".

List for player 2;
  * 2dropflag
  * 2stats
  * 2setviewpos
  * 2teamvote
  * 2callteamvote
  * 2vote
  * 2callvote
  * 2team
  * 2followprev
  * 2follownext
  * 2follow
  * 2levelshot
  * 2teamtask
  * 2kill
  * 2noclip
  * 2notarget
  * 2god
  * 2give
  * 2vtaunt
  * 2votell
  * 2vosay\_team
  * 2vosay
  * 2vtell
  * 2vsay\_team
  * 2vsay
  * 2tell
  * 2say\_team
  * 2say
  * 2holdable
  * 2holdprev
  * 2holdnext
  * 2camreset
  * 2dropin
  * 2dropout
  * 2centerview
  * 2in\_joystickDebug = "0"
  * 2teamtask = "0"
  * 2spheadmodel = ""
  * 2spmodel = "mike"
  * 2cg\_thirdPersonRange = "120"
  * 2cg\_thirdPersonAngle = "0"
  * 2cg\_thirdPerson = "1"
  * 2cl\_anglespeedkey = "1.5"
  * 2in\_joystickUseAnalog = "0"
  * 2in\_joystickNo = "0"
  * 2cl\_yawspeed = "140"
  * 2cl\_pitchspeed = "140"
  * 2j\_pitch = "0.022"
  * 2j\_yaw = "-0.022"
  * 2j\_forward = "-0.25"
  * 2j\_side = "0.25"
  * 2j\_pitch\_axis = "3"
  * 2j\_yaw\_axis = "4"
  * 2j\_forward\_axis = "1"
  * 2j\_side\_axis = "0"
  * 2name = "UnnamedPlayer2"
  * 2model = "mike"
  * 2headmodel = ""
  * 2color1 = "5"
  * 2color2 = "4"
  * 2handicap = "100"
  * 2in\_joystick = "1"
  * 2in\_joystickThreshold = "0.150000"
  * 2cl\_thirdPersonAnalog = "1"