# Character dmg simulation: Lyney


A little program to simulate rotations in order to calculate average dmg outputs from the Genshin Impact character's Lyney, under certain assumed circumstances and rotations.

The program by default simulates 100k rotations for each of the main builds, which differ mainly in the artifact set chosen for the character and *the weapon they wield* (*).

<h2>Use (command line)</h2>

  Compile:
  ```bash
  make
  ```
  Execute:
  ```bash
  ./lyney [-cr <crit_rate>]
  ```
  where the words between brackets are optional arguments:

   + `-cr` is the flag to set an arbitrary value of crit rate for the character
   + `<crit_rate>` is a numerical value representing the arbitrary crit rate of the character chosen by the player.\
     It may range `from 0 to 1`  or `from 1 to 100`; both ways work the same.


(*) This is currently marked as WIP. 

<h2>Output and analysis</h2>

As the result, the program will overwrite a csv file tied to each one of the builds run in the simulations, creating them if they not exist previously. 

In order to see some comparative measurements (ie average dmg and standard deviation) among each build's outputs, run the following command (must have *Python3*, *Pandas* and *Numpy* installed in your device):

```bash
python3 avg
```

