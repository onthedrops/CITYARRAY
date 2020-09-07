#!/bin/bash

apt-get update
apt-get -y install libexpat-dev libnet-ssleay-perl
apt-get -y install libdbd-mysql-perl wiringpi cpanminus festival

cpanm RPi::Pin
cpanm RPi::Const
cpanm RPi::WiringPi
cpanm MIME::Lite
cpanm Memory::Usage
cpanm IO::Socket::SSL
cpanm XML::Simple
