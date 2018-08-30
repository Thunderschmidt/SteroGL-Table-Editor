StereoGL - color table editor
----------------------------------

StGL_ct_edit.exe is part of the StereoGL engine by Martin Huedepohl.
It's written by Martin Huedepohl and Hannes Pertersen in 2005-2006.
This program creates StereoGL color tables (.ct files). Every monitor needs another color table for
perfect obliteration. A perfect obliteration means that your right eye doesn't see the picture for
the left eye and vice versa.

LEFT EYE / RIGHT EYE
Every .ct file contains two color tables, one for each eye. Choose here which of these two tables is currently edited.

RED / GREEN / BLUE
Defines which color component, red, green or blue, is currently edited by the 9 sliders.
When RIGHT EYE is selected, only the blue value can be edited.

THE 9 SLIDERS
Use the sliders for adjusting the two color tables.
1.When LEFT EYE is selected, watch the color bar above the sliders through the right glass of your red-green or red-blue
3D glasses. Move the sliders up or down, until the color bar is obliterated best possible. The 'red' values should rise
from 0 on the left end to 140-255 on the right end in a straight line. The 'green' and 'blue' values should be identical
to the corresponding background color values, but in many cases, you got to lower these values with growing 'red' values
due to perfect obliteration.
2.When RIGHT EYE is selected, watch the color bar through the left (red) glass of your 3D glasses. Modify the 'blue' values
until you don't see the color bar anymore. In this mode you're only allowed to adjust the 'blue' values. These values should
rise from 0 on the left end to 140-255 on the right end in a straight line. Sometimes the 'blue' values are too bright
and you need to lower them.

BACKGROUND COLOR
This RGB triplet is the background color StereoGL will use. Standard values are 40,40,40. Best obliteration is provided with
middle-grey values like 128, 128, 128. Remember to also modify the GREEN and BLUE values in LEFT EYE mode whenever changing
the background color.

FILE
Load and save your .ct files here. A file browser is not included. You can't save the file default.ct.

TEST IMAGE
Try out your color table here. Click REFRESH! first. Note that the more valuable tool for checking your is the
the color bar in the middle left.

