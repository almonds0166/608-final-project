
"""
almonds@mit.edu for help

Example usage: /br/chart.py?song=bad_apple&left
Example response: 1486,138:0,d;869,d;...
            (AKA) offset,bpm:timestamp,direction;timestamp,direction;...
"""

import os

def request_handler(request):
   if request["method"] != "GET" \
   or not "song" in request["values"] \
   or not "side" in request["values"]:
      return "400 - Expecting a GET request with parameters: song, side"

   # Which song the client asked for
   song = request["values"]["song"]
   song = song.lower()
   fname = "__HOME__/br/{}.txt".format(song)

   if not os.path.exists(fname):
      return "500 - No file named: " + fname

   # Which side the client asked for
   letter = "L" if request["values"]["side"] == "0" else "R"

   offset = 0
   bpm    = 0
   out    = ""
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
         elif line[0] == letter:
            out += "{},{};".format(line[1], line[2])
   out = "{},{}:".format(offset, bpm) + out[:-1]

   return out
