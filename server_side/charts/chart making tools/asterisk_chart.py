# This is what I used to make the chart for Bad Apple
# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects
# https://www.youtube.com/watch?v=uqolVXp5XLA 

chart = [""]*20
beat_offsets = [0]*20

# intro
chart[0] = "r0d l0d r2u l3u r4r r5l l8d l9d l13u r15r r16.5l r17u l18d r18d"
beat_offsets[0] = 4

#       i'm waiting for the rain
chart[1] = "r0l     r1r     r2d l4d l5d l6d l7d"
beat_offsets[1] = beat_offsets[0] + 20

#       i'm bracing for the thun                 der
chart[2] = "r0l     r1r     r2d l2.5d l2.75u l3d r3d l3.25u l3.5d r4d l4l l5d l6d l7d"
beat_offsets[2] = beat_offsets[1] + 8

#        a twig that wouldn't sway                                        in the wind
chart[3] = "r0l      r1r      r2d l2l l2.5r l3l l3.5r l4l l4.5r l5l l5.5r r5.5u  r6d"
beat_offsets[3] = beat_offsets[2] + 8

# instrumental
chart[4] = "r0r l0l r1d l1d"
beat_offsets[4] = beat_offsets[3] + 7 + 4

# hear my yearning
chart[5] = "r0r r1l l2r l3l r4d l4d r6d l6d"
beat_offsets[5] = beat_offsets[4] + 2 + 3

# see the crimson flame
chart[6] = "l0l r1l l2r r3r r4u l4u"
beat_offsets[6] = beat_offsets[5] + 7

# like a ruby
chart[7] = "l0d r0.5d l1l r1r l2r r2l"
beat_offsets[7] = beat_offsets[6] + 5 + 2

#            its the hope in my     eye ( d r u m s )
chart[8] = "l0l r0.5l r1r r4r l4.5r l5l l7d r7.75d l8d l8.5d r9d l9.25d r9.5d l9.75d"
beat_offsets[8] = beat_offsets[7] + 4

# if you go away
chart[9] = "l0u r1l r1.5r"
beat_offsets[9] = beat_offsets[8] + 10

# and i don't see you anymore
chart[10] = "l0.5l r1u l1.5r r2d l2.5l r3r r3.25l r3.5d l3.5d"
beat_offsets[10] = beat_offsets[9] + 3

# i still | wouldn't sway | but | i'd | be | missing | you
chart[11] = "l0l r0.5l l1r r1r r2u l3u r4d l5l l5.5l l6r"
beat_offsets[11] = beat_offsets[10] + 4

# now don't be afraid
chart[12] = "r0d l0.5d l0.75d r1d l1d r2.5u l2.5u"
beat_offsets[12] = beat_offsets[11] + 8

# you the flower couldn't sting
chart[13] = "l0r r0.5r l1u r1u l2.5d r2.5d"
beat_offsets[13] = beat_offsets[12] + 3

# you can now be my shade 
chart[14] = "l0.5u l0.75u l1u l2l r2r"
beat_offsets[14] = beat_offsets[13] + 3

# or maybe I x3
chart[15] = "r0r r1u r2d r3d l3d r4d l4d r5d l5d"
beat_offsets[15] = beat_offsets[14] + 3

# could I be safe x3
chart[16] = "l0l r0.5r l1d r1d l2d r2d l3d r3d"
beat_offsets[16] = beat_offsets[15] + 6

# in yours? x10
chart[17] = "l0d l0.5u r0.5u"
beat_offsets[17] = beat_offsets[16] + 4

# i'm waiting for the rain
chart[18] = "r0l     r1r     r2d l2d"
beat_offsets[18] = beat_offsets[17] + 10


offset = 0 # 4 beats? accounted for at intro offset
bpm = 85

print("offset " + str(offset))
print("bpm " + str(bpm))
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

for i in range(18):
    pretty_print(chart[i], beat_offsets[i])
"""
pretty_print(chart1, 0)
pretty_print(chart2, 48)
pretty_print(chart3, 80)
pretty_print(chart4, 112)
pretty_print(chart5, 120)
pretty_print(chart6, 128)
pretty_print(chart7, 136)
pretty_print(chart8, 144)
pretty_print(chart9, 152)
"""
