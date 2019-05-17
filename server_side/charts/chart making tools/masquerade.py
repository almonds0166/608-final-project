# this chart is an attempt at translating Cytus's chart of Masquerade into  Beat Revolution

chart = [""]*50
beat_offsets = [0]*50

for i in range(50): 
    beat_offsets[i] = i*8

# intro part 1 skipped
# intro part 2
chart[0] = "r0u l0l l1l l2d r2r r3r r4u l4l l5l l6d r6r r7r"
chart[1] = "r0u l0l l1l l2d r2r r3r l4u r4u l5d r5d l6d r6.25d l6.5d r6.75d l7d"

# lead into chorus
chart[2] = "r0r l2l r4d l5d r6d"

# chorus
# mismatched motion
chart[3] = "r0d r1.5u l2l l2.5d r2.5d l3d r3d r3.5l l4d l5u r5.5u l6l l6.5d r6.5d l7d r7d r7.5l"
chart[4] = "l0u r1u l2l r3l r3.5r l5u r6u l7r l7.5l"
chart[5] = "l1u r1u l1.5l l3d r3d r3.5r l4d l5u r5u l5.5u r5.5u l6l r6r l7d r7d l7.5d r7.5d"
chart[6] = "l0l r2r r4d l5d r5d l6d"

# stanza 1
chart[7] = "l0u l1.5r r2d r3.5l l4u l5.5r r6d r7l"
chart[8] = "l0u l1.5r r2d r3r r4u r5.5l l6d l7r"
chart[9] = "r0d l2d l4u r6u"
chart[10] = "l0l r2r l4d r5d l6d r6.75d l7d"

# chorus
# matching motion 
chart[11] = "l0l r0l l0.5u r0.5u l1u r1u l1.5r r1.5r l2d r2d l2.5l r2.5l l3.5d r3.5d l4r r4r l4.5u r4.5u l5u r5u l5.5l r5.5l l6d r6d l6.5r r6.5r l7.5d r7.5d"
chart[12] = "l0u r0u l2d r2d l4u r4u l7d r7d l7.5d r7.5d"
chart[13] = "l0r r0r l0.5u r0.5u l1u r1u l1.5l r1.5l l2d r2d l2.5r r2.5r l3.5d r3.5d l4l r4l l4.5u r4.5u l5u r5u l5.5r r5.5r l6d r6d l6.5l r6.5l l7.5d r7.5d"
chart[14] = "l0u r0u l2d r2d l4u r4u l6d r7d"
# chose repeated, now mismatched motion -- not much from idle hand
chart[15] = "l0l r0u l0.5u l1u l1.5r l2d r2d l2.5l l3.5d l4u r4r r4.5u r5u r5.5l l6d r6d r6.5r r7.5d"
chart[16] = "l0u r0u l2d r2d l4u r4.5r r5r l7d r7.5d"
chart[17] = "l0u r0r r0.5u r1u r1.5l l2d r2d r2.5r r3.5d l4l r4u l4.5u l5u l5.5r l6d r6d l6.5l l7.5d"
chart[18] = "l0r l2l r3r r4d l4.5u r4.5u l5d r5d l5.5u l6d l7l r7r"

# intro again
chart[19] = "l0l r0u l2d r2r l4u r4r r6d l6l"
chart[20] = "l0l r0u l2d r2r l4u r4u l5u r5u l6d r6d"
chart[21] = "l0l r0u l2d r2r l4u r4u l4.75d r4.75d l5.5u r5.5u l6d r6.25d l6.5d r6.75d l7d"
chart[22] = "r0u l2d r4r l5l l6d r6d" 

# stanza 1
chart[23] = "l0u r0r l2l r2u l4l r4d l6d r6r"
chart[24] = "l0l r0r l2u r2d l4d r4u l6l r6r"
chart[25] = "r0d r0.5d l0.75d r1d r1.5d l2d l2.5d r2.75d l3d l3.5d r4l l5r r6r"
chart[26] = "l0d r0d l2u r2u l4d r4d l5d r5d l6d r6d"

# chorus
# matching motion 
chart[27] = "l0l r0l l0.5u r0.5u l1u r1u l1.5r r1.5r l2d r2d l2.5l r2.5l l3.5d r3.5d l4r r4r l4.5u r4.5u l5u r5u l5.5l r5.5l l6d r6d l6.5r r6.5r l7.5d r7.5d"
chart[28] = "l0u r0u l2d r2d l4u r4u l7d r7d l7.5d r7.5d"
chart[29] = "l0r r0r l0.5u r0.5u l1u r1u l1.5l r1.5l l2d r2d l2.5r r2.5r l3.5d r3.5d l4l r4l l4.5u r4.5u l5u r5u l5.5r r5.5r l6d r6d l6.5l r6.5l l7.5d r7.5d"
chart[30] = "l0u r0u l2d r2d l4u r4u l6d r7d"
# chose repeated, now mismatched motion -- not much from idle hand
chart[31] = "l0l r0u l0.5u l1u l1.5r l2d r2d l2.5l l3.5d l4u r4r r4.5u r5u r5.5l l6d r6d r6.5r r7.5d"
chart[32] = "l0u r0u l2d r2d l4u r4.5r r5r l7d r7.5d"
chart[33] = "l0u r0r r0.5u r1u r1.5l l2d r2d r2.5r r3.5d l4l r4u l4.5u l5u l5.5r l6d r6d l6.5l l7.5d"
chart[34] = "l0r l2l r3r r4d l4.5u r4.5u l5d r5d l5.5u l6d l7l r7r"

offset = 9470
bpm = 128
duration = 146000

print("offset " + str(offset))
print("bpm " + str(bpm))
print("duration " + str(duration))
print()

def pretty_print(chart, offset_beat):
    split_char = chart.split()
    for note in split_char:
        hand = note[0]
        direc = note[-1]
        beat = note[1:-1]
        output_string = ""
        output_string += hand.upper()
        output_string += ' '
        output_string += str(int(float(float(beat)+offset_beat)*60000/bpm))
        output_string += ' '
        output_string += direc
        print (output_string)

for i in range(50):
    pretty_print(chart[i], beat_offsets[i])
