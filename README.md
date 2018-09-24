# ProjectPoker
An attempt at creating an online planning poker (agile) windows application using QT and C++.


This program is for effort estimation, adhering to the SCRUM agile methodology.


As is common in the various effort estimation tools, this software uses the modified fibonacci series as increments when voting.
 
 
 This is because of the inherent difficulty in estimating values when estimations get too large, and the recognizable change in magnitude   inherent to the modified series (allows voters to easily distinguish between different values and what they represent).

Functions only for Windows (for now).


Currently operational. Allows for:

-One room host per ip/port (this room host will act as the server)

-Passwords implemented, can be input or ignored when creating rooms

-Any number of room members (clients for that server) with usernames

-Messaging via TCP sockets

-Voting via TCP sockets (Graph display updates every time voting round has completed)

  This voting function is controlled by the room host who can enable and reset voting at will.
  All room members are able to see the graph of total votes once voting is complete but only the room host will see who voted for what. 

-Player shuffling by room host


To use this software over the internet, the source files must be downloaded and recompiled with the correct IP and PORT numbers in the file "info.cpp"

The current release of this project simply uses localhost for testing purposes.
