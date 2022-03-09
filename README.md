# LDR_Arduino_Code
Arduino code example for using LDR's on a model railroad layout

This approach uses (at least) 3 LDR's between the rails and all are used for detection purposes. The ambient light they see does not need to be constant but should affect them all more or less uniformly. I will give the code snippets here and if anyone cares to test I would be happy to assist and discuss it or demonstrate further. 

Notes: Out of the box LDR's will have piece to piece variations. 
This is exacerbated when they are installed since they will not be at precisely the same alignment and may have fingerprints or smudges on them etc.
In the sketch you will see offsets are defined per LDR. For your initial testing you should reset them to zero and uncomment the line #define calibrate.
By using the serial plotter in the Arduino IDE you will get a plot foreach LDR. You will want to determine the constants to make these lines overlap as close as possible.
Then make the offset this value and the LDR plots should align to within a few counts (you know you are close when 'noise' dominates the picture.
Adjust your code by making changes to the offset and replotting. 
With a little testing I got all three plots on top of each other (the plot fills the screen with 'noise' but the scale was only a few counts high.   

The LDR threshhold above is a constant and I started with a value of 40 (Units are just 'counts' after the analog input .. so a scale of 0-1024)
Later on I lowered it once I got the offsets sorted out, and it could likely go lower still.

To act on this information we just compare the analog input value to the upper limit (which is tracking the average). 
The neat part of this is that one value changing does not affect the average as much as it affects itself
and this deviation is easily detected.

I have included some plots that demonstrate the key aspects. (take note of the scale on the left)

Key Findings: 

Reliably detect shadow over the LDR in significantly different ambient light conditions
Rate of change of the light is not an issue (deviation from the average is what is being measured).
Starting with one LDR in shadow is detected every time - again because it differs from the average.

Anecdotal:
- changing contents on the monitor affect all three LDR's that are a mter away.
- a slight shadow is easily observable on the plot and could be detected in code. 
