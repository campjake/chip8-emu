# Implementing the CHIP-8 Interpreter My Way
Check out the guide [here](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/).

CHIP-8 was originally created as a way to make small programs and games in the late 70's and early 80's using hexadecimal instructions that were a layer of abstraction above machine code. The interpreter would run your program on the fly.

## What's Different from the other implementations?
I'm attempting to implement my own version of CHIP-8 in C++ with smart pointers, and a [Pimpl](https://en.cppreference.com/w/cpp/language/pimpl). The overhead of using a `std::unique_ptr` is very low since we are not using any custom deleters. 

The Pimpl does have overhead on operations that are running very frequently. To be specific, using a Pimpl, we have a layer of indirection whenever we access a member variable to dereference the pointer. However, CHIP-8 is not very demanding, and we often have to manually add some buffering time in there to run games at the intended speed. As a result, I see these as excellent learning opportunities for Modern C++ concepts without paying any design breaking consequences (for now).

## Anything else coming?
Yep! I want to write a fully-fleshed out testing environment with Google Test, and build it with More Modern CMake techniques from CMake 3.14 and on. As of right now, CHIP-8 resources online offer testing scripts that run lightweight programs, like printing the IBM logo to the screen. I think a Test-Driven Development version of this project could benefit a lot of new programmers to tackle this project since they will have a step-by-step process to incrementally make progress through this challenge.

This README will also continue to get updated with lessons learned, key concepts I think you should know, and instructions on how to reproduce my program with CMake. Stay tuned!

## Motivations
For the last couple years, I've been wanting to learn how to build emulators. I think it's amazing that the Nintendo Switch Online service can provide users with the opportunity to play their old classic games right on their device. It's an awesome way to introduce the next generation of gamers to the flawed and relatively primitive gems that fill us with nostalgia. I'm hoping that I'll gain some skills from this product that I can roll into the next venture. Somewhere in the future, I want to create my own GameBoy emulator in C.
