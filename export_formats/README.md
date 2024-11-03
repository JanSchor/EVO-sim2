# Export formats
The `export_formats` folder contains formats for data transfer between separate programs.
This README will explain how does these formats work and how to write own scenario.

## Example scenario
Example scenario file shows how should the scenario look like.
The file in this directory is also valid scenario so it is possible to run it inside program.
Scenario file is used for setting conditions for the simulation.
This includes basic values like number of creatures in generation or values for when and where to build a wall.

Each scenario file starts with `head{}`. Inside head there are informations for simulation that do not change during the entire run of program.

After head there are separate informations for each generation, where something is supposed to change.
The lines look like this: `NUMBER{}`, where `NUMBER` represents number of generation.
Important generations are the first one and the last one, because these define the boundaries of simulation.
This means the simulation can start on generation `10000` and end on generation `20000`.
This is not usefull yet, but it is ready for future implementation of importing brains.

> [!CAUTION]
> If some parameters are not set, program will replace them with default values, this can create unwanted scenario.

### Commands
All the commands have the same format `key:value;`. This format can be seen inside both head and generation lines. All the commands are separated using `;`.

Starting here with header commands. These can be put only inside `head`.
It does not matter in which order they are put in. Only exception is *version*, which should be always first.
 * **Version** `ver:value;` - This tracks the version of scenario file.
    * `value` represents version.
    * Example: `ver:1.0;`
* **Grid** `grid:a1,a2;` - This tells the simulation, how big should the grid be.
    * `a1` represents x-axis, `a2` represents y-axis
    * Example: `grid:100,100;`
* **Brain size** `bs:n;` - This sets brain size for every creature (number of genomes)
    * `n` represents number of genomes in brain
    * Example: `bs:6;`
* **Inner neuron count** `inn:n;` - This sets number of inner neurons for all the creatures.
    * `n` represents number of inner neurons
    * Example: `inn:3;`
* **Status** `status:n;` - This sets how often should the status be printed to console (generations).
    * `n` represents number of generation until status
    * Example: `status:50;`

After `head` there should be commands for each wanted generation.
They are executed in order so it matter if some are put in front of another ones.
This is important for `clear`, because it clears all previous areas like `alive` area or `wall`.
All the values set are applied for all the generations after, until not defined differently.
This does not apply for `export`, which is done only at the mentioned generation.

First line of generation info sets starting parameters for simulation. If some are not set, program will replace it with default values.
 * **Steps** `steps:n;` - This sets number of steps for each generation.
    * `n` represents number of steps
    * Example: `steps:300;`
 * **Mutation rate** `mr:n;` - This sets probability for mutation to happen (1/n)
    * `n` represents the change (1/n)
    * Example: `mr:40;`
 * **Number of creatures** `cre:n;` - This sets number of creatures in each generation
    * `n` represents the number of creatures
    * Example: `cre:100;`
 * **Alive area** `alive:x1,y1,x2,y2,param;` - This sets coordinates and specification of criteria for reproduction.
    * `x1` represents the starting coordinates on x-axis
    * `y1` represents the starting coordinates on y-axis
    * `x2` represents the ending coordinates on x-axis
    * `y2` represents the ending coordinates on y-axis
    * `param` represents specification of the area, `0` is no specification.
    * Example: `alive:0,0,5,99,0;`
 * **Wall** `wall:x1,y1,x2,y2;` - This sets coordinates of wall.
    * `x1` represents the starting coordinates on x-axis
    * `y1` represents the starting coordinates on y-axis
    * `x2` represents the ending coordinates on x-axis
    * `y2` represents the ending coordinates on y-axis
    * Example: `wall:40,20,40,80;`
 * **Clear** `clear:area;` - This clears all specified areas
    * `area` represents the key of area
        - `clear:alive;` clears all alive areas
        - `clear:wall;` clears all walls
    * Note: *After clearing, all the areas need to be set again*
    * Example: `clear:wall;`
 * **Export** `export:ex;` - This exports specific information of generation
    * `ex` represents the key of export information
        - `export:s;` exports steps of entire generation
        - `export:b;` exports brains of all the creatures
    * Example: `export:s;`

**Example file**:
```bash
head{ver:1.0;grid:100,100;bs:6;inn:3;status:50;}
0{steps:300;cre:300;mr:40;alive:0,0,5,99,0;alive:95,0,99,99,0;export:s;}
199{export:s;}
200{wall:20,20,20,80;export:s;}
300{clear:wall;wall:40,20,40,80;export:s;}
400{export:s;export:b;}
```
This simulation will run from generation 0 to 400.
It will have 100x100 grid.
Creatures will have 6 genomes and 3 inner neurons.
Status will be printed every 50 generations.

On generation 0, there will be some values set. Simulation will have 300 steps and 300 creatures. Mutation rate will be 1/40. There will be 2 alive zones on specific coordinations and all the steps of the creatures will be exported.

On generation 199 will be another export of all the steps.

On generation 200 there will be built wall on specific coordinations, steps of this generation will be also exported.

On generation 300 all the walls will be deleted and there will be built new one. Another export of steps will happen.

On generation 400 there will be two exports. One for steps and another for brains. This is the last tracked generation so the simulation will end here.

> [!NOTE]
> In future I plan to build interactive program for creating scenarios.

When building your own scenario, it is not recommended to export lot of generations.
When building more complex simulation with many steps and creatures, the exported files can take a lot of space in memory.

## Example generation brains
This file is used for storing brains of all the creatures.
It is currently not used, but it will have mainly two purposis.
Firstly there will be possibility to display the brain with graphic.
This can serve as easy look on how does the creature work and why is it doing what is it doing.
Secondly there will be opportunity to start simulation with specific set of brains.
This will allow user to continue with already trained creatures and not start from 0.

The file format is very simple.
The file starts with `head{}`, that gives some basic informations about the generation.
Currently there are nor many parameters, that would be tracked.

Here are all the parameters, that are set in head:
 * **Version** `ver:value;` - This tracks the version of brain file.
    * `value` represents version.
    * Example: `ver:1.0;`
 * **Generation number** `gen:n;` - This shows what is the number of exported generation.
    * Example: `gen:400;`

Next lines of the file track individual creatures brains.
It is based on the brain size of creatures. Brain looks like array of genomes, these genomes are represented by 8-digit hex number.
Here is example of three lines inside this file:
```
cre0{29465930,6e207f7d,e197d68c,96ca3b50,4f575800,d1fde43d;}
cre1{6af8c33d,33a0c3d4,c960a547,7bf8d44e,44b1d4e5,d071b658;}
cre2{7b9d8e4f,4a1b2c3d,4e5f6a7b,8c9d0e1f,2a3b4c5d,6e7f8a9b;}
```
The beginning of the line shows the id of tracked creature. Each genome hex is separated by `,` and the line ends with `;`.

## Example steps log
File information is not here yet, will be in future.