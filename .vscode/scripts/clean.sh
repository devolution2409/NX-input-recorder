#!/bin/bash


#LULW i'm stupid this was way easier
docker run --name pluto  --rm -v $(pwd):/input-recorder plutonium-sdk make clean;

# rm -rf *.elf
# rm -rf *.nacp
# rm -rf *.nro
# rm -rf *.nsp
# rm -rf *.nso
# rm -rf *.npdm
