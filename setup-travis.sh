#/bin/sh

if ( test "`uname -s`" = "Darwin")
then
else
  echo "yes" | sudo add-apt-repository ppa:kalakris:cmake
  sudo apt-get update -qq
  sudo apt-get install cmake libsdl2-dev
fi
