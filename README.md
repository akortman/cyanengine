# cyanengine
cyanengine is (an unfinished attempt at) a 2D game engine.

This is the 3rd or 4th attempt at a generic game engine (i.e. not tied to a specific game implementation).
It's currently quite bare, but modules from my previous engine attempts are slowly being integrated in.
So, despite being apparently started in April 2021, this project has code roots that go back at least 2-3 years.

# vision: what ~~is~~ will cyanengine be?
cyanengine is a C++ library that consists of a modular game engine. The engine will primarily run a game specified
via the use of a number of written chaiscript scripts, but the library nature of the engine means integration with C++
code when necessary will be simple (aside from building it).
    - cyanengine will be 2D
    - cyanengine will allow games to be fully written in chai
    - cyanengine will allow games to be fully written in C++
    - cyanengine will be fast, dynamic, and able to handle a lot of co-existing game entities.
    - more to come.
    - despite the apparent certainty of the previous statements, cyanengine will probably never be finished (but I can
      hope).

This project is primarily one borne out of my own interest in working on projects that are (1) larger than I should be
working on alone, (2) require a lot of care and thought put into the architecture and the interface of the codebase, and
(3) a (I hope) platform for my own future games. I'd love for this project to be used by others, and it's designed with 
the hope of being easily usable by others, but I have no expectations of it being used in such a way.

# installation
Right now the engine is only an ECS system, scripting integration, and related tests, but if you'd like to build and
run cyanengine:
    
1. Check out this repo.
2. Check out the following dependency repos:
    - chaiscript: https://github.com/ChaiScript/ChaiScript
    - catch2: https://github.com/catchorg/Catch2
    - {fmt}: https://github.com/fmtlib/fmt
3. Build those projects using CMake into the ./library directory within cyanengine.
4. Run cyanengine's code generation with `./generate_code.cmd`, or if you're not on Windows, run the python script:
    - `python util/codegen/codegen.py -i cyan/codegen -o cyan/generated` (run in project root)
5. Build cyanengine with CMake.
    - As this project is (as of right now) solely worked on by myself on my (Windows 10) system, if the project isn't
      building after following steps, please let me know (april.kortman@gmail.com). When the project is more complete
      I'll be testing it on several different systems, as well as writing a script to automate the dependency
      installation steps, hopefully making it much easier to get started on. Also, there will be standalone applications
      to run the game via chaiscript without building it from scratch eventually.
      
  Note that all development has been done in CLion, and use of CLion to navigate and build the project is recommended.
