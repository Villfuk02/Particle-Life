# Particle Life

This is a simulation consisting of many particles existing in a 2D toroidal world. The particles interact with each other by exerting forces at a short range. Each particle species has simple rules, which dictate how much is it attracted to or repelled from particles of each other species. Even though the rules are simple, interesting self-organizing structures emerge.

[Video showcase](https://youtu.be/xeaFCix8YOg).

## Build

This project uses the SFML library for C++.

The Visual Studio project is configured for a debug build on a 64-bit Windows machine. Simply [download SFML 2.5.1 for 64bit Windows](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip), unzip it and put the folder into this project's folder. Then open the project in Visual Studio and start debugging. For release build, you'll need to reconfigere the project for release build according to [this guide](https://www.sfml-dev.org/tutorials/2.5/start-vc.php). On other operating systems, you'll need to download the corresponding SFML library from [here](https://www.sfml-dev.org/download.php).

The application also requires a true type font file called "font.ttf" in the same directory. This font is then used for drawing text in the graphical view. The font file included in this project is [Cascadia Mono](https://github.com/microsoft/cascadia-code).

## Usage

The application consists of two windows. One is terminal and the other one is graphical view. If either of them is closed, the application will terminate.

### Graphical view

This window shows the position of individual particles in real time. In the top left it also displays the ellapsed simulation time. In the top right it shows average ticks per second (tps) and frames per second (fps).

### Terminal

This window is for the user to configure the simulation. Simply type a command with corresponding arguments, confirm it with enter, and it will execute. If you use a command that requires arguments, but don't provide any, the corresponding current settings will be printed into the terminal. If a command doesn't succeed for any other reason, an error explaining the issue will be provided. If you end the input stream, the application will terminate.

#### List of commands

- **help**: Prints list of commands.
- **add**: Add a new particle species with given particle count.
- **cc**: The world will be split along each axis into a given amount of chunks. This setting won't affect the simulation, but will affect computation time.
- **dc**: Set the display color of a particle species.
- **dr**: Set radius of the particles as displayed to the screen. If set to 0, rendering will be much faster and particles will be rendered as 1px points.
- **f**: Set how fast particles lose their momentum.
- **fps**: Set target frames per second.
- **p**: Pause or unpause the simulation.
- **q**: Exit this application.
- **r**: Set peak repulsion strength.
- **s**: Run a single tick of simulation.
- **sa**: Set the peak strength of attraction of particles of given species to particles of the other species. Can be negative, then the particles are repelled instead.
- **sar**: Set the distance at which particles of given species start being attracted to particles of the other species. Must be more than the corresponding repulsion range and less than third of world size.
- **sc**: Change number of particles of a given species.
- **srr**: Set the distance at which particles of given species start being repelled from particles of the other species. Must be less than the corresponding attraction range.
- **ss**: Set simulation speed.
- **tps**: Change number of ticks per second of simulation. Too long time steps make simulation unstable. Inverse of "ts".
- **ts**: Change simulation time step. Too long time steps make simulation unstable. Inverse of "tps".
- **ws**: Set world size. It must be greater than three times the largest attraction range.

## Optimisations

Instead of calculating the effects of each particle on each particle (O(n^2) time), the world is split into chunks. Since each interaction has maximum range, there is no need to consider forces between particles which are to far apart. For each interaction type, for each chunk only the chunks close enougn to contain relevant particles are checked. This approach is more complicated and technically slower, (still worst case O(n^2) time), but for spread out particles, this approach is way faster. This is where the chunk count setting comes in. For each simulation setting, there exists optimal chunk count, at which the simulation will run the fastest.

Also, calculating the forces each particle is experiencing at any given time usually takes the longest. That is why it is split up into separate threads, one for each particle species.

## Attributions

This project was inspired by many other "Particle Life" simulations, namely [this video by CodeParade](https://youtu.be/Z_zmZ23grXE). Detecting that standard input isn't empty was taken from [this Stack Overflow answer by radj](https://stackoverflow.com/a/71992965).
