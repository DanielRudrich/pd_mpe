# pd_mpe
## Description
A little PureData external for MIDI Polyphonic Expression (MPE)

## Usage
You can instantiate it simply with `[mpe]`, however you can also select a specific midi device e.g. device number 2 with `[mpe 2]`.

The object outputs a list with the following elements:

- device number: integer in range [1, 16], reflecting the midi devices in the pd midi settings (will be omitted when specific device is selected)
- voice number: integer in range [1, 15], represents midi channels 2-16 (MPE)
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
  
## Compilation
You can find the binary for macOS in the `bin` directory.
In case you want to compile it yourself, you can use the `CMakeList.txt` to set your paths, and create your project with the terminal similar to this:

```
mkdir build
cd build
cmake .. -G XCode
```

(see `createXcodeProject.sh`)

It's very similar for windows to create Microsoft Visual Studio projects. You should also be able to build it directly with CMAKE.
