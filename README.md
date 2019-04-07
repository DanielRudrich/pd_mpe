# pd_mpe
A little PureData external for MIDI Polyphonic Expression (MPE)

You can instantiate it simply with `[mpe]`, however you can also select a specific midi device e.g. device number 2 with `[mpe 2]`.

The object outputs a list with the following elements:

- device number: integer range from 1 to 16, reflecting the midi devices in the pd midi settings (will be omitted when specific device is selected)
- midi event symbol
- value1
- value2

The last two values depend on the midi event symbol:
- noteon
  - value1: midi pitch
  - value2: velocity
- noteoff
  - value1: midi pitch
  - value2: velocity
- press
  - value1: integer 0-127
  - value2: omitted
- slide
  - value1: integer 0-127
  - value2: omitted
- glide
  - value1: integer 0-16383
  - value2: omitted

