
"""
almonds@mit.edu for help

Example usage: /br/chart.py?song=2&left
Example response: 1486,138:0,d;869,d;...
            (AKA) offset,bpm:timestamp,direction;timestamp,direction;...
"""

SONGS = ["bad_apple"]

def request_handler(request):
   if request["method"] != "GET" \
   or not "song" in request["values"]:
      return "400 - Expecting a GET request with parameter: song"

   # Which song the client asked for
   song = request["values"]["song"]
   if not song.isdigit(): return "400"
   song = int(song)
   if song < 0 or song >= len(SONGS):
      return "400 - We only have {} songs.".format(len(SONGS))

   # Which side the client asked for
   letter = "L" if "left" in request["args"] else "R"

   offset = 0
   bpm    = 0
   out    = ""
   with open("__HOME__/br/{}.txt".format(SONGS[song]), "r") as f:
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
