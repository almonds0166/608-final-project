# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects


chart = [""]*20
beat_offsets = [0]*20


# intro
chart[0] = "l0d l0.75u l1d l1.5r r2d r2.75u r3d r3.5l l4u r4d l5d r5u l6u r7d l8d l8.75u l9d l9.5l r10d r10.75u r11d r11.5u l12d r12d l13l r13r l14d r14d"
beat_offsets[0]=0

# intro part 2
chart[1] = "r0d r0.75u r1d r1.5r l2d l2.75u l3d l3.5l l4d r4u l5u r5d r6u l7d r8d r8.75u r9d r9.5l l10d l10.75u l11d l11.5u l12d r12d l13l r13r l14d r14d"
beat_offsets[1] = 16

# melody part 1
chart[2] = "r0r l2l l4l r4r l5d r5d l5.5u r5.5u l6l r6r l8d r10u l12l l13d l14d l15u"
beat_offsets[2] = 32

# melody part 2
chart[3] = "r0d l0.75d r1d r1.5r l2l l3u l4d r4.75d l5l l5.5r l6d r7u r8r l10r r10l l12d r12d l15u r15u"
beat_offsets[3] = 48

# bouncy part
chart[4] = "r0d l0.75u l1d l1.5u r2u l2.75d l3u l3.5d l4u r4d l5d r5u l6l r6r l8d r8.75r r9d r9.5u l10u r10.75l r11d r11.5u l12d r12d l13l r13r l14d r14d"
beat_offsets[4] = 64

# bouncy part continued
chart[5] = "l0d r0.75u r1d r1.5u l2u r2.75d r3u r3.5d l4d r4u l5u r5d l6r r6l r8d l8.75r l9d l9.5u r10u l10.75l l11d l11.5u l12d r12d l13l r13r l14d r14d"
beat_offsets[5] = 80

# fancier melody
chart[6] = "l0r r0l l1l r1r l2u r2u l4d r4d r5u r5.5u r6r r7l l8d l9u l9.5u l10l l11u r11u l12d r12d r13r r14r r15u"
beat_offsets[6] = 96

# fancier melody part 2
chart[7] = "l0d r0.75l r1d r1.5u l2l r2r r3u r4d l4.75r l5d l5.5u l6r r6l l7u r7u l8d r8d l10l r10r l12d r12d"
beat_offsets[7] = 112

# three measures rest

# bounce bounce bounce
chart[8] = "l0d r0d l0.75r r0.75l l1l r1r l1.5u r1.5u r2d r2.75u r3d r3.5u l4d r4.5d l5u r5.25u l5.5l r5.75r l6u r7u l8d r8d l9u r9d l10d r10d l11d r11u l12l r12r l12.5l r12.5r l13r r13l l14d r14d"
beat_offsets[8] = 140

# bounce bounce bounce contd
chart[9] = "l0d r0d l0.75u r0.75u l1l r1r l1.5r r1.5l l2d r2d l2.75r r2.75l l3l r3r l3.5u r3.5u l4d r4.5d l5l r5.25r l5.5u r5.75u l6d r6.5d l6.75r r6.75l l7.25l r7.25r l8d r8d l10r r10l l12d r12d l13l r13r l14d r14d"
beat_offsets[9] = 156

# melody
chart[10] = "l0r r0r l2l r2l l4l r4r l5d r5d l5.5u r5.5u l6l r6r l8d r8d l9u r9u l9.5u r9.5u l10l r10r l11u r11u l11.5u r11.5u l12d r12d l13u l14u l15l"
beat_offsets[10] = 172

# melody
chart[11] = "l0d r0d r0.75r l1l r1l l1.5u r1.5u l2r r2r l3l l4u r4u l4.75l l5r r5r l5.5u r5.5u l6d r6d r7u l8l r8r l10u r10u l12d r12d"
beat_offsets[11] = 188

offset = 6480
bpm = 107
num_notes = 0

print("offset " + str(offset))
print("bpm " + str(bpm))
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
