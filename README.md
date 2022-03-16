# blinken
Basic driver code for pimoroni scroll hat (as exercise) - i2c sans driver, based on IS31FL3731 IC docs.

# Basic design
Create a buffer of WIDTH * HEIGHT pixels assming X-window type coordinate system (i.e. origin in top left, fast heading right, slow downwards.) Here assuming that the USB connection is _to the right_.

Image is mapped to the correct electronic coordinates in rendering loop.
