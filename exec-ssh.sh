#!/bin/bash
docker run -p 322:22 -d --name pluto  --rm -v /home/devolution/input-recorder:/input-recorder plutonium-sdk:latest

