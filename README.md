# unisim_cpp

## Guidelines

There are a few rules to follow when creating a big project with multiple people.

### 1 Pragma once

Change this setting in Qt creator  
> Edit > Preferences > C++ > File Naming > Headers > Include guard template and check the box "Use pragma once instead"

### 2 File naming

Change this setting in Qt creator  
> Edit > Preferences > C++ > File Naming > Headers > Suffix  
> Change this to hpp  

Change this setting in Qt creator  
> Edit > Preferences > C++ > File Naming > Sources > Suffix  
> Change this to cpp

### Line endings
Change this setting in Qt creator
> Edit > Preferences > Text Editor > Behavior > File Encodings > Default line endings   
> Change this to Unix (LF)

### 3 How to write type names

Use snake case and make the first letter a capital letter to show that it is a type name

### 4 Linting rules

Please run clang-format using the given .clang-format file and check that linting passes on bitbucket piplines

### 5 No broken code in main branch

Always use a pull request to merge code in the main branch, this makes it so that no broken code can get in the main branch.

## Creating a new simulator

[How to create a client ui](create_client.md)  
[How to create a server sim](create_server.md)

## Setup

### Requirements

Fedora packages:
```bash
sudo dnf install gtest-devel cmake git ninja-build qt6-qttools-devel qt6-qtsvg-devel qt6-qtwebsockets-devel qt6-qtserialport-devel libsocketcan qt6-qtserialbus-devel libgpiod-devel
```
### Clone repo

Easiest way to download a repo from our bitbucket is to use an ssh-key. Put or generate the key on the system and copy the private key to your bitbucket account. (Cog > Personal Bitbucket Settings > SSH keys) Then run this command on the system.
```bash
git clone --recurse-submodules git@bitbucket.org:belgiantrain/unisim-cpp.git
```

### Server build

```bash
cmake -S . -B ./build/ -G Ninja
cmake --build ./build/ --target unisim_server #your sims (sim_*)
```

### Client build

```bash
cmake -S . -B ./build/ -G Ninja
cmake --build ./build/ --target unisim_client #your ui (ui_*)
```

### Running the project

Copy the libsim_* and/or libui_* together with the server or client to /usr/local/bin
```bash
sudo cp build/bin/unisim_* build/bin/libsim_* build/bin/libui_* /usr/local/bin/
```
then run
```bash
unisim_client -s $Server_IP
```
or
```bash
unisim_server
```
for more info on each executables commandline params run with -h option

### Enabling CAN bus on Raspberry Pi

Append this to /boot/config.txt
```
dtparam=spi=on
dtoverlay=mcp2515-can0,spi0-0,oscillator=16000000,interrupt=22
dtoverlay=mcp2515-can1,spi0-1,oscillator=16000000,interrupt=26
dtoverlay=spi-bcm2835-overlay
```
Copy can_dev.service to /etc/systemd/system. Reboot to apply changes.
It could be that can0 is switched with can1.
```bash
sudo cp can_dev.service /etc/systemd/system
sudo systemctl enable can_dev.service
sudo reboot
```

### Autostart server on boot

Copy unsim_cpp.service to /etc/systemd/system
If you use can you could copy can_dev.service as well
(Remove -c can0 if you don't use can)
```bash
sudo cp unisim_cpp.service /etc/systemd/system
sudo systemctl enable --now unisim_cpp.service
```