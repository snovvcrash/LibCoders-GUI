# LibCoders-GUI
A GUI version of the [libcoders](https://github.com/snovvcrash/libcoders "libcoders") project.

A simple library that lets you compress files (6 algorithms available: Shennon, Fano, Huffman, Bigram Huffman, Adaptive Huffman and Arithmetic coding) with Qt GUI. Made solely for educational purposes.

## Screenshot
![Screenshot-1](https://user-images.githubusercontent.com/23141800/27843655-d82b8f54-611e-11e7-8a25-9dcf27c327dd.png)

## Building and requirements
LibCoders-GUI is created via Qt framework so it needs QtCreator (or at least `qmake`) installed to be built:
* Download Qt: https://www.qt.io/ide/

Building with `qmake`:
```
mkdir release
cd release
qmake ../LibCoders-GUI.pro -r -spec linux-g++-32 CONFIG+=debug
make
```

## The code sucks!
Yep, the code sucks. This is my first Qt application so don't be too strict with me.

## Platform
LibCoders-GUI is developed only for GNU/Linux.

## Help
There is a Help button in the main window of the app which invokes the dialog with all the nessesary explanations. Also feel free to contact me at <snovvcrash@protonmail.com>.
