#!/bin/bash

apt-get install hostapd
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
rfkill unblock wlan

apt-get install bind9


