sudo apt install build-essential uuid-dev gcc git nasm python3-distutils
sudo apt install iasl || sudo apt install acpica-tools

echo ""
echo "Make sure GCC version is less than 10. If not - install gcc-10 package"
echo "And change '/usr/bin/gcc' to be pointing at '/usr/bin/gcc-10'"
