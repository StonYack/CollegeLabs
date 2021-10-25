#!/bin/sh

sopcinfo2swinfo.exe -input=soc_system.sopcinfo
swinfo2header.exe --swinfo soc_system.swinfo --single hps_0.h --module hps_0
