# edit chart[], beat_offsets[], offset, and bpm 
# basically just made chart[] by hand
# the script will print out the chart in the format that the server expects


chart = [""]*20
beat_offsets = [0]*20

# there's some measure before this but it doesn't quite feel like a full measure

# intro
chart[0] = "l0d r3u l4d r6r r7u l8d r10.5d l11d r12d l14l r15u r16r l18l r20d l2
1d r22d l23u r24l r26r l28d"
beat_offsets[0]=0

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
