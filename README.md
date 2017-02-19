# Gumoku AI
### 5 in a row on a 15 by 15 board

### Overview

To checkout the game and play it. Please go onto:

http://54.164.38.11/ (Be ware that we were not able to use the full tree search because AWS instances aren't cheap. This is a simplified version)


### Algorithm

This AI uses a combination of Threat Search, Proof Numbers, and Minimax to evaluate and prune moves. The main idea comes from:
http://www.aaai.org/Papers/Symposia/Fall/1993/FS-93-02/FS93-02-001.pdf

### Results:
This AI beats all other online AIs that I could find, and on MiniClip.com's surprisingly competitve 5 in a row game, it had a win rate of 94%

### File Structure

app/__init__.py contains the majority of the web app
app/ai.c contains all the AI code. Be warned, not pretty!

Thank you!
