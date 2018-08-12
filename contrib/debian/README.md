
Debian
====================
This directory contains files used to package jfkbitcoin1776d/jfkbitcoin1776-qt
for Debian-based Linux systems. If you compile jfkbitcoin1776d/jfkbitcoin1776-qt yourself, there are some useful files here.

## jfkbitcoin1776: URI support ##


jfkbitcoin1776-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install jfkbitcoin1776-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your jfkbitcoin1776-qt binary to `/usr/bin`
and the `../../share/pixmaps/jfkbitcoin1776128.png` to `/usr/share/pixmaps`

jfkbitcoin1776-qt.protocol (KDE)

