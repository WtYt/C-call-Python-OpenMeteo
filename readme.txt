premise
to compile this, following are required.
  Python3
  - version is 3.10.* (on ubuntu 22.04(WSL), stable version(3.10.12) is recommended.
    developed on this environment. won't work on 2.*)
  Python3.10-dev
  - this is necessary to include Python.h.

how to compile
1, file location is following
root(not root of system)
|
+---ccode
|   |
|   +---pyMeteo.h
|   +---pyMeteo.c
|   +---weather_cache.h
|   +---weather_cache.c
|   +---weather_client.c
|   +---weather_server.c
|
+---pycode
    |
    +---coordinates_searcher.py
    +---Meteo.py
    +---open_meteo_url_builder.py
    +---open_meteo_utility.py

2, locate Python.h and object files of include files, and compile server.
  this is necessary to compile
  in my development, compile command of server is following.
  $ gcc weather_server.c weather_cache.c pyMeteo.c -I/usr/include/python3.10/ -L/usr/lib/python3.10/config -lpython3.10 -o weather_server
  path of them are located after -I option and -L option.
  but generally this command is available.
  $ gcc weather_server.c weather_cache.c pyMeteo.c $(python3.10-config --cflags --ldflags --embed) -o weather_server
  "$(python3.10-config --cflags --ldflags --embed)" will show the necessary options to compile this program.

3, compile client
  $ gcc weather_client.c -o weather_client
