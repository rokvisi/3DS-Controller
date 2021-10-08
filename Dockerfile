FROM devkitpro/devkitarm
LABEL Rokas Višinskas rokas.post@gmail.com

#COPY . /root/

COPY include /root/include
COPY romfs /root/romfs
COPY src /root/src

COPY Dockerfile /root/Dockerfile
COPY Makefile /root/Makefile

COPY .gitignore /root/.gitignore
COPY .dockerignore /root/dockerignore