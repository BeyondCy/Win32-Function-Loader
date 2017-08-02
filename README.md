# Windows Function Loader Proof-of-Concept

This project illustrates how an application could theoretically load functions from an external library with no function calls i.e. LoadLibrary / GetProcAddress.

## Getting Started

Make a pull request, download the file as a zip file, or copy the code from Main.c and PEB.h. 

Make a new project with any IDE of your choice and include PEB.h and main.c or copy the code from main.c

### Prerequisites

The project is 32-bit based. It must be compiled as 32-bit, not 64-bit. Additionally, you must include PEB.h and compiler options must be set to execute MASM otherwise the assembly instruction
```
asm("Mov %0, Dword Ptr Fs:[0x30] \n" : "=r" (pPeb));
```
will not work.

The following segments can be removed from PEB.h:
```
#define NT int
```
The following segments can be removed from main.c:
```
#include <psapi.h>
#include <winuser.h>
#include <wingdi.h>
```

## Running the tests

Please note that this application is console based. You will need to run the executable from a console (CMD.exe) otherwise the application may terminate on completion. Some versions of Windows ignore the final system("PAUSE") call. Additionally, this code segment contains pauses with each function iteration. If you do not want this remove:

```
system("PAUSE");
```

from line 79

## Built With

* [Dev C++](https://sourceforge.net/projects/orwelldevcpp/)
* [Microsoft Windows API](https://msdn.microsoft.com/en-us/library/aa383723(VS.85).aspx)

## Authors

* **Mathew A. Stefanowich** - *Initial work*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Windows Internals Volume 1.
* The Shellcoders Handbook
* VxHeavens
