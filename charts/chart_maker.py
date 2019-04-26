# This is what I used to make the chart for Bad Apple
# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects


chart = [""]*20
beat_offsets = [0]*20


# intro
chart[0] = "l0d l2d r3.5d l4d l6d r7d r7.5d l8d l10d r11.5d l12d l14d l15l r15r     l16d r17d l18l r19r l20u r21u l22r r22.5l l23l r23.5r     l24d r25d l26r r27l l28u r29u l30l r30.5r l31r r31.5l    l32u r33u l34l r35r l36d r37d l38l r38.5r l39r r39.5l    l40u r41u l42r r43l l44d r45d l46d r46d l47d r47d"
beat_offsets[0]=0

# singing 1, offset 48
chart[1] = "r0d r2r r4d r6l r8d r10l r12d r14r    l16d l18l l20d l22r l24d l26r l28d l30l"
beat_offsets[1]=48

# singing 2, offset 80
chart[2] = "r0d l0d r2r r4d l4u r6l r8d l8d r10l r12d l12u r14r    l16d r16d l18l l20d r20u l22r l24d r24d l26r l28d r28u l30l r30r"
beat_offsets[2]=80

# singing 3, offset 112
chart[3] = "l0d r0.5d l1d r1.5d l2d l3l r4d l4.5d r5d l5.5d r6d r7r"
beat_offsets[3]=112

# singing 4, offset 120
chart[4] = "l0d r0.5d l1d r1.5d l2d l3r r4d l4.5d r5d l5.5d r6d r7l"
beat_offsets[4]=120

# singing 5, offset 128
chart[5] = "l0d r0.5d l1d r1.5d l2d l3l r3l r4d l4.5d r5d l5.5d r6d r7r l7r"
beat_offsets[5]=128

# singing 6, offset 136
chart[6] = "l0d r0.5d l1d r1.5d l2d l3r r3r r4d l4.5d r5d l5.5d r6d r7l l7l"
beat_offsets[6]=136

# singing 7, offset 144
chart[7] = "l0d r0.5d l1d r1.5d l2d l3l r4d l4.5d r5d l5.5d r6d r7r"
beat_offsets[7]=144

# singing 8, offset 152
chart[8] = "l0d r0.5d l1d r1.5d l2d l3r r4d l4.5d r5d l5.5d r6d r7l"
beat_offsets[8]=152

# offset 160
chart[9] = "l0d r0d l0.5u r0.5u l1d r1d l1.5u r1.5u l2d r2d l3r r3l"
beat_offsets[9]=160

# offset 164
chart[10] = "l0d r0d l0.5u r0.5u l1d r1d l1.5u r1.5u l2d r2d l3l r3r"
beat_offsets[10]=164

# offset 168
chart[11] = "l0d r0d l0.5u r0.5u l1d r1d l1.5u r1.5u"
beat_offsets[11]=168

# offset 170
chart[12] = "l0d r0d l0.5u r0.5u l1d r1d l1.5u r1.5u"
beat_offsets[12]=170

# offset 172
chart[13] = "l0d r0d l1d r1d l2d r2d"
beat_offsets[13]=172

# offset 176, key shift, chill phase
chart[14] = "l0l l4r l8l l12r l16u l20d l24r l28d"
beat_offsets[14]=176

# offset 208, circles each hand
chart[15] = "l0l l2u l4r l6d r8l r10u r12r r14d"
beat_offsets[15]=208

# offset 224, circles both hands
chart[16] = "l0l r0l l2u r2u l4r r4r l6d r6d l8l r8l l10u r10u l12r r12r l14d r14d l16l r16r"
beat_offsets[16]=224

offset = 8442
bpm = 138

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

for i in range(20):
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
