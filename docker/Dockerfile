# stole from https://github.com/eduardofilo/RG350_buildroot_buster_docker
FROM debian:buster-slim
ENV DEBIAN_FRONTEND noninteractive

# This is where we mount our various things to build, usually
# in their own dir under /source
RUN mkdir /root/source/
WORKDIR /root/

ENV TZ=America/Toronto
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get -y -o Dpkg::Options::="--force-confold" upgrade

RUN apt-get install -y \
      automake \
      bc \
      build-essential \
      ccache \
      cmake \
      cpio \
      dosfstools \
      g++-multilib \
      gcc-multilib \
      gettext \
      git \
      libncurses5-dev \
      mercurial \
      mtools \
      python \
      python3 \
      rsync \
      squashfs-tools \
      subversion \
      unzip \
      vim \
      wget \
      zip

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Snag its own copy of the buildroot
RUN git clone https://github.com/od-contrib/buildroot-rg350-old-kernel /root/buildroot

ENV FORCE_UNSAFE_CONFIGURE=1
ENV PATH="/root/buildroot/output/host/usr/bin:$PATH"

WORKDIR /root/buildroot/
# Allow parallel compilation
ENV BR2_JLEVEL=0

# Meat an potatoes
RUN make rg350_defconfig BR2_EXTERNAL=board/opendingux
RUN make toolchain

# I ran into errors if I didn't make each generation of SDL together
RUN make sdl
RUN make sdl_image
RUN make sdl_ttf
RUN make sdl_mixer

RUN make sdl2
RUN make sdl2_ttf

# Needed for things that invoke the rumble feature
RUN make libshake
RUN libtool --finish /usr/lib

