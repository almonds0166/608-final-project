chart = [""]*40
beat_offset = [0]*40

# first normal piece first half
chart[0] = "l0d r0d r1d r1.5d l2l r2d r3d r3.5d l4u r4d r5d r5.5d l6r r6d r7d r7.5d l8d r8d r9d r9.5d l10r r10d r11d l12u r12u l13d r13d l14d r14d"
beat_offset[0] = 0

# first normal piece second half
chart[1] = "l0.5u r1.5u l2l r2r r3d r3.5d l4u r4d r5.5d l6r r6d r7d r7.5d l8d r8d r9d r9.5d l10r r10d r11d l12u r12u l13d r13d l14d r14d"
beat_offset[1] = 16

# first crazy piece, first quarter
chart[2] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[2] = 32

# first crazy piece, second quarter
chart[3] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[3] = 36

# first crazy piece, third quarter
chart[4] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[4] = 40

# first crazy piece, fourth quarter (end) 
chart[5] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.25u l2.75d r2.75d l3u r3u"
beat_offset[5] = 44

# second crazy piece, first quarter
chart[6] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[6] = 48

# second crazy piece, second quarter (special)
chart[7] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.5u l2.75d r3d l3.5u r3.5u"
beat_offset[7] = 52

# second crazy piece, third quarter
chart[8] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[8] = 56

# second crazy piece, fourth quarter (end) (different)
chart[9] = "l0d r0d l0.75d r0.75d l1.5d r1.5d l2d r2d l3d r3.25d l3.5u r3.75u"
beat_offset[9] = 60

# last note of crazy part, plus short break
chart[10] = "l0d r0d"
beat_offset[10] = 64

# 8 beats per block, one movement every half beat

# O block
chart[11] = "l0d l0.5d l1r l1.5r l2u l2.5u l3l l3.5l"
beat_offset[11] = 72

# S block
chart[12] = "r0d r0.5d r1r r1.5d r2r r2.5u r3u r3.5l r4u r4.5l" 
beat_offset[12] = 80

# I block
chart[13] = "l0d l0.5d l1d l1.5d l2r l2.5u l3u l3.5u l4u l4.5l"
beat_offset[13] = 88

# T block
chart[14] = "r0d r0.5r r1d r1.5r r2u r2.5r r3u r3.5l r4l r4.5l"
beat_offset[14] = 96

# Z block
chart[15] = "l0d l0.5r l1d l1.5r l2r l2.5u l3l l3.5u l4l l4.5l"
beat_offset[15] = 104

# J block
chart[16] = "r0d r0.5r r1r r1.5d r2r r2.5u r3u r3.5l r4l r4.5l"
beat_offset[16] = 112

# L block
chart[17] = "l0d l0.5d l1d l1.5r l2r l2.5u l3l l3.5u l4u l4.5l"
beat_offset[17] = 120

# back to main theme

# second normal piece first half
chart[18] = "l0d r0d r1d r1.5d l2l r2d r3d r3.5d l4u r4d r5d r5.5d l6r r6d r7d r7.5d l8d r8d r9d r9.5d l10r r10d r11d l12u r12u l13d r13d l14d r14d"
beat_offset[18] = 128

# second normal piece second half
chart[19] = "l0.5u r1.5u l2l r2r r3d r3.5d l4u r4d r5.5d l6r r6d r7d r7.5d l8d r8d r9d r9.5d l10r r10d r11d l12u r12u l13d r13d l14d r14d"
beat_offset[19] = 144

# back to crazy

# first crazy piece, first quarter
chart[20] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[20] = 160

# first crazy piece, second quarter
chart[21] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[21] = 164

# first crazy piece, third quarter
chart[22] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[22] = 168

# first crazy piece, fourth quarter (end) 
chart[23] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.25u l2.75d r2.75d l3u r3u"
beat_offset[23] = 172

# second crazy piece, first quarter
chart[24] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[24] = 176

# second crazy piece, second quarter (special)
chart[25] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.5u l2.75d r3d l3.5u r3.5u"
beat_offset[25] = 180

# second crazy piece, third quarter
chart[26] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[26] = 184

# second crazy piece, fourth quarter (end) (different)
chart[27] = "l0d r0d l0.75d r0.75d l1.5d r1.5d l2d r2d l3d r3.25d l3.5u r3.75u"
beat_offset[27] = 188

# final repeat

# second crazy piece, first quarter
chart[28] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[28] = 192

# second crazy piece, second quarter (special)
chart[29] = "l0d r0d r0.5u l1d r1l r1.5r l1.75d r1.75d l2.25u r2.5u l2.75d r3d l3.5u r3.5u"
beat_offset[29] = 196

# second crazy piece, third quarter
chart[30] = "l0d r0d r0.5u l1d r1r r1.5l l1.75d r1.75d l2.25u r2.75u l3d r3.5d"
beat_offset[30] = 200

# second crazy piece, fourth quarter (end) (different)
chart[31] = "l0d r0d l0.75d r0.75d l1.5d r1.5d l2d r2d l3d r3.25d l3.5u r3.75u"
beat_offset[31] = 204

chart[32] = "l0d r0d"
beat_offset[32] = 208


offset = 8050
bpm = 134
duration = 106000

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
