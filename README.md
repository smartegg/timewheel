Generic Stuff
------------------------------
the timewheel implemantation in c++ by [smartegg](https://github.com/smartegg)

you are free to modify and resue this code.

you can downlod the lastest dev codes by 

* git clone git://github.com/smartegg/timewheel.git 

you can find the lastest doc  in

*  https://www.dropbox.com/sh/0hjwsmynanfw3xt/bYuEj8D4NI  


how to install?
--------------------

+ cd  timewheel/

+ ./configure

+ make    

you will find  a new file libtimewheel.a  in lib/  directory.
now you can link with you codes.
you must add timewheel/include   with your gcc HEADERS search path, 
 and link with timewheel/libtimewheel.a



===============================================================
Denpendency:
----------------
- now just support the linux platform ( I just test under ubuntu)
- version >=2.0 need the boost library support, 
         because I just use the boost list   and boost unittestframework
- version 1.0  just a naive implemantation  use pure c++. and c++ STL list

Fin
-----------
No rights reserved, do with this what you like.
Written by smartegg, <lazysmartegg@gmail.com>
