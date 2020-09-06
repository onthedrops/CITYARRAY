#!/bin/bash

apt-get update
apt-get -y install libexpat-dev
apt-get -y install libdbd-mysql-perl wiringpi cpanminus festival

cpanm RPi::Pin
cpanm RPi::Const
cpanm RPi::WiringPi
cpanm MIME::Lite
cpanm Memory::Usage
cpanm XML::Simple
