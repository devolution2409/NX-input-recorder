# NX Input Recorder

This is the repository containing the source necessary to build the input-recorder nro.
This homebrew goal is to record gameplay as a pair of time and keypresses, in order to be able to be used as a starting material for TAS.

## Getting Started

### Prerequisites

To be able to build this `.nsp`, one must download and compile the latest version of [Plutonium](https://github.com/XorTroll/Plutonium) and place the `include` and `lib` folder in the Plutonium folder. (The Plutonium folder in git is just a symlink to the compiled Plutonium).

Or you can use the following Dockerfile in order to create an image with devkitpro and plutonium.

``` 
FROM devkitpro/devkita64

WORKDIR /tmp/

#Getting pacman and the deps needed for Plutonium
RUN 	wget  https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb && \
	sudo dpkg -i devkitpro-pacman.deb && \ 
	dkp-pacman -S --noconfirm switch-sdl2 switch-sdl2_ttf switch-sdl2_image switch-sdl2_gfx switch-sdl2_mixer switch-mesa switch-glad switch-glm switch-libdrm_nouveau switch-libwebp switch-libpng switch-freetype switch-bzip2 switch-libjpeg-turbo switch-opusfile switch-libopus

WORKDIR /

#Cloning and compiling Plutonium
RUN git clone https://github.com/XorTroll/Plutonium.git 

WORKDIR /Plutonium/Plutonium

RUN make

WORKDIR /

RUN git clone https://github.com/switchbrew/switch-examples.git && \
	apt-get update && \
	apt-get install -y vim

WORKDIR /input-recorder

#Symlinking it
RUN ln -s /Plutonium/Plutonium/Ouput /input-recorder/Plutonium


CMD ["make"]

```


Once this is done you can make the image by using the following
```docker build -t plutonium-sdk:latest .```

and run the make.sh script :)

## Versioning

Whatever, this this use semantic versionning, go check out changelogs in the release tab since they are all available forsenE.


## License

Blabla, some licence shit that says this is open source and free to fork. But please acknowledge me so i can ride into valhalla shinny and chrome.

## Acknowledgments

💿 OMEGALUL 




