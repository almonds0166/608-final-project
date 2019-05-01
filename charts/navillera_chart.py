# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects


chart = [""]*20
beat_offsets = [0]*20

# there's some measure before this but it doesn't quite feel like a full measure

# intro
chart[0] = "l0d r3u l4d r6r r7u l8d r10.5d l11d r12d l14l r15u r16r l17.5l r20d l21d r21.75d r23u l24l r26r l28d r28.75d l29.5d"
beat_offsets[0]=0

# singing part 1
chart[1] = "r0.5d l1d r1.5d l2d r3r r4r l4.75d r5.5l l7u r8d l10r r12d r13d r14d r15d l16d r18l l20r r22u r23d l24u r26r l28d l29d l30d l31d"
beat_offsets[1] = 32

# singing part 2
chart[2] = "l0.5u r1u l1.5d r2d r3d l4l r6r l8d r10u r11u l12d r12.75d l13.5d r16r l18l r20d r21d l22u r24d r26d l28l r28r l29l r29r l30l r30r"
beat_offsets[2] = 64

# singing part 3
chart[3] = "l0d r2d l4r r4l l5l r5r l6d r6d l8u r10u l12d r12u l13u r13d l14l r14r r16d l18r l20d r21r l22u r24.5d l25d r25.5d l26d r28r l28.75l r29.5d"
beat_offsets[3] = 96

# interlude
chart[4] = "l0d r2u l4u r6d l8l r10r l12r r12.75l l13.5d r13.5d"
beat_offsets[4] = 128

# singing
chart[5] = "l0.5d r1d l1.5d r2r l3l r4d l8u r10l l12d l16l l17r l18d r20d r22r l24l r24r l26l r26r l28d r28d"
beat_offsets[5] = 144

# singing
chart[6] = "r0.5u l1u r1.5d l2d r3d l4.5u r5u l5.5d r6d l7d r8r l10l l12d r12d l12.75l r12.75r l13.5d r13.5d l16l l17r l18d r20r r21l r22d l24d r25u r26u r27u l28d l29d l30d"
beat_offsets[6] = 176

# singing
chart[7] = "r0d l2d r4r r5l r6d l8u l9l l11r l12d r13d l13.5d r13.75d l16l r18r l20d r20d l21r r21l l22d r22d r24.5d l25d r25.5d l26d r27r l28d r28d l28.75r r28.75l l29.5d r29.5d"
beat_offsets[7] = 208

# coda
chart[8] = "l0d r2u l4u r6d l8l r10r l12d r12d l12.75d r12.75d l13.5d r13.5d"
beat_offsets[8] = 240

# I don't know what these are
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
