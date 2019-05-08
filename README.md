# pd_mpe
## Description
A little PureData external for MIDI Polyphonic Expression (MPE)

## Usage
You can instantiate the mpe object simply with `[mpe]`, and it will listen to all MIDI devices enabled in the pd midi settings. Alternatively, you can select a specific midi device e.g. device number 2 with `[mpe 2]`.

The object has only one outlet which outputs a list with the following atoms:

- **device number** integer in range [1, 16], reflecting the midi devices in the pd midi settings (will be omitted when specific device is selected)
- **voice number** integer in range [1, 15], represents midi channels 2-16 (MPE)
- **midi event symbol** see below
- **value1**
- **value2**

The last two values depend on the **midi event symbol**. In case of *noteon* and *noteoff*, both values are used. The other three events only send one value:
- **noteon**
  - value1: midi pitch
  - value2: velocity
- **noteoff**
  - value1: midi pitch
  - value2: velocity
- **press**
  - value1: integer 0-127
- **slide**
  - value1: integer 0-127
- **glide**
  - value1: integer 0-16383
  
## Compilation
You can find the binary for macOS in the `bin` directory.
In case you want to compile it yourself, you can use `make` in terminal / command line.
