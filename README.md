# SpeedTyper
### A Speed-typing application to improve keyboard skills! The main purpose of developing this software is for me learn the Raylib library. The goal of this software is to make a functional speed-typing software which tracks typing-speed, "accuracy" as defined by me, and overall completion time. I aim to pull samples of text from a database of some sort, either by an online-API or a written file.
## Update Log
### Initial state:
#### Initial development. Opens a window with Raylib and displays user-typed text on-screen.
### Update 1:
#### Added functionality to identify which user-typed text matches the sample-prompt or not. I resorted to using a few separate char arrays which correspond to in/correctly typed text and the sample-prompt text. I'm considering figuring out a more efficient way to store the user-text, one that uses less char arrays, but that is currently of little concern, as the text to be contained is fairly short. I intend the SpeedTyper to utilize fairly short prompts, which could be completed in ~10 seconds. I hope to work on the timing functionality. I might use std::chrono 🤷‍♂️.
