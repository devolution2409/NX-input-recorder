#!/bin/bash

rm -rf ./dist;

docker run --name pluto  --rm -v $(pwd):/input-recorder plutonium-sdk make -j8;

result=($?);

if [ $result = 0 ]
then
	#sudo chown -R devolution .; 
	#creating dist stuff
	## the nsp

	mkdir -p ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;);
	echo "Moving .nsp to dist folder..";
	mv ./input-recorder.nsp dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/exefs.nsp; 

	## the boot flag
   	mkdir -p ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/flags;
   	echo "Creating boot2.flag..";
	touch ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/flags/boot2.flag;

else

	echo "Error during make. Aborting..";

fi

