Prototype/Development Repo for the FCW RPG game that we are attempting to make

Actually Builds in Linux *AND* Windows Now!

I don't have a Mac to test it on.

Requirements (Windows):
=====
  * MinGW 7.2.0
  * CMake
  * Patience

Requirements (Linux):
=====
  * CMake
  * Working c++ compiler
  * Allegro 5 (`liballegro5-dev` on Debian/Ubuntu)

How to Build:
=====
  * Clone repository
  * Make new folder `build` 
  * Open terminal in the build folder
  * Run `cmake ..`
  * Run `make`
  * (Optional) Run `make zip_package`