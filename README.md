# Basics of Embedded Systems Labs for IAS0230
In this repository you will find all of the labs that are required for lab completion during the **IAS0230** course at TalTech.  

## Hardware and Tools Available in Class
* **EK-TM4C123GXL**
* **Hardware box (resistors, switches, LEDs, OLED screen, potentiometer, buzzer)**
* **Multimeter**
* **Saleae Logic Analyzer** (8 channel 24MHz)

--- 

## Getting Started
### Windows Subsystem for Linux (WSL2)
You can use a full Linux environment natively on Windows 10/11 without a traditional virtual machine if desired.

#### Install WSL (Ubuntu default)  
In **PowerShell (Admin)**, run:
```powershell
wsl --install
```

*Restart your PC after installation finishes.*

#### Install VS Code and WSL Extension (*ms-vscode-remote.remote-wsl*) on Windows.

#### Install USB Passthrough (*usbipd-win*)
In **PowerShell (Admin)**, run:
```powershell
winget install --interactive --exact dorssel.usbipd-win
```

#### Connecting Hardware to WSL
* Plug in the board via USB and run this in **PowerShell (Admin)** on Windows:
  ```powershell
  usbipd bind -i 1cbe:00fd # Only required once per unique board
  ```

After **cloning the repository**, it is possible to use provided scripts:
* Open your **WSL Linux terminal** and run this **only once per WSL boot**: 
  ```bash
  ./wsl2_usb/prepare.sh
  ```

* Run this **every time you plug in the board**:
  ```bash
  ./wsl2_usb/attach.sh
  ```

Now your hardware is accessible inside WSL.

