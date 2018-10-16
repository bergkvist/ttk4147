FROM ubuntu-debootstrap:12.04.5
RUN apt-get update
RUN apt-get install -y \
	g++ \
	gcc \
	make \
	perl \
	bison \
	flex \
	gettext \
	texinfo \
	patch \
	bzip2 \
	wget \
	subversion

RUN apt-get install -y libncurses5-dev

WORKDIR /folder

CMD make -k
