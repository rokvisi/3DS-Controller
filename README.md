# 3DS-Controller
Sending keystrokes from the 3DS to a computer.

Add the `HandController.3dsx` file to your luma 3ds directory as `SD/3DS/HandController.3dsx` and create a `SD/3DS/data/HandController/controller.ini` file which will store the ip and port information.

Example contents of the `controller.ini` file =>
`192.168.8.101 8889`

The IP and PORT can be anything you choose, but make sure it isn't being used by other applications on your PC.
You can use local and non-local ips, just make sure to port-forward them.

This application can be paired with anything, even your own implementation of the `3DS-Controller-PC`.
Since the app sends 12 bytes of data to a UDP Server anywhere in the world everytime a button is pressed.
The data is constructed exactly like this:
```
std::array<uint32_t, 3> get_key_states() {
	hidScanInput();
	return { hidKeysDown(), hidKeysHeld(), hidKeysUp() };
}
```
Meaning the data can be aquired by any service and interpreted in any way imaginable.
This will be extended in the future to include touch screen and cstick data.
