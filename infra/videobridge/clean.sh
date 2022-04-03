#!/bin/bash

mkdir /janus/lib/janus/loggers
cd /janus/etc/janus \
&& for FILE in *; \
	do echo $FILE; 
		if [[ $FILE =~ .sample$ ]];  
			then mv $FILE ${FILE::-7}; 
		fi; 
	done;
