# Penn State IEEE
## Ethical Hacking Workshop
Occured on February 27, 2024

### About
Participants learned the basics of ethical/white-hat hacking.
Ethical hacking is a way to use hacking strategies and software in order to protect an organizations data and improve their security against malicious attacks.
The workshop went over the basics of ethical white hat hacking versus illegal black hat hacking, as well as learning how to use a few different attack methods commonly used by both, such as password cracking and DOS (Denial of Service) attacks!

### Disclaimer!
Penn State IEEE does not condone the use of these tools for malicious intent. These practices and tools should only be used with permission from the owner of the device.

## Workshop Materials
### Password Cracker
[Windows Download](https://www.openwall.com/john/k/john-1.9.0-jumbo-1-win64.zip)
[MacOS Arm (M1, M2, etc.) Download](https://download.openwall.net/pub/projects/john/contrib/macosx/john-1.8.0.9-jumbo-macosx_sse4.zip)
[MacOS Intel (NOT FOR APPLE SILICON) Download](https://download.openwall.net/pub/projects/john/contrib/macosx/john-1.8.0.9-jumbo-macosx_avx2.zip)

First download John the Ripper and extract the folder
Then download `PasswordProtected.zip` and place it in the John the Ripper folder (`john-1.8.0.9-jumbo-macos_sse4` or similar)

In Command Prompt (Windows) or Terminal (MacOS), use `cd __DIR__` to get into `__JOHN_FOLDER__/run`
Run `zip2john ../PasswordProtected.zip > PasswordProtected.hash`
Run `john ./PasswordProtected.hash`

### DOS Attack
