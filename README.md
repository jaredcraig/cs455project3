CS 455 - Fall 2015
Program #3

 Object and Viewing Transformations, Input

Dr. Egbert
Due Tuesday October 6
(200 Points)

The purpose of this project is to teach you the basics of object transformations, viewing transformations, and input, and to give you experience implementing them inside of OpenGL.

For this project you will write the code that will allow you to perform object transformations and viewing transformations.  The first part of the assignment will involve performing object transforms to put objects in their correct location and orientation relative to one another.  We will give you three .obj files.  The first one will contain an environment.  The second will contain the model of a car.  The third will contain the model of a wheel.  You will need to replicate the wheel 4 times then perform the translations, rotations, and scales necessary to put the wheels at the proper locations in the car, with the proper orientations.  Once you have done that, you will need to perform the appropriate transformations to put the car into its parking spot in the environment model.

The last model you need to deal with is your humanoid from program 1.  You will need to place that humanoid either on top of, or inside the car.

Once the objects are situated correctly, you will need to associate the directional pad on the game controller with the front tires.  When the pad is pressed to the left, the tires should turn to the left.  When the pad is pressed to the right, the tires should turn to the right.

You will also need to allow for changes in the view of the scene.  Thus, you will need to implement the matrix transforms that will allow viewing changes to happen.  The left thumbstick should be associated with translation of the camera.  The right thumbstick should be associated with rotation of the camera.

Camera translations should occur in the XZ plane.  Moving the left thumbstick ahead should move the camera position ahead in the Z direction.  Moving the left thumbstick back should move the camera back in the Z direction.  Moving the left thumbstick left should move the camera left in the X direction, and moving it right should move the camera right in the X direction.

Camera rotations will occur about the Y and X axes.  Moving the right thumbstick ahead should rotate the camera down, about the X axis.  Moving the right thumbstick back should rotate the camera up about the X axis (or, if you like, you can swap these two).  Moving the right thumbstick to the left should rotate the camera to the left, about the Y axis (as if you were turning your head to the left), and moving it to the right should rotate the camera to the right about the Y axis.

Note: you are not allowed to use the OpenGL transform functions to accomplish these tasks.  You need to write the code yourself.

To pass-off this assignment, you will need to email your source code to myself and the TA, and you will need to meet with the TA to show your program in action.
