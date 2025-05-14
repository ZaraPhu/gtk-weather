# gtk-weather

## Build Instructions
To build this app from source, you will require the `cmake` and `make` CLI utilities + the GTK4 library and libcurl installed on your system.

Clone this repository to your local machine by opening up a shell session and executing the command
```sh
git clone https://github.com/ZaraPhu/gtk-weather.git
```
and navigate into the project root directory by running
```sh
cd gtk-weather
```
Create and navigate into a `build` sub-directory by executing
```sh
mkdir build && cd build
```
You can run CMake using
```sh
cmake ..
```
which takes the source files from the parent directory and creates a build Makefile in the current directory. You can  now compile the source by running
```sh
make
```
and the program can be started using
```sh
./gtk_weather
```
Later on, I will figure out how to create release builds which will be put out in this GitHub repo.
