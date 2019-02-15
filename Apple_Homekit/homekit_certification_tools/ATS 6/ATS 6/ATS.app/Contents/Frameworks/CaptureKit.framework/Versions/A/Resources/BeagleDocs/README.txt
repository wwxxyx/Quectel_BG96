                  Beagle Protocol Analyzer Software
                  ---------------------------------

Introduction
------------
This software is used to interface with the Beagle Protocol Analyzers
(I2C/SPI/MDIO, USB 12, and USB 480).  It provides APIs in a number of
languages for maximum development flexibility.


Directory Structure
-------------------
c/            - C/C++ language binding files and examples
python/       - Python language binding files and examples
csharp/       - C# language binding files and examples
net/          - .NET language binding files
vb.net/       - Visual Basic .NET language binding files and examples
vb6/          - Visual Basic 6 language binding files and examples

The vb.net directory is only available in the 32-bit and 64-bit
Windows packages and the vb6 directory is only available in the
32-bit Windows package.

See the EXAMPLES.txt and the README.txt in each language subdirectory
for details on the included source code that demonstrates the usage of
the API.


USB Driver
----------
Under Windows, ensure the device drivers have been installed before
plugging in any devices or running software.  Refer to the Beagle
datasheet for more details on the Windows USB driver.

Under Linux, the libusb library must be installed since the Beagle
software library is dynamically linked to libusb.

Under Mac OS X, a specific kernel driver is not required, but it is
recommended to install the latest OS X updates.  The Mac OS X binary
supports Intel versions of Mac OS X 10.4 Tiger or higher.


Features
--------
For explanation and demonstration of the API features and constructs,
please refer to the Beagle datasheet and the example source code available
on the Total Phase website.


C/C++ bindings
--------------
1) Create a new C/C++ project or open an existing project
2) Add beagle.c and beagle.h to the project
3) Place beagle.dll (or beagle.so for Linux/Darwin) in the PATH
4) Place #include "beagle.h" in any module that uses the API
5) Develop, compile, and run your project


Python bindings
---------------
If not already installed, download Python from:
http://www.python.org/2.5/

1) Copy beagle_py.py to a new folder
2) Copy beagle.dll (or beagle.so for Linux/Darwin) to the same folder
3) Create a new script (i.e. program.py)
4) Put the following line in your script file:
   from beagle_py import *
5) Develop and run your script

There are two main difference between the Beagle API documented in the
datasheet and the Beagle Python API.

1) The "array" type is used for passing data to and from the Beagle
Python API.  See the Python documentation for more information on this
type.

2) Beagle Python API functions can return multiple arguments on
the left hand side.  This obviates the need to pass pointers to
output arguments on the right hand side of API functions.

3) There are a variety of ways one can call API functions that have
array arguments.

  All arrays can be passed into the API as an ArrayType object or as
  a tuple (array, length), where array is an ArrayType object and
  length is an integer.  The user-specified length would then serve
  as the length argument to the API funtion (please refer to the
  product datasheet).  If only the array is provided, the array's
  intrinsic length is used as the argument to the underlying API
  function.

  The ability to pass in a pre-allocated array along with a separate
  length allows the performance-minded programmer to use a single
  array for repeated calls to the API, simply changing the contents
  of the array and/or the specified length as needed.

  Additionally, for arrays that are filled by the API function, an
  integer can be passed in place of the array argument and the API
  will automatically create an array of that length.  All output
  arrays, whether passed in or generated, are passed back in the
  returned tuple.

The calling conventions for each Beagle Python API function is
documented in the comments of beagle_py.py.


C# bindings
-----------
1) Create a new C/C++ project or open an existing project
2) Add beagle.cs to the project
3) For Windows, place beagle.dll in the PATH;
   For Linux rename beagle.so to libbeagle.so and place in the 
   LD_LIBRARY_PATH
4) Develop, compile, and run your project

Every API function that accepts an array argument must be accompanied
by a separate length field, even though the array itself intrinsically
has a length.  See the discussion of the Python API above explaining
the rationale for such an interface.

For C#, structures that contain arrays do not have the length field in
the structure as documented in the datasheet.  Instead, the intrinsic
length of the array is used when the structure is an argument to an
API function.

In cases where the API function ignores the structure argument, a dummy 
structure should be used instead of null since all versions of the 
language do not support null.


.NET and VB.NET bindings
------------------------
Copy beagle.dll and beagle_net.dll to your application development
environment.  The beagle.dll contains the APIs themselves and the
beagle_net.dll provides the .NET interface to the APIs.  For
detailed documentation of APIs refer to the datasheet and the comments
in the C# (beagle.cs) binding.

Like for C#, every API function that accepts an array argument must
be accompanied by a separate length field.  Also as in C#, arrays in
structures use their intrinsic length instead of having a separate
length field in the structure, and dummy structures should be used
instead of passing null when the API is expected to ignore the
structure argument (see above).

Due to the use of unsigned arguments, the .NET bindings are no longer
fully Common Language Specification (CLS) compliant.  As a result, for
example, Microsoft .NET 2.0 is required for any VB.NET applications
using the bindings.


Visual Basic 6
--------------
Copy beagle.dll to your application development environment and
integrate the beagle.bas interface layer into your application.
For detailed documentation of the APIs refer to the datasheet
and the comments in the beagle.bas file.

Like for C#, .NET, and VB.NET, every API function that accepts an
array argument must be accompanied by a separate length field.  Also
as in C#, .NET and VB.NET, arrays in structures use their intrinsic
length instead of having a separate length field in the structure,
and dummy structures should be used instead of passing null when
the API is expected to ignore the structure argument (see above).


Open Source
-----------
When running on Windows and Mac OS X, this software uses a modified
version of the libusb-0.1.12 open source library.  The library is
licensed under LGPL v2.1, the full text of which can be found here:
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html

To link against a custom version of the library, set the LIBUSBTP
environment variable to the full path of the custom library.  The
software will then link against the specified custom library.
