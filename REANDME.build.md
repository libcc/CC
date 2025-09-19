****
# The libCC Library
Copyright 2006-present The libCC Open Source Group

This product includes software developed by The libCC Open Source Group (https://libcc.cn/).

-------------------------------------------------------------------------------

This product contains a modified portion of 'zlib', compute the Adler-32 checksum of a data stream, which can be obtained at:

  * LICENSE:
    * pkg/zlib.pkg/LICENSE.md (Zlib License)
  * HOMEPAGE:
    * http://zlib.net/

This product optionally depends on 'openssl', a TLS/SSL and crypto library,
which can be obtained at:

  * LICENSE:
    * pkg/openssl.pkg/LICENSE.md (OpenSSL License and SSLeay license)
  * HOMEPAGE:
    * https://www.openssl.org/
  * Download Page:
    * https://openssl-library.org/source/
  * Windows Download Page:
    * https://slproweb.com/products/Win32OpenSSL.html

This product optionally depends on 'sqlite3', a self-contained, high-reliability, embedded,
full-featured, public-domain, SQL database engine, which can be obtained at:

  * LICENSE:
    * Public Domain
  * HOMEPAGE:
    * http://sqlite.org/

-------------------------------------------------------------------------------

# Build libCC
  * Debug
    * make .a platform=linux debug=1
  * Release
    * make .a platform=linux

  * libcc.widgets
    * make .so platfrom=linux target=widgets all=1 debug=1

  * dynamiclib
    * make .dylib platform=osx
    * make .so platform=linux

  * Windows
    * proj.Win/libcc.vcxproj

  * Android
    * cd proj.Android/JNI
    * Debug
      * $NDK/ndk-build NDK_DEBUG=1
    * Release
      * $NDK/ndk-build [NDK_UNICODE=1]


# Install MySQL8 devel
  * Centos
    * wget https://repo.mysql.com//mysql80-community-release-el7-7.noarch.rpm
    * rpm -ivh mysql80-community-release-el7-7.noarch.rpm
    * yum -y install mysql-devel

# OpenSSL Download Page for Windows
  * https://slproweb.com/products/Win32OpenSSL.html

  * iOS
    * ./Configure ios64-cross --prefix=/opt/libcc/include/openssl
    * make && make install
    *
      * lipo -create arm64/libssl.a x86_64/libssl.a -output universal/libssl.a
# SQLite Download Page
  * https://www.sqlite.org/download.html
  * download：sqlite-amalgamation-3500100.zip sqlit3 header
  * MSYS2 build sqlite3
    * gcc -shared -o sqlite3.dll sqlite3.c -Wl,--out-implib,libsqlite3.a
    * gcc -DSQLITE_ENABLE_COLUMN_METADATA sqlite3.c -shared -o sqlite3.dll -Wl,--out-implib,libsqlite3.a

# Linux Ubuntu/Debian
  * sudo apt-get install libsqlite3-dev
  * sudo apt-get install libmysqlclient-dev

# macOS Homebrew
  * brew install sqlite
  * brew install mysql-client

# Download ODBC driver for MacOSX
  * https://learn.microsoft.com/en-us/sql/connect/odbc/download-odbc-driver-for-sql-server?view=sql-server-ver17&redirectedfrom=MSDN
  * brew tap microsoft/mssql-release https://github.com/Microsoft/homebrew-mssql-release
  * brew update
  * HOMEBREW_ACCEPT_EULA=Y brew install msodbcsql18 mssql-tools18