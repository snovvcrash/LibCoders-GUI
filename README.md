# LibCoders-GUI
Simple Qt application that lets you compress files (6 algorithms available: Shennon, Fano, Huffman, Bigram Huffman, Adaptive Huffman and Arithmetic coding).

Made for educational purposes.

# Screenshot
![Screenshot-1](https://user-images.githubusercontent.com/23141800/27843655-d82b8f54-611e-11e7-8a25-9dcf27c327dd.png)

# Building and requirements
LibCoders-GUI is created via Qt framework so it needs QtCreator (or at least `qmake`) installed to be built:
* Download Qt: https://www.qt.io/ide/

Building with `qmake`:
```
mkdir release
cd release
qmake ../LibCoders-GUI.pro -r -spec linux-g++-32 CONFIG+=debug
make
```

# Weird code?
Yep, a little bit. That's because this app is just a GUI adaptation of the CLI version [libcoders](https://github.com/snovvcrash/libcoders "libcoders") library.

# Platform
LibCoders-GUI was developed for GNU/Linux only.
