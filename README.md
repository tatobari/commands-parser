# Commands Parser

This little piece collection of simple functions, which are namespaced, can be used to parse "AT" like commands, using your own prefix, instead of "AT". Not sure if this is the best way to send human readable commands through a serial or bluetooth connection, but that's what I'm using it for.

This is a very simple and early version because it's all I needed for my projects. It only only supports `char[]` and `long` arguments.

It currently doesn't have an argument type identifier, so, it won't work for optional arguments that aren't at the end of a command's arguments list.

I made it to use it while working with Arduino and other microcontrollers or embedded systems.

## Build and run the example code.
1. Run `g++ -o example *.cpp`.
1. On Linux/Mac, run `./example` or `./example [COMMAND]`.

Command examples:

- `SW`
- `SW+SETSTATE`
- `SW+SETSTATE?`
- `SW+SETSTATE=?`
- `SW+SETSTATE=ON,-128000,NOW`
- `SW+SETSTATE=ON,\"slow,then fast\",-12800,NOW`
