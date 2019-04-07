/*
 ==============================================================================
 Copyright (c) 2019 - Daniel Rudrich
 Released under GPLv3, see LICENSE.txt or <https://www.gnu.org/licenses/>.
 ==============================================================================
 */

#pragma once
#include <vector>

class RawMidiToMPE
{
    static const int maxNumberOfPorts = 16;

    static t_symbol* noteOnSym;
    static t_symbol* noteOffSym;
    static t_symbol* slideSym;
    static t_symbol* glideSym;
    static t_symbol* pressSym;
public:
    enum MidiEvent
    {
        noEvent, // shouldn't be sent out
        noteOn,
        noteOff,
        press,
        slide,
        glide
    };

    RawMidiToMPE (t_outlet *outlet) : outletPtr (outlet), numPorts (maxNumberOfPorts), portFilter (-1) //
    {
        //post ("Created RawMidiToMPE instance listening to all devices.");
        init();
    };


    RawMidiToMPE (t_outlet *outlet, const int portIndex) : outletPtr (outlet), numPorts (1), portFilter (portIndex) //
    {
        //post ("Created RawMidiToMPE instance listening to only one device.");
        init();
    };

    ~RawMidiToMPE() {};

    void init()
    {
        channel.resize (numPorts);
        midiEvent.resize (numPorts);
        firstByte.resize (numPorts);

        for (auto &elem : channel)
            elem = -1;

        for (auto &elem : midiEvent)
            elem = noEvent;

        for (auto &elem : firstByte)
            elem = -1;

    }

    void processRawMidi (const int byte, const int portNumber)
    {
        const int index = portFilter != -1 ? 0 : portNumber;

        if (index < 0 || index > maxNumberOfPorts - 1 || (portFilter != -1 && portFilter != portNumber))
            return;

        auto& ch = channel[index];
        auto& event = midiEvent[index];
        auto& byteOne = firstByte[index];

        if (event == noEvent) // listen to midi events
        {
            if (((byte >> 4) & 0x0F) == 0x09) // in range [144, 160[ -> noteOn
            {
                ch = (byte & 0x0F) + 1;
                byteOne = -1; // reset
                event = noteOn;
            }
            else if (((byte >> 4) & 0x0F) == 0x08)
            {
                ch = (byte & 0x0F) + 1;
                byteOne = -1; // reset
                event = noteOff;
            }
            else if (((byte >> 4) & 0x0F) == 0x0B) // control change
            {
                ch = (byte & 0x0F) + 1;
                byteOne = -1; // reset
                event = slide;
            }
            else if (((byte >> 4) & 0x0F) == 0x0D)
            {
                ch = (byte & 0x0F) + 1;
                byteOne = -1; // reset
                event = press;
            }
            else if (((byte >> 4) & 0x0F) == 0x0E)
            {
                ch = (byte & 0x0F) + 1;
                byteOne = -1; // reset
                event = glide;
            }
        }
        else // already received midiEvent
        {
            const int offset = portFilter == -1 ? 0 : 1;

            if (byteOne == -1) // waiting for first byte
            {
                if (event == press) // only one byte needed
                {
                    //post ("Press! port: %d, channel: %d, value %d ", portNumber, ch, byte);
                    t_atom list[4];

                    SETFLOAT (list, portNumber + 1);
                    SETFLOAT (list + 1, ch);
                    SETSYMBOL (list + 2, pressSym);
                    SETFLOAT (list + 3, byte);

                    outlet_list (outletPtr, &s_list, 4 - offset, list + offset);

                    event = noEvent;
                    return;
                }
                else if (event == slide && byte != 74) // this byte has to be 74 in order to be a slide
                    return;

                byteOne = byte;
            }
            else // already received first byte
            {
                t_atom list[5];

                switch (event)
                {
                    case noteOn:
                        //post ("NoteOnReceived! port: %d, channel: %d, pitch: %d, velocity %d ", portNumber, ch, byteOne, byte);

                        SETFLOAT (list, portNumber + 1);
                        SETFLOAT (list + 1, ch);
                        SETSYMBOL (list + 2, noteOnSym);
                        SETFLOAT (list + 3, byteOne);
                        SETFLOAT (list + 4, byte);

                        outlet_list (outletPtr, &s_list, 5 - offset, list + offset);
                        break;

                    case noteOff:
                        //post ("NoteOffReceived! port: %d, channel: %d, pitch: %d, velocity %d ", portNumber, ch, byteOne, byte);

                        SETFLOAT (list, portNumber + 1);
                        SETFLOAT (list + 1, ch);
                        SETSYMBOL (list + 2, noteOffSym);
                        SETFLOAT (list + 3, byteOne);
                        SETFLOAT (list + 4, byte);

                        outlet_list (outletPtr, &s_list, 5 - offset, list + offset);
                        break;

                    case slide:
                        //post ("Silde! port: %d, channel: %d,value %d ", portNumber, ch, byte);

                        SETFLOAT (list, portNumber + 1);
                        SETFLOAT (list + 1, ch);
                        SETSYMBOL (list + 2, slideSym);
                        SETFLOAT (list + 3, byte);

                        outlet_list (outletPtr, &s_list, 4 - offset, list + offset);
                        break;

                    case glide:
                        //post ("Glide! port: %d, channel: %d,value %d ", portNumber, ch, byte * 127 + byteOne);

                        SETFLOAT (list, portNumber + 1);
                        SETFLOAT (list + 1, ch);
                        SETSYMBOL (list + 2, glideSym);
                        SETFLOAT (list + 3, byte * 127 + byteOne);

                        outlet_list (outletPtr, &s_list, 4 - offset, list + offset);
                        break;

                    default: ;
                        //post ("Something went wrong!"); // shouldn't be reached!
                }


                // reset
                byteOne = -1;
                event = noEvent;
            }
        }
    }


private:
    t_outlet *outletPtr = nullptr;

    const int numPorts;
    const int portFilter;

    std::vector<int> channel;
    std::vector<MidiEvent> midiEvent;
    std::vector<int> firstByte;

};

t_symbol* RawMidiToMPE::noteOnSym = gensym ("noteon");
t_symbol* RawMidiToMPE::noteOffSym = gensym ("noteoff");
t_symbol* RawMidiToMPE::slideSym = gensym ("slide");
t_symbol* RawMidiToMPE::glideSym = gensym ("glide");
t_symbol* RawMidiToMPE::pressSym = gensym ("press");
