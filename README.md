# Poker
Command Line Poker Game
Overview

This is a simple Command Line Poker Game written in C++.
It features:

ASCII-style playing cards with suit symbols (♠ ♥ ♦ ♣)

Player starts with $500, CPU starts with $500

Custom betting: you can choose how much to bet each round

Shared pot system

AI decisions are visible, including bets, checks, and folds

Community cards (Flop, Turn, River)

Simple poker hand evaluation

How to Compile and Run
Requirements:

C++ compiler (like g++)

Steps:

Open terminal or command prompt

Compile the code:

g++ poker.cpp -o poker


Run the game:

./poker    # Linux / Mac
poker.exe  # Windows

Game Rules

You and the CPU start with $500 each.

Each round:

You are dealt 2 cards.

CPU is dealt 2 cards (hidden from you).

Community cards are revealed in stages: Flop (3), Turn (1), River (1).

Betting:

You can check, bet, or fold each round.

When betting, you choose how much to wager.

CPU will respond with a visible action (check, bet, or fold).

Showdown:

After all community cards are revealed, the hands are compared.

The winner gets the pot.

If tied, the pot is split.****
