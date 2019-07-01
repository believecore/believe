
Debian
====================
This directory contains files used to package believed/believe-qt
for Debian-based Linux systems. If you compile believed/believe-qt yourself, there are some useful files here.

## believe: URI support ##


believe-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install believe-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your believeqt binary to `/usr/bin`
and the `../../share/pixmaps/believe128.png` to `/usr/share/pixmaps`

believe-qt.protocol (KDE)