### Installation
1. Open [TalTech BESLabs GitLab](https://gitlab.cs.ttu.ee/ajasts/beslabs), press *Fork*.

2. Name the project using kebab-case (e.g. ias0230-labs), set *Visibility level* to **Private** and *Select a namespace* must be your UNI-ID username. After in *Manage -> Members -> Invite members* invite *ajasts* as a *Reporter*.

> [!WARNING]
> Ensure *Git* is installed on your system before proceeding (*Skip this step if using pre-configured classroom computers*). If not:
> * **Linux**:
>   * Debian/Ubuntu: In terminal run `sudo apt update && sudo apt install -y git`
>   * Fedora/RHEL: In terminal run `sudo dnf install -y git`
>   * Arch: In terminal run `sudo pacman -Syu --needed --noconfirm git`
> * **macOS**: In terminal run `xcode-select --install`
> * **Windows 10/11 (Native)**: In PowerShell terminal run `winget install --id Git.Git -e --accept-source-agreements --accept-package-agreements -s winget`

3. Clone the BESLabs repository and change into the cloned directory:
   ```bash
   git clone https://gitlab.cs.taltech.ee/<YOUR_UNI_ID_USERNAME>/<YOUR_PROJECT_NAME>.git
   cd beslabs
   ```

> [!IMPORTANT]
> In order not to enter the UNI-ID username and password everytime you push, use `git config credential.helper 'cache --timeout=10800'`. When you enter credentials next time, git will save them for next 3 hours.

4. Run the setup script for your OS which takes ~3 minutes (*Skip this step if using pre-configured classroom computers*):
   * **Linux / WSL**  
   In terminal, run: 
   ```bash
   ./setup/linux.sh
   ```

> [!IMPORTANT]
> Linux users must install [VS Code](https://code.visualstudio.com/docs/setup/linux) manually if it is not already installed.

   * **macOS**  
   In terminal, run: 
   ```bash
   ./setup/mac.command
   ```

> [!IMPORTANT]
> macOS users must install [VS Code](https://code.visualstudio.com/Download) manually. Then install required extensions: *'marus25.cortex-debug'*, *'llvm-vs-code-extensions.vscode-clangd'*.

   * **Windows 10/11 (Native)**  
   In **PowerShell (Admin)**, run:
   ```powershell
   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser -Force
   .\setup\win.ps1
   ```

5. Close and reopen the terminal for all environment variables (*PATH*) to take effect.

### Structure
```text
.
├── common/      # Shared source files
├── lab*/        # Bare-metal microcontroller labs
│   └── test/    # Pre-compiled host OS object files
├── oled/        # Driver for the Adafruit 128x64 OLED
├── setup/       # Setup scripts for Linux, macOS and Win10/11
├── ti/          # TivaWare include and source files*
├── unity/       # Unit testing library
└── wsl2_usb/    # WSL2 USB attach and prepare scripts
```

**Include files are taken from [TivaWare for C Series Software](https://www.ti.com/tool/download/SW-TM4C/2.1.4.178).*

### Lab Contents
1. Simple I/O
2. Functions in C (**No grader**)
3. Port init, delay, LED control
4. Loop sequence, subroutines
5. Breadboard circuit building
6. µC debugging
7. Interrupts and buzzer (**No unit tests**)
8. FSM (**No unit tests**)
9. UART, OLED display (**No grader**)
10. DAC (**Unit tests as grader**)
11. ADC, slide potentiometer (**Unit tests as grader**)
12. Low power mode (**No unit tests**)
13. Final game project (**No unit tests or grader**)

### Usage
#### Building Code
Navigate to the specific lab directory (e.g., lab1/ or rtos/rtos1/) and run:
```bash
make
```  

#### Running Unit Tests
To build and run the unit tests:
```bash
make test
```

#### Cleaning Build Files
To remove compiled object files and executables:
```bash
make clean
``` 

#### Flashing to Target Board
To flash the executable onto the board, run the flash script inside a lab directory:

* **Linux / macOS / WSL**:
  ```bash
  ../flash.sh
  ```

* **Windows (PowerShell)**:
  ```powershell
  ..\flash.ps1
  ```

#### Launching Serial Terminal
Launch a new terminal inside the lab directory to monitor serial output:

* **Linux / macOS / WSL**:
  ```bash
  ../monitor.sh # Press 'Ctrl + t', and then 'q' to close
  ```

* **Windows (PowerShell)**:
  ```powershell
  ..\monitor.ps1 # Press 'Ctrl + c' to close
  ```

> [!NOTE]
> Saving any source file automatically applies code formatting via *.clang-format*. Learn more about formatting options in the [Clang documentation.](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)

### Debugging
1. Connect target board to PC via USB.
2. Open the **main.c** file from the lab.
3. Navigate to the **Run and Debug tab**.
4. Select matching board configuration (either **MSP-EXP432P401R** or **EK-TM4C123GXL**).
5. Press **F5** to start debugging.

## Source Control & Submission Workflow

### Useful Git Commands

#### Show Changes Staged/Not Staged for Commit
```bash
git status
```

#### Show History of Commits
```bash
git log # Press 'q' to exit
```

#### Show What Changes Were Done in the Commit
```bash
git show [HEAD~X] # X - How many commits ago, press 'q' to exit
```

#### Stage Specific Files for Commit
```bash
git add <FILES_TO_ADD_TO_COMMIT>
```

#### Stage All Unstaged Files for Commit
```bash
git add .
```

#### Commit Staged Changes
Follow the [Conventional Commits format:](https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13) `<type>: <description>`.  
- **type**: Either feat (new feature/lab implementation) or fix (bug fix/correction). 
- **description**: Imperative present case, no period, no capital letter start.  

Example: `feat: add LED initialization`

```bash
git commit -m "<type>: <description>"
```

#### Push Changes to Your Remote Repository
```bash
git push
```

#### Sync Updates From Course Repository
In your forked repository in [TalTech GitLab](https://gitlab.cs.ttu.ee/), click *'Update fork'* when your repository is behind upstream. Then, in the repository directory terminal type:
```bash
git pull
```

> [!NOTE]
> Make sure you commited any local changes before running `git pull`.

## Lab assistance contact
Anton Jaštšuk: ajasts@taltech.ee  
Uljana Reinsalu: uljana.reinsalu@taltech.ee

## Acknowledgements
* Anton Jaštšuk - Grader, unit tests development, lab contents upgrade
* Uljana Reinsalu - Ideas and mentor
* Nazrul Nazeer - Tools suggestions and interrupt handler help
* Tatsuki Ishikawa - OLED display driver help
* Jonathan Valvano - Material and labs
* Daniel Valvano - Material and labs
* Ramesh Yerraballi - Material and labs
* Bruno4l - [OLED driver](https://github.com/Bruno4l/SSD1306-EK-TM4C123GXL)

![EU twin logo](images/Co-funded-by-the-European-Union-twin-logos.png)
![Inseneriakadeemia logo](images/Inseneriakadeemia-logo.png)
