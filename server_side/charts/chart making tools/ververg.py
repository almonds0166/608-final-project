chart = [""]*40
beat_offset = [0]*40

for i in range(25):
    beat_offset[i] = i*12

# note: time signature is 3/4

# first four measures chorus
chart[0] = "l1d r1.5d l2d r3d r4r r5u r7d l7.5d r8d l9d l10l l11u"

# second four measures chorus
chart[1] = "r0r l1u r2.5d l3d r6u l7d r8d l9l r10r l11d"

# pause for four measures 
chart[2] = "r0d l6u"

# stanza 1
chart[3] = "r0r l2l r3u l5u r6d l8d r9r l10l r11l"
chart[4] = "l0u r2r l3d r4u l5l r6l l7r r8d l9u r10u l11d"
chart[5] = "r0d l2l r3u l5d r6r l8u r9d l10r r11l"
chart[6] = "l0l r1r l2u r3u l5d r6d l8u r9u"

# lead up to chorus
chart[7] = "l0.5d r1d l1.5d r2.5d l3.5d r4d l4.5d r5.5d l6.5d r7d l7.5d r8.5d l9d l10l l11u"
chart[8] = "r0.5d l1d r1.5d l2.5d r3.5d l4d r4.5d l5.5d r6.5d l7d r7.5d l8.5d r9d r10r r11u"
chart[9] = "l0.5d r1d l1.5d r2.5d l3.5d r4d l4.5d r5.5d l6.5d r7d l7.5d r8.5d l9d r10r l11u"
chart[10] = "r0.5d l1d r1.5d l2.5d r3.5d l4d r4.5d l5.5d r6d l7.5l r9r l10.5u"

# chorus
chart[11] = "l1d r1.5d l2d r3d r4r r5u r7d l7.5d r8d l9d l10l l11u"
chart[12] = "r0r l1u r2.5d l3d r6u l7d r8d l9l r10r l11d"
chart[13] = "l1d r1.5d l2d r3d r4r r5u r7d l7.5d r8d l9d l10l l11u"
chart[14] = "r0u l1l r3r l4u r5u l6l r6r l9d r9d"

# lead up to chorus
chart[15] = "l0.5d r1d l1.5d r2.5d r3.5d l4d r4.5d l5.5d l6.5d r7d l7.5d r8.5d l9d r10r l11u"
chart[16] = "r0.5d l1d r1.5d l2.5d l3.5d r4d l4.5d r5.5d r6.5d l7d r7.5d l8.5d r9d l10l r11u"
chart[17] = "l0.5d r1d l1.5d r2.5d r3.5d l4d r4.5d l5.5d l6.5d r7d l7.5d r8.5d l9d l10l r10r l11u l11u"
chart[18] = "r0.5d l1d r1.5d l2.5d r3.5d l4d r4.5d l5.5d r6d l7.5l r9r l10.5u"

# pause
chart[19] = ""

# chorus
chart[20] = "l1d r1.5d l2d r3d r4r r5u r7d l7.5d r8d l9d l10l l11u"
chart[21] = "r0r l1d r2.5d l3l r6u l7d r8d l9l r10r l11d"
chart[22] = "r1d l1.5d r2d l3d r4r l5u l7d r7.5d l8d r9d l10l r11u"
chart[23] = "l0d r1d l2.5u r3u l5d r6d l10u r10u l11u r11u"
chart[24] = "l0u r0u"

# super freaking weird pause, then chill
chart[25] = "r0d r3l r6r r9u l12d l15r l18l l21u"
beat_offset[25] = 24*12 + 11


offset = 8651
bpm = 190
duration = 117000

print("offset " + str(offset))
print("bpm " + str(bpm))
print("duration " + str(duration))
print()

total = 0

def pretty_print(chart, offset_beat):
    global total
    split_char = chart.split()
    for note in split_char:
        total+=1
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

for i in range(40):
    pretty_print(chart[i], beat_offset[i])

print(total)
