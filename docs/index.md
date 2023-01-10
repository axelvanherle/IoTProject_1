# Hi!

Welcome to the GitHub pages of this repo. 

This is my attempt at introducing the project through a blog. If you're looking for an explanation about the cogs in this machine, refer to the wiki. 

### The ultimate blog post.

The very first thing we did in relation to this project is simply setting up this repository. We could score some points doing this right, so i made sure that it was as clean as possible and ready to go.

The second thing we did was getting a docker container using GitHub actions that can run on the pynq z2 (we had to cross compile for this to work) This was a struggle, and we gave up for some time. However, this did not last long, because another course offered the opportunity to score some points if we could make it work. Wanting to kill two birds with one stone, we got to work. With our new-found reason to attempt this, we got it to work! 🥳 (It was a docker container running tic-tac-toe, it can be found [here](https://youtu.be/4sYgRcMa_cc) if you want to take a look)

Once we got cross compiling to work, a UDP client was made that can send random values to a UDP server. To achieve a mesh network, a UDP server was made that receives two values, and then sends the average of these two values to another UDP server. Both these programs can be found [here](https://github.com/axelvanherle/IoTProject_1/tree/main/src). Using our previously built GitHub action docker containers running both these programs were made. The intention was for these to run on the pynq z2.

Then the struggle began. We were first using an odroid m1, which came with its own Ubuntu server image. However, because of this image the docker container for open remote was unable to start up. This wasnt discovered until a few days of struggling. This was actually found out by our professor during a call, after switching to a fresh, clean Ubuntu server install there were minimal issues getting open remote up and running.

We were attempting to get openremote to receive the UDP packets we are sending but this failed.
So we decided to switch to MQTT packets, We found a way to connect it with open remote but we still can't send packets. We believe that the issue is with clientID's but we don't know how to fix it.

So currently we are still looking for a fix.

