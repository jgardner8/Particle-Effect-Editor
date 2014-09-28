# Particle Effect Editor
A 2d Particle Engine and Effect Editor written on top of SDL 2.0. 

## Setup Notes:
For Windows: Make sure you use the dll files in the lib directory, as SDL has been modified slightly to fix an additive blending on coloured backgrounds bug I had. They need to be copied to the bin directory manually after compilation.
For Linux/OSX: just use default SDL files, as the changes were only made to the Direct3D renderer anyway.

## TODO
### Fixes
- No way to set _texture.
- Switch between presets.
- Bitmap fonts for framerate (5ms/frame -> 25ms/frame when enabling text!).
- Reset button is entirely broken.
- You can select multiple sliders simultaneously.

### Features
- Save/load emitters to/from file
- Composite pattern
- Face direction of movement (sparks)
- Colour depends on life (rather than just alpha)
- Bloom
