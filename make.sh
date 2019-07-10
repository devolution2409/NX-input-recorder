#!/bin/bash
docker run --name pluto  --rm -v /home/devolution/input-recorder:/input-recorder plutonium-sdk &&
sudo chown -R devolution .

#creating dist stuff
## the nsp 
rm -rf ./dist
mkdir -p ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)
echo "Moving .nsp to dist folder.."
mv ./input-recorder.nsp dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/exefs.nsp 

## the boot flag
mkdir -p ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/flags
echo "Creating boot2.flag.."
touch ./dist/atmosphere/titles/$(grep "title_id" config.json  -m1 | cut -f 3 | sed 's/,//g' | sed 's/"//g'  ;)/flags/boot2.flag
## clean all the stuff
./clean.sh
