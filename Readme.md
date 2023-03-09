
Prerequisits
Microsoft Visual Studio: Desktop Development with C++ (such as, Visual Studio 2019 or 2022)
Strawberry Perl: A perl environment for Microsoft Windows (Required to build OpenSSL)
Netwide Assembler (NASM): An assembler for the x86 CPU architecture (Required to build OpenSSL)


Building openssl on windows
Get the source: git clone https://github.com/openssl/openssl
Install NASM and PERL. Verify that their directories are in %PATH%
Open xYZ Native Tools Promt
change dir to openssl root directory
perl Configure VC-WIN64A --prefix=DIR
DIR is the directory the libraries will be stored
nmake
nmake install

Bulding zlib
Get the source: git clone https://github.com/madler/zlib
Run xYZ Native Tools Command Prompt for Visual Studio
cd zlib
nmake /f win32/Makefile.msc 
Library and header files are in the root directory 
Create include and lib directory and move header and libreary files


Building curl
get source: https://curl.se/download.html
Run xYZ Native Tools Command Prompt for Visual Studio
change dir into curl root directory
nmake /f Makefile.vc mode=static MACHINE=x64 WITH_SSL=static SSL_PATH=OPENSSL_PATH WITH_ZLIB=static ZLIB_PATH=ZLIB_PATH

