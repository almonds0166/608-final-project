# 608-final-project

## Beat Revolution

This repo contains our team's final project for **[6.08](http://student.mit.edu/catalog/search.cgi?search=6.08&style=verbatim)** (Introduction to EECS via Interconnected Embedded Systems). (**Note**: for this reason, this repo may become private during the Spring semesters 6.08 is taught.)

![(thumbnail of video demonstration)](https://i.imgur.com/kGdH17G.png "Click for a demonstration video")

The image above links to a video demonstration. **Beat Revolution** is a combination between [Beat Saber](https://en.wikipedia.org/wiki/Beat_Saber) and [Dance Dance Revolution](https://en.wikipedia.org/wiki/Dance_Dance_Revolution), made on the [ESP32](https://en.wikipedia.org/wiki/ESP32) microcontroller with an MP3 module & SD card and two IMUs. [Look at the documentation in `site_report/`](https://koto.mit.edu/br/docs/) for more robust documentation, or contact the owner of this repo for questions not answered there. (The documentation uses [Markdeep](https://casual-effects.com/markdeep/).)

### Overview

Our project Beat Revolution is a mix between the games [Beat Saber](https://www.youtube.com/watch?v=ZZWDwHN0dLw) and [Dance Dance Revolution](https://www.youtube.com/watch?v=lpekHL_vXOc) (DDR). Beat Saber is a virtual reality game where players hold "lightsabers" to hit incoming blocks to the beat of the song. DDR is a classic arcade game where players stomp or "dance" on directional arrow pads according to the given arrows on the monitor. Beat Revolution combines the lightsaber idea from Beat Saber and the directional arrows from DDR.

The player selects from a variety of songs and charts in a scrolling menu. Past high scores for each song is displayed on the selection menu. As a song plays, arrows are shown on the LCD in DDR style (moving toward the top of the screen as the beat approaches) and the player moves two breadboards, each with an attached IMU to detect directed motion (either left, right, up, or down), in the correct direction to the beat of the song. Our system detects whether the correct motions are performed at the correct times, and calculate scores based on whether notes are hit correctly. Later on we may implement different amounts of points for getting a "perfect" hit *vs* a "decent" hit. After the song is completed, the player's score is uploaded onto a database of all past scores. The player's score and the highest n scores are displayed, where n is the number of scores we can reasonably display on the screen. The player can go back to the main menu by clicking any button, where they can select another song and continue playing the game.

### System

<p align="center">
  <img src="https://i.imgur.com/ZACBqHT.png" alt="(Diagram of system)"/>
  <img src="https://i.imgur.com/ZtsFnLg.png" alt="(Diagram of server side)"/>
</p>

### State machine

<p align="center">
  <img src="https://i.imgur.com/FoVUJh7.png" alt="(Diagram of the game's state machine for the ESP32)"/>
</p>

* **State Alpha**: During this state, we will connect to WIFI and initialize our IMUs. This also acts as a calibration state where the IMU gets the normal values for acceleration and force. These will be the new “zero”. Then, it enters state 0.
* **State 0**: This is the state for song selection. The player can use short presses to move the song selection up or down. A long press indicates that they want to select this song, and brings us to the state 1.
* **State 1**: Given the name of the user-selected song, retrieve the audio file and list of notes/timestamps from the server.
* **State 2**: State for the actual game play. During the game, the system keeps track of the next unprocessed note. In each time stamp, the system checks for whether the next note is hit, as well as updates stored acceleration data in a call to is_note_hit(time, dir). If note is hit or missed, update note_index and score. Then update the display to reflect movements and any changes in the notes and any changes to score.
* **State 3**: State to display the user's score and the list of highest scores for that song. We'll grab the list from the database on the server through a request. Press any of the buttons to move onto the song selection screen at state 0.

<!--

### Parts used

1. Speaker - 3” Diameter - 4 Ohm 3 Watt 
  * URL: https://www.adafruit.com/product/1314
  * Quantity: 2
  * Total price: $3.90
  * Description: Outputs audio from ESP32.
  * Use case: Plays the songs. Known to work with the ESP32, according to https://www.hackster.io/julianfschroeter/stream-your-audio-on-the-esp32-2e4661.

2. Stereo 3.7W Class D Audio Amplifier - MAX98306
  * URL: https://www.adafruit.com/product/987 
  * Quantity: 1
  * Total price: $8.95
  * Description: Outputs audio from ESP32 together with speakers (approach 1)
  * Use case: Plays the songs. Known to work with the ESP32, according to https://www.hackster.io/julianfschroeter/stream-your-audio-on-the-esp32-2e4661.

3. Longruner Dfplayer Mini MP3 Player Module for Arduino LK01
  * URL: https://www.amazon.com/dp/B01MXOFAE4 
  * Quantity: 1
  * Total price: $5.52
  * Description: Outputs audio from ESP32 together with speakers (approach 2)
  * Use case: Plays the songs.

4. SanDisk 16GB Ultra microSDXC UHS-I Memory Card with Adapter
  * URL: https://www.amazon.com/Sandisk-Ultra-Micro-UHS-I-Adapter/dp/B073K14CVB/
  * Quantity: 1
  * Total price: $5.79
  * Description: Outputs audio from ESP32 together with speakers (approach 2)
  * Use case: Plays the songs.

5. ELEGOO 6PCS 170 tie-Points Mini Breadboard kit for Arduino
  * URL: https://www.amazon.com/ELEGOO-6PCS-tie-Points-Breadboard-Arduino/dp/B01EV6SBXQ/
  * Quantity: 1
  * Total price: $6.99
  * Description: Where the imu sticks to, at the end of each of our sabers (sticks).
  * Use case: Use with imu to detect motion.

-->
