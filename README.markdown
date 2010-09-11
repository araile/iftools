Interactive Fiction
===================

* **unblorb** – Program to extract the executable data from a Blorb file.

        $ make unblorb
        $ bin/unblorb < Shade.zblorb > /tmp/test
        $ md5sum Shade.z5 /tmp/test
        299d6cd034af144c2c7c94c2fdceb4f5  Shade.z5
        299d6cd034af144c2c7c94c2fdceb4f5  /tmp/test
