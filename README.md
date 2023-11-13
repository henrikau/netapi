# Simple NetAPI demo

Main idea: use a generic driver struct with function prototypes and let
each protocol/transport mechanism hide behind a common API.


To build the demo application:
```
gcc -o build/main src/main.c src/socket_driver.c -I include/
```
