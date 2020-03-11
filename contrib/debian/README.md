
Debian
====================
This directory contains files used to package betheld/bethel-qt
for Debian-based Linux systems. If you compile betheld/bethel-qt yourself, there are some useful files here.

## bethel: URI support ##


bethel-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bethel-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bethel-qt binary to `/usr/bin`
and the `../../share/pixmaps/bethel128.png` to `/usr/share/pixmaps`

bethel-qt.protocol (KDE)

