
"""
almonds@mit.edu for help

Example usage: /br/chart.py?song=1&side=0
Example response: 123000,1486,138:0,d;869,d;...
            (AKA) duration,offset,bpm:timestamp,direction;timestamp,direction;...
"""

import os

SONGS = [ None, \
   "asterisk",
   "bad_apple",
   "navillera",
   "the_blocks_we_loved",
   "monody",
   "bloody_purity",
   "ververg",
   "masquerade" ]

def request_handler(request):
   if request["method"] != "GET" \
   or not "song" in request["values"] \
   or not "side" in request["values"]:
      return "400 - Expecting a GET request with parameters: song, side"

   # Which *song* the client asked for
   song = request["values"]["song"]
   if not song.isdigit():
      return "400 - \"" + song + "\" could not be parsed as an integer"
   song = int(song)
   if song < 1 or song >= len(SONGS):
      return "400 - \"{}\" is not an appropriate song index. Expected: [1,{}]".format(song, len(SONGS)-1)
   fname = "__HOME__/br/{}.txt".format(SONGS[song])

   if not os.path.exists(fname):
      return "500 - No file found at: " + fname

   # Which *side* the client asked for
   letter = "L" if request["values"]["side"] == "0" else "R"

   duration = 0
   offset   = 0
   bpm      = 0
   out      = ""
   with open(fname, "r") as f:
      for line in f:
         line = line.strip("\n ").split(" ")
         if not line: continue
         if line[0] == "offset":
            if not line[1].isdigit(): return "500 - Could not parse offset."
            offset = int(line[1])
         elif line[0] == "bpm":
            if not line[1].isdigit(): return "500 - Could not parse bpm."
            bpm = int(line[1])
         elif line[0] == "duration":
            if not line[1].isdigit(): return "500 - Could not parse duration."
            duration = int(line[1])
         elif line[0] == letter:
            out += "{},{};".format(line[1], line[2])
   out = "{},{},{}:".format(duration, offset, bpm) + out[:-1]

   return out
