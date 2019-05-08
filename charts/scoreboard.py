
"""
almonds@mit.edu for help

Located at: http://608dev.net/sandbox/sc/almonds/br/scoreboard

Usage:
   Three ways:
      1. Make a POST request to update the scoreboard (pls no hack)
         * user  = username
         * score = the score
         * song  = the song index
      2. Make a simple GET request in your browser to view and browse the scoreboard
         Sort using a GET request with sort = time|user|score|song
      3. There's not actually three

Useful diagram here: 

"""

import os
from datetime import datetime
#from dateutil import parser

import sqlite3

SCOREBOARD = "__HOME__/br/BeatRevolution.db"

SONGS = [ \
   "Any",
   "Asterisk",
   "Bad Apple",
   "Navillera",
   "The Blocks We Loved",
   "Monody",
   "Bloody Purity",
   "Ververg",
   "Masquerade" ]

HTML_KWs = {
   "title":  "BeatRevolution!",
   "header": "BeatRevolution",
   "new":    "",
   "top":    "",
   "user":   "",
   "table":  "<tr><td>There's nothing here yet.</td></tr>" }

# Makes an HTML table given a list of rows, where each row
# is in turn a list of items.
def make_table(table, indent=2):
   """Takes a list of rows which are each a list of items."""
   indent = "\n" + "   " * indent
   if not table: return ""
   # Header:
   out = ["<tr>"]
   for item in table[0]:
      out.append("   <th>{}</th>".format(item))
   out.append("</tr>")
   # The rest:
   for row in table[1:]:
      out.append("<tr>")
      for item in row:
         out.append("   <td>{}</td>".format(item))
      out.append("</tr>")
   return indent.join(out)

# This constructs the list of options (the {{ songs }} part) for
# the HTML template
def make_songs_list(songs, selected_song, indent=3):
   indent = "\n" + "   " * indent
   out = []
   for i in range(len(songs)):
      if i == selected_song:
         out.append('''<option selected="selected" value="{}">{}</option>'''.format(i, songs[i]))
      else:
         out.append('''<option value="{}">{}</option>'''.format(i, songs[i]))
   return indent.join(out)

# Dynamically serves HTML using /br/html/table.html as a template
# Basically a jinja template, but hacky.
def render_template(fname, **kwargs):
   fname = "__HOME__/br/html/{}".format(fname)
   if not os.path.exists(fname):
      return "500 - No file named: " + fname
   with open(fname, "r") as f:
      html = f.read()
   for key in kwargs.keys():
      html = html.replace("{{{{ {} }}}}".format(key), kwargs[key])
      
   return html

# The mitochondria
def request_handler(request):
   connection = sqlite3.connect(SCOREBOARD)
   c = connection.cursor()
   c.execute('''CREATE TABLE IF NOT EXISTS new (t timestamp, song int, user text, score int);''')
   
   if request["method"] == "POST": # POSTing via ESP32
      # Expecting: song, user, score
      if not "song"  in request["form"] \
      or not "user"  in request["form"] \
      or not "score" in request["form"]:
         return "400 - Expecting valid form parameters: song, user, score"
      # Make sure it's valid
      user = request["form"]["user"]
      try:
         song  = int(request["form"]["song"])
         score = int(request["form"]["score"])
      except ValueError:
         return "400 - Expecting **valid** form parameters"
      if song < 1 or song >= len(SONGS):
         return "400 - There is no song with index {}".format(song)
      # OK now let's add it to the scoreboard
      # There are two tables -- one for the most recent scores and one for high scores
      c.execute('''INSERT INTO new VALUES (?,?,?,?);''', (datetime.now(), song, user, score))
      connection.commit()
      return 200 # returning also closes connection

   elif request["method"] == "GET":
      filters = {}
      song = int(request["values"].get("song", 0))
      user = request["values"].get("user")
      if song > 0:
         filters["song = ?"] = song
      HTML_KWs["songs"] = make_songs_list(SONGS, song)
      if user:
         filters["user = ?"] = user
         HTML_KWs["user"] = user
      if request["values"].get("sortby") == "top": # give highscores
         HTML_KWs["top"] = "checked"
         rows = \
         c.execute(
            '''SELECT song, user, score FROM new ''' + \
            ("WHERE " if filters else "") + " AND ".join(filters.keys()) + \
            ''' ORDER BY score DESC LIMIT 100;''', tuple(filters.values())).fetchall()
      else: # give most recent scores
         HTML_KWs["new"] = "checked"
         rows = \
         c.execute(
            '''SELECT song, user, score FROM new ''' + \
            ("WHERE " if filters else "") + " AND ".join(filters.keys()) + \
            ''' ORDER BY t DESC LIMIT 100;''', tuple(filters.values())).fetchall()
      if "esp32" in request["args"]:
         rows = ["{},{}".format(*row[1:]) for row in rows] if rows else []
         return ";".join(rows)
      if rows:
         # convert song indices to actual song names
         rows = [[SONGS[row[0]], *row[1:]] for row in rows]
         HTML_KWs["table"] = make_table([["Song", "Player", "Score"]] + rows)
      return render_template("table.html", **HTML_KWs)
   else:
      return 400
