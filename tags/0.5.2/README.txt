Turtle Arena source directories setup

base (or base/assets*.pk3) - Game data used to run the engine
base-src - Source files for the game data that are not used by the engine
engine - Modified version of ioquake3
extras/bspc - Pre-build BSPC for creating bot .aas files for maps
extras/radiant-config - GtkRadiant 1.5.0 / NetRadiant config
extras/wminput - Config for using wiimote with Turtle Arena (Linux only)

Build engine/game-logic by running 'make' in engine directory.
Build packages by running 'make' in this directory.
