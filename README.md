# Homework 01 - Image Editing
### Due September 26 @ 11:59pm

## Overview

Your task in this assignment is to change the c program in [main.cpp](https://github.umn.edu/CSCI-3081-01-F25/public-hw01/blob/main/main.cpp) into a c++ program that uses classes.  You will read through the comments looking for opportunities to create objects.  Our image editor should have at least the following classes:

* Application
* Window
* Image
* Texture
* TexturedRectange
* ShaderProgram
* Button
* (any others that you would like to add)

Define your classes and copy code out of main.cpp into the classes that you write.  Be sure to change the [Makefile](https://github.umn.edu/CSCI-3081-01-F25/public-hw01/blob/main/Makefile) to build your all your code.  We may build or explore a few of these  classes during lecture or in lab.

**Single Responsibility Design Principle (High Cohesion)** - Be sure that every class does only one thing and does it well.  Be sure that every method does one thing and one thing well.

## Requirements

1. Create at least the classes specified above.  You may create more classes.
   * Modify the makefile appropriately to build your program.
   
2. Classes should have both a header file (.h) and an implementation file (.cpp).  Be sure to add header guards and follow class practices from class.
   * Create classes within the namespace csci3081
   * Use header guards.
   * Only use ```using namespace``` in the implementation, not the header.
   * Simple one line function implementations can be in header.
   * Only use structs for super simple objects.
   * Follow the **Single Responsibility Principle**.
     
3. The ```main.cpp``` should be simple.  For example
   ```c++
   #include "Applicaiton.h"

   int main() {
     using namespace csci3081;
   
     Application app;
     app.run();
   
     return 0;
   }
   ```
   
4. Follow good memory management practices:
   * Use ```new``` and ```delete``` in c++.
   * If any memory is allocated, it must be deleted.
   * If you have dynamic memory in a class, be sure to implement a copy constructor, the assignment operator, and a destructor.
     
5. Implement the Reset button.  When this is clicked, it should remove the drawing on top of the background image.  One way to do this is to reload the image.
   
6. Create another button with a new icon.
   
7. When the new button is clicked it must modify the background image in some way.  Here are some options:
   * Threshold the image by intensity.
   * Grayscale the image.
   * Change the color of the pen.
   * Change the shape or size of the pen.
   * Blur the image.
   * Sharpen the image.
   * Edge detect.
   * Make the colors or a color brighter.
   * Undo the last pen stroke.
   * Copy the image to the clipboard.
   * Rotate the image.
   * (Many more ideas to consider) -  Be creative!  Remember we will be hopefully releasing some of these to the world, so more diverse features may lead to a better final product.

## Submission

Please submit all your files (excpet the build directory) to the gradescope link provided by the due date.

## Getting Started

### On the CSE lab machines (TA Support)

Consider using Vole for remote development here:  [Vole - Virtual Online Linux Environment](https://cse.umn.edu/cseit/self-help-guides/virtual-online-linux-environment-vole)

* Clone the repository
  ```bash
  git clone https://github.umn.edu/CSCI-3081-01-F25/public-hw01.git hw01
  cd hw01
  ```
* Build the program
  ```bash
  make
  ```
* Run the program
  ```bash
  ./build/ImageEditor
  ```
