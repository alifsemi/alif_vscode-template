# VSCode Getting Started Template
A simple CMSIS-Pack based example to setup and test VS Code development environment for Alif Ensemble kits.

- The solution consists of following projects:
  - **blinky** is a bare bone LED blinker
  - **hello** demonstrates retargeting printf() to UART
  - **hello_rtt** demonstrates retargeting printf() to SEGGER RTT

## Compiler support
Arm GNU toolchain is used as a default.

In order to build with Arm LLVM edit the `alif.csolution.yml`
```
  # Select used compiler
  compiler: CLANG
```
  - Toolchains are downloaded by Arm Environment Manager (see `vcpkg-configuration.json`)
  - You can find the compiler specific settings in `cdefault.yaml`
  - To download armclang you can add "arm:compilers/arm/armclang": "^6.22.0" to the "requires" object.

## Quick start
First clone the template project repository
```
git clone https://github.com/alifsemi/alif_vscode-template.git

```

To build the template for a supported board other than the DevKit, you have to update the `board.h` file to pick the right variant of the board.
By default the template will build for gen2 DevKit.

The required software setup consists of *VS Code*, *Git*, *CMake*, *Ninja build system*, *cmsis-toolbox*, *Arm GNU toolchain* and *Alif SE tools*.
By default the template project uses J-link so *J-link software* is required for debugging.
In addition to build tools the VS Code extensions and CMSIS packs will be downloaded automatically during the process.

To make environment setup easier this project uses *Arm Environment Manager* for downloading and configuring most of the tool dependencies.
Basically only VS Code, Alif SE tools and J-Link software need to be downloaded and installed manually.

**NOTE:** Please see the Readme_Arm_Environment_Manager.txt file first if you would like to manage where the tools are installed.

Opening the project folder with VS Code automatically suggests installing the extensions needed by this project:
- Arm Environment Manager
- Arm CMSIS csolution
- Cortex-Debug
- Microsoft C/C++ Extension Pack

After setting up the environment you can just click the CMSIS icon and then the *context* and *build* icon to get going.

For Alif SE tools and J-link debugging support add the following entries to VS Code user settings.json (Press F1 and start typing 'User')
```
{
    "alif.setools.root" : "C:/alif-se-tools/app-release-exec",
    "cortex-debug.JLinkGDBServerPath": "C:/Program Files/SEGGER/JLink/JLinkGDBServerCL.exe"
}
```

## More detailed getting started guide
Please refer to the [Getting started guide](doc/getting_started.md)
