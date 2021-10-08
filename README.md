# 3DS-Controller
Sending keystrokes from the 3DS to a computer (or more).

## How to use
1. Download and compile the latest source code of [3DS-Controller-PC](https://github.com/rokvisi/3DS-Controller-PC). *Check the bottom of the readme for more info.

2. Download the latest release of [HandController.3dsx](https://github.com/rokvisi/3DS-Controller/releases/tag/v0.1).

3. Place and create files and folders so you have the same directory structure on the SD Card of your 3DS.
```
📂 SD Card
┣ 📂 3ds
┃ ┣ 📜 HandController.3dsx
┃ ┣ 📂 data
┃ ┃ ┣ 📂 HandController
┃ ┃ ┃ ┗ 📜 controller.ini
```
| File                 | Full Path                                       |
| -------------------- | ----------------------------------------------- |
| HandController.3dsx  | `SDCard\3ds\HandController.3dsx`.               |
| controller.ini       | `SDCard\3ds\data\HandController\controller.ini` |

4. Add the `IP` and `PORT` that your computer will be listening on to the `controller.ini` file.

Example contents of `controller.ini`:
```
192.168.8.104 8889
```
### Note
Keep in mind that if you use a local IP, your 3DS also has to be on the same network.

On the other hand if you use a public IP, your 3DS has to be on a different network and the port needs to be forwarded correctly.

------------------

## Some path theory
Please note that in reality, the provided `.3dsx` file can be placed anywhere on the system.

To make it clearer for how it looks for the `controller.ini` file, here's a table that hopefully explains it:
| Started from the homebrew launcher| relative path           | Will look for                            |
| ----------------------------------| ----------------------- | ---------------------------------------- |
| icon                              | `.3dsx` directory       | `data\HandController\controller.ini`     |
| 3dslink NetLoader                 | `root` directory        | `3DS\data\HandController\controller.ini` |

I am reluctant to use absolute paths, since I want to keep the behaviour of using the relative paths  in case some users keep their homebrew apps in a different location.

`3dslink NetLoader`, however, forces me to.

## How to edit and compile source code yourself

1. Download Docker Desktop.
2. Pull the provided [docker image](https://github.com/users/rokvisi/packages/container/package/3ds-controller).\
The image includes a `devkitarm` development environment and a ready-to-use makefile with partial `C++20` support enabled.
3. Start the image as a container and attach `VSCode` to it. (Or use any other form of containerized development.)

I also advise you to create a volume when launching the container so that you can freely access the container's filesystem for build output files or anything else.

If not, you can use the built-in `3DSLink` tool to very conveniently upload the compiled `.3dsx` to the console.\
This is the recommended way to develop 3DS Homebrew.

The syntax is like this: `>3dslink {path/to/file} -a {ip-of-3ds}`\
Example: `>3dslink out/HandController.3dsx -a 192.168.8.101`

You can find tutorials for all of this online if you need any help.

## Want more?
The 3DS application sends 12 bytes of data to a UDP Server everytime a button is pressed.\
The data is constructed as follows:
```c++
std::array<uint32_t, 3> get_key_states() {
    hidScanInput();
    return { hidKeysDown(), hidKeysHeld(), hidKeysUp() };
}
```
This means that it can be paired with anything that can recieve and interpret said data.\
Like your phone or a smart microwave.

This will be extended in the future to include touch screen and c-stick data.
