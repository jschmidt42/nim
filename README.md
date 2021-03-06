# [node.js](http://www.nodejs.org) instance manager

node.js instance manager is a simple application that lets you manage the execution of multiple instance of node.js applications. 
You can play/stop many instances and monitor them using a simple UI that reside in the system tray. You can 
specify the port of all instances and many other process parameters.
 
[Download (Win64) ~6.2MB](https://github.com/jschmidt42/nim/raw/master/Build/VS2010/Setup/Release/NIM.msi)  
[Source on GitHub](https://github.com/jschmidt42/nim) 

## Motivation

It is useful to have a single view on all your node instances that are running and being able to quickly restart them, etc.

## Usage

![main ui](https://raw.github.com/jschmidt42/nim/master/Docs/Screenshots/main.png)

- You add a new node instance slot by pressing the **Add** button.
- Then browse for a **node.js startup script** *(same script you would start using node.exe on the command line)*.
- If your node application is a server that needs a given port, enter it in the **port field**.
- Finally press the **Play** button to start the node process. If it didn't crashed at startup, the icon should pass to a stop button state.
	- Pressing the stop button will kill the node process
- *Repeat the process for other node instances.*

### Additional options

- More node instance options...
	- ![more options](https://raw.github.com/jschmidt42/nim/master/Docs/Screenshots/more-options.png)
		- **Open Browser**: opens a browser view on http://localhost:port
		- **Open Explorer**: opens a file explorer view where the startup scripts is
		- **Edit Env. Vars**: edits environment variables that will be passed to the node process when started.
			- Special variables:
				- INITSTART=1: start node when application starts
				- HTTPROOT=a_root/sub_folder: path concatenated when opening the node in the browser (e.g. *http://localhost:9999/a_root/sub_folder*)
				- DEBUGPORT=2600: used to override the 5858 default node debug port
				- NODEPATH=C:/Programe Files/nodejs/node.exe: override which node.exe to use when launching that specific instance.
			- ![vars](https://raw.github.com/jschmidt42/nim/master/Docs/Screenshots/vars.png)
		- **Log**: opens a log window of the running node process.
			- - ![log](https://raw.github.com/jschmidt42/nim/master/Docs/Screenshots/log.png)
		- **Debug**: adds the *debug* argument to the node process to be debugged
		- **Delete**: deletes the node instance slot 
- When you minimize the application it will be hidden from the application taskbar and a icon will be available in the system tray to re-open it.
	- ![system tray](https://raw.github.com/jschmidt42/nim/master/Docs/Screenshots/system-tray.png)

### Configurations

<PUT IMAGE>

- Node.js executable path
- Start as minimized

## Improvements (TODO)

- Port on Mac (partly done, just need to fix UI a bit)
- Port on Linux
- Start all nodes at once
- Group nodes
	- Start all nodes of a group
- Sort node instance slots (using a small drag and drop widget?)
- Save logs
- See in the log window stderr
- Detect all started node.exe process and list them as new slots

## Requirements

### Windows

- Install Visual Studio 2010
- Install [Qt libraries 4.8.4 for Windows (VS 2010, 234 MB)](http://releases.qt-project.org/qt4/source/qt-win-opensource-4.8.4-vs2010.exe "QT 4.8.4")
- Install [Visual Studio Add-in 1.1.11 for Qt4](http://releases.qt-project.org/vsaddin/qt-vs-addin-1.1.11-opensource.exe)

### Mac

- Install xcode tools (xcode > preferences > downloads > install command line tools)
- `git clone` && `configure` && `make` && `sudo make install`[QT 4.8.5 for Mac](http://qt.gitorious.org/qt/qt)
- `> export PATH=/usr/local/Trolltech/Qt-4.8.5/bin:$PATH`
- `> qmake -spec /usr/local/Trolltech/Qt-4.8.5/mkspecs/unsupported/macx-clang/`
- `> make`
- `> ls build.output/nim.app`