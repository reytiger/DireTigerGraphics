Names: Kevin Carbaugh, Allison Crouch, Ryan Porter

Assignment: Midterm - Guild Wars

Description: A culmination of all the previous openGL techniques we've learned
             A world with serveral vehicles, each moving a unique way

Usage: 

    Key               Effect

  Camera
  -------------------------------------
  | 1 |  Switch Camera to free mode   |
  |-----------------------------------|
  | 2 | Switch Camera to arcball mode |
  |-----------------------------------|
  | z |   Move free camera forward    |
  |-----------------------------------|
  | x |   Move free camera backward   |
  |-----------------------------------|

  Free hero Movement
  |---------------------|
  | w |   Move forward  |
  |---------------------|
  | s |   Move backward |
  |---------------------|
  | a |   Rotate left   |
  |---------------------|
  | d |   Rotate right  |
  |---------------------|

  Surface hero movement
  |------------------|
  | i |   increase V |
  |------------------|
  | k |   decrease V |
  |------------------|
  | h |   decrease U |
  |------------------|
  | l |   increase U |
  |------------------|
  | o |   next patch |
  |------------------|
  | u |   prev patch |
  |------------------|

Compilation: Run 'make' from the main directory to build

Details: Run with ./diretiger worldFile.dat

Time taken:
Kevin - 35 hours
  Bezier curves and surfaces, rocketship, materials and lighting
Allison - 
Ryan - 

Lab effectiveness on this assignment: 6/10

Fun factor: 5/10

Input File Format
-----------------

All whitespace is ignored, so it's for human readability:

Single empty lines denote end of sub-objects
Two spaces denote separations of objects
Three spaces separate between different sections


=======
<Number of Bezier surfaces>
<3 Empty lines>
<control point 1x>, <control point 1y>, <control point 1z>
...
<control point 16x>, <control point 16y>, <control point 16z>
<3 Empty lines>
<Number of control points for familiar's path>
<control point 1x>, <control point 1y>, <control point 1z>
...
<control point fx>, <control point fy>, <control point fz>
<3 Empty lines>
<Number of control points for Bezier track>
<control point 1x>, <control point 1y>, <control point 1z>
...
<control point jx>, <control point jy>, <control point jz>
<End>


Environment Data File Format:
<Number of buildings>
<location point 1x>, <location point 1z>
...
<location point mx>, <location point mz>
<Empty line>
<Number of trees>
<location point 1x>, <location point 1z>
...
<location point nx>, <location point nz>
<End>
