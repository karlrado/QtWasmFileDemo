# Sample File I/O for Native and Web Qt Applications

## Motivation
The requirement here is to create a simple cross-platform application that runs on local machines and can run in a web browser.
The application must be able to read and write files.

For native applications, the file access isn't a problem.
But for browser applications, the sandboxed code running in the web browser needs to be able to access the local filesystem, which it cannot do openly and by default.
When it wants to read a file, we can use the Qt support to present the user with a File Open dialog that lets the user control what the web application is reading.
For writing a file, the web application initiates a download dialog with the user and the file is written under the user's control to a location selected by the user.

## Operation
This application is very simple.

### Native
Click on the box to select a file.
The program reads it and displays the contents in a text box.
It also appends `.tmp` to the filename and writes the file back out to the same directory.

### Web
Click on the box to select a file.
The program reads it and displays the contents in a text box.
It also appends `.tmp` to the filename and starts a file download in the web browser.

## Building

### Native
It is easiest to open the project in QtCreator and build all the configurations.

### Web
Assumes that a recent Visual Studio is installed and the emscripten SDK is installed.
* Start a Windows Developer Command Prompt
* Change to the Emscripten SDK directory and run `emsdk_env.bat`.
* Create a build directory for the WASM build and change to it
* Run qmake: `\qt\5.15.2\wasm_32\bin\qmake <path to source code dir>`
* `nmake`

When complete, copy these files to the web server:

* FileDemo.html
* FileDemo.js
* FileDemo.wasm
* qtloader.js
