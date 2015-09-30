#/bin/sh

if ( test "`uname -s`" = "Darwin")
then
  echo "no"
else
  echo "yes" | sudo add-apt-repository ppa:kalakris:cmake
  sudo apt-get update -qq
  sudo apt-get install cmake mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
fi
