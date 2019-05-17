# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects


chart = [""]*20
beat_offsets = [0]*20

# measures 5 through 12
chart[0] = "l0d r0d l1.5u r2.5u r3.5d l5d r6.5l l9d r10.5u l11u l11.5l l12d r12d l14u r14u l16d r16.5d l17d r17.5d l18d r18.5d l19d r19.5d l20l r20.5r l21l r21.5r l22l r22.5r l23l r23.5r l24d r24.5d l25d r25.5d l26d r26.5d l27d r27.5d l28r r28.5l l29r r29.5l l30r r30.5l l31r r31.5l"
beat_offsets[0] = 16

# second part of intro
chart[1] = "l0u r0u r1d l1.5d r3.5r r5l r5.5d l8d r8d l12l r12r l12.5l r12.5r l13.5l r13.5r l16l r16r"
beat_offsets[1] = 48

# melody 1
chart[2] = "l0d l2d r2.5u r3r r3.5u l4d r4d r4.5u l6d l8l l10l r10.5u r11l r11.5u l12l r12d r12.5u l14l r16d r18d l18.5u l19l l19.5u l20d r20d l20.5u r22d r24r r26r l26.5u l27r l27.5u l28d r28r r30r"
beat_offsets[2] = 68

# melody 2
chart[3] = "l0d r0d l4l r4r r5d l5.5d r6.5l r7.5r l8d r9l r9.5d l10.5u l11r l11.5l l16l r16r l20d r20d l21u r21u l21.5d r21.5d l23.5d r23.5r l25.5u r25.5u l27d r27.5d l32u r32u"
beat_offsets[3] = 100

# melody 3
chart[4] = "l0r r0r l4l r4l l8r r8r l10d r10.5d l11l r11.5r l12u r12u l16d r16.5r r17l r17.5u r20d l20.5r l21l l21.5u l24d r24d l28d r28d"
beat_offsets[4] = 136

# melody 4
chart[5] = "l0d r0d r2r l2.5l r3l l3.5r r4d l6d l8u r8u r10r l10.5l r11d l11.5d r12u l16l r16r l17.5r r17.5l l19.5d r19.5d l24d r24d l27.5u r27.5u l29.5d r29.5d"
beat_offsets[5] = 168

# syncopated
chart[6] = "l1d r1.5d l2.5l r3u l4u r4.5r l5.5d r6u l7l r7.5r l8.5u r9d l10d r10.5l l11.5r r12u l13u r13.5u l14.5d r15d"
chart[7] = "l1d r1d l1.5d r1.5d l2.5l r2.5u l3l r3u l4u r4r l4.5u r4.5r l5.5d r5.5u l6d r6u l7l r7r l7.5l r7.5r l8.5u r8.5d l9u r9d l10d r10l l10.5d r10.5l l11.5r r11.5u l12r r12u l13u r13u l13.5u r13.5u l14.5d r14.5d l15d r15d"
beat_offsets[6] = 200
beat_offsets[7] = 216

# sixteenths
chart[8] = "l0d r1d l2d r3d l4l r5r l6l r7r l8d r9d l10d r11d l12l r12r l16d r16d l18d r18d l20l r20r l22l r22r l24d r24u l26u r26d l28d r28d"
beat_offsets[8] = 232

# don't remember
chart[9] = "r0.5d r1u l1.5u r2d l4d r5r l5.5r r6l l6.5l r7d d7.5d l8d r8d l10l r10r l10.5u r10.5u l11d r11d l11.5u r11.5u l12r r12r l16.5d l17u r17.5u l18d r18d l19l r19r l20d l21d l22d l23d l24d r24d l25d r25d l26d r26d l27d r27d l28l r28r l28.666666l r28.666666r l29.333333l r29.333333r l30d r30d"
beat_offsets[9] = 268

# syncopated part 2
# not yet edited
chart[10] = "l0.5u l1d r2d r2.5u l3.5u l4l r5d l5.5d l8r r8l r10d r10.5u l11.5u l12d r12d l12.666666l r12.666666r l13.333333u r13.333333u l14d r14d l14.666666r r14.666666l l15.333333l r15.333333r l16.5d r16.5u l17u r17d l18r r18r l19.5u r19.5d l20d r20u l21l r21l l24d r24d l26u r26u l28d r28.5d l29d r29.5d l30d r30.5d l31d r31.5d"
beat_offsets[10] = 300

# fast ending
chart[11] = "l0l l1l l2l l3l l4d l5d l6d r6.5d l7d r7.5d r8r r9r r10r r11r r12l r13l r14d l14.5d r15d l15.5d l16l r16r l20d r20d l24u r24u l28d r28d l32u r32u l32.5d r32.5d"
beat_offsets[11] = 332

offset = 1503
bpm = 155
duration = 147000
num_notes = 0

print("offset " + str(offset))
print("bpm " + str(bpm))
print("duration " + str(duration))
print()

def pretty_print(chart, offset_beat):
    global num_notes
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
        num_notes += 1
        print (output_string)

for i in range(20):
    pretty_print(chart[i], beat_offsets[i])

print(num_notes)
