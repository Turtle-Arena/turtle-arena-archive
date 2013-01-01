Turtle Arena source directories setup

base (or base/assets*.pk3) - Game data used to run the engine
base-src - Source files for the game data
engine - Modified version of Spearmint (based on ioquake3)
extras/radiant-config - GtkRadiant 1.5.0 / NetRadiant config
extras/wminput - Config for using wiimote with Turtle Arena (Linux only)

Build engine and game-logic by running 'make' in engine directory.
Build BSPC by running 'make' in engine/code/bspc directory.
Build packages by running 'make' in this directory.
