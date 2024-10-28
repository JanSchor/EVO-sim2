# Example formats
The `example_formats` folder contains formats for data transfer between separate programs.
This file will explain how does these formats work and how to write own scenario.

## Example scenario
Example scenario file shows how should the scenario look like.
The file in this directory is also valid scenario so it is possible to run it inside program.
Scenario file is used for setting conditions inside animations.
This includes basic values like number of creatures in generation or values like when and where to build a wall.

Each scenario file starts with `head{}`. Inside head there are inforamtions for simulation that do not change during the entire run of program.

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
It does not matter in which order they are put in. Only exception id *version*, which should be always first.
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
    * Example: `status:3;`

After `head` there should be commands for each generation.
They are executed in order so it matter if some are put in from of another ones.
This is important for `clear`, because it clears all previous areas like `alive` are or `wall`.
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
    * Note: *After clearing all the areas need to be set again*
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

## Example generation brains
File information is not here yet, will be in future.

## Example steps log
File information is not here yet, will be in future.