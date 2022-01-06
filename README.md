# Commands Parser

This collection of functions, which are namespaced, can be used to parse "AT" like commands, using your own prefix, instead of "AT", if you want to create your own AT like commands for your own devices. Not sure if this is the best way to send human readable commands through a serial or bluetooth connection to a custom device, but that's what I'm using it for.

This is a very simple and early version because it's all I needed for my projects. It only only supports `char[]` and `long` arguments. I'll probably add support for  `float` arguments soon.

It currently doesn't have an argument type identifier, but I'll also try to implement something to help with it so it supports function overload.

I made it to use it while working with Arduino and other microcontrollers or embedded systems which I need to control from a comfortable device such as a computer or a smartphone.

## Build and run the example code.
1. Run `g++ -o example *.cpp ./TBCommandsParser/*.cpp`.
1. On Linux/Mac, run `./example` or `./example [COMMAND]`.

Command examples:

- `SW`
- `SW+SETSTATE`
- `SW+SETSTATE?`
- `SW+SETSTATE=?`
- `SW+SETSTATE=ON,-128000,NOW`
- `SW+SETSTATE=ON,\"slow,then fast\",-12800,NOW`
