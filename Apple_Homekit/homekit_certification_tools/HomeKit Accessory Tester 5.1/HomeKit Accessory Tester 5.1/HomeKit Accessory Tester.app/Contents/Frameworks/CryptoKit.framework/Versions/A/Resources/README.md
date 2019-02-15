###  What is this directory?
This directory contains sources from CoreUtils that are required by CryptoKit

###  Why is it needed?
As of MacOS SDK 17A360, the CoreUtils internal framework does not expose symbols from SRPUtils.h.
This causes linker issues when building CryptoKit.

The sources in this directory have been copied into the project as a workaround.  There is also an entry in
the top-level CryptoKit.h to include needed header files.
