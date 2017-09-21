Interactive Fiction stuff
=========================

* **unblorb** – Program to extract the executable data from a Blorb file.

        $ make unblorb
        $ bin/unblorb < Shade.zblorb > /tmp/test
        $ md5sum Shade.z5 /tmp/test
        299d6cd034af144c2c7c94c2fdceb4f5  Shade.z5
        299d6cd034af144c2c7c94c2fdceb4f5  /tmp/test

* **zplay.py** – Wrapper for frotz. Looks for games in `$ZCODE_PATH` or
  `$INFOCOM_PATH`.

        $ ./zplay.py advent
        Games:
             1. Adventure
             2. Adventureland
             3. Danger! Adventurer At Work!

        Choose a game:
        >
