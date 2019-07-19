FROM devkitpro/devkita64

WORKDIR /tmp/


RUN 	wget  https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb && \
	sudo dpkg -i devkitpro-pacman.deb && \ 
	dkp-pacman -S --noconfirm switch-sdl2 switch-sdl2_ttf switch-sdl2_image switch-sdl2_gfx switch-sdl2_mixer switch-mesa switch-glad switch-glm switch-libdrm_nouveau switch-libwebp switch-libpng switch-freetype switch-bzip2 switch-libjpeg-turbo switch-opusfile switch-libopus

WORKDIR /

RUN git clone https://github.com/XorTroll/Plutonium.git 

WORKDIR /Plutonium/Plutonium

RUN make

WORKDIR /

RUN git clone https://github.com/switchbrew/switch-examples.git && \
	apt-get update && \
	apt-get install -y vim

WORKDIR /input-recorder

RUN ln -s /Plutonium/Plutonium/Ouput /input-recorder/Plutonium


WORKDIR /input-recorder


EXPOSE 22

CMD ["make"]


