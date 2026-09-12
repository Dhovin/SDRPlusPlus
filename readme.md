# SDR++, The bloat-free SDR software<br>

![Screenshot](https://i.imgur.com/Ter2MQJ.png)
SDR++ is a cross-platform and open source SDR software with the aim of being bloat free and simple to use.

![Build](https://github.com/AlexandreRouma/SDRPlusPlus/workflows/Build%20Binaries/badge.svg)

* [Patreon](https://patreon.com/ryzerth)
* [Discord Server](https://discord.gg/aFgWjyD)
* [Reddit](https://www.reddit.com/r/sdrpp/)
* IRC: `#sdrpp` ([libera.chat](https://libera.chat)) __**NO LONGER ACTIVE, JOIN DISCORD INSTEAD**__

## Features

* Multi VFO
* Wide hardware support (both through SoapySDR and dedicated modules)
* SIMD accelerated DSP
* Cross-platform (Windows, Linux, MacOS and BSD)
* Full waterfall update when possible. Makes browsing signals easier and more pleasant
* Modular design (easily write your own plugins)

# Installing

## Nightly Builds

Nightly builds contain the very latest features and bugfixes. They are usually just stable as [normal releases](https://github.com/AlexandreRouma/SDRPlusPlus/releases) but are available basically minutes to hours after a change has been pushed to the code.

You can download them [here](https://www.sdrpp.org/nightly). It'll redirect you to the latest nightly on GitHub, scroll down to "Artifacts" and click on the version for your OS.

GitHub currently requires an account for the files to be downloadable so make sure you are logged in.

## Windows

Download the latest release from [the Releases page](https://github.com/AlexandreRouma/SDRPlusPlus/releases) and extract to the directory of your choice.

To create a desktop shortcut, rightclick the exe and select `Send to -> Desktop (create shortcut)`, then, rename the shortcut on the desktop to whatever you want.

## Linux

### Debian-based (Ubuntu, Mint, etc)

**Warning: DO NOT** use the `sdrpp` package from the package manager. The package is incompete and incompatible with all official and out-of-tree modules.

Download the latest release from [the Releases page](https://github.com/AlexandreRouma/SDRPlusPlus/releases) and extract to the directory of your choice.

Then, use apt to install it:

```sh
sudo apt install path/to/the/sdrpp_debian_amd64.deb
```

**IMPORTANT: You must install the drivers for your SDR. Follow instructions from your manufacturer as to how to do this on your particular distro.**

### Arch-based

Install from source following the instructions below.

**WARNING: The sdrpp-git AUR package is no longer official, it is not recommended to use it.**

### Other

There are currently no existing packages for other distributions, for these systems you'll have to [build from source](https://github.com/AlexandreRouma/SDRPlusPlus#building-on-linux--bsd).

## MacOS

Download the app bundle from the latest [nightly build](https://www.sdrpp.org/nightly)

## BSD

There are currently no BSD packages, refer to [Building on Linux / BSD](https://github.com/AlexandreRouma/SDRPlusPlus#building-on-linux--bsd) for instructions on building from source.

# Building on Windows

The recommended build environment is **Visual Studio 2022** (MSVC v143 toolset) or **VS Code** configured with the CMake Tools extension.

## Install dependencies

* [CMake](https://cmake.org) (3.16 or newer)
* [vcpkg](https://vcpkg.io)
* [PothosSDR](https://github.com/pothosware/PothosSDR) or individual SDR vendor drivers (Install PothosSDR in `C:/Program Files/PothosSDR` if using pre-packaged SDR drivers)
* [RtAudio](https://www.music.mcgill.ca/~gary/rtaudio/) (Install in `C:/Program Files (x86)/RtAudio/` or build via CMake)

Next, install required libraries via vcpkg (64-bit):

```powershell
.\vcpkg.exe install fftw3:x64-windows glfw3:x64-windows zstd:x64-windows libusb:x64-windows
```

## Building using the command line

Replace `<vcpkg install directory>` with the absolute path to your vcpkg installation:

```bat
cmake -B build -S . "-DCMAKE_TOOLCHAIN_FILE=<vcpkg install directory>/scripts/buildsystems/vcpkg.cmake" -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release -j
```

## Running for development

### Create a new configuration root directory

```bat
./create_root.bat
```

This will create the `root_dev` directory that will be used to save the configs of sdrpp and the modules.

You will next need to edit the `root_dev/config.json` file to point to the modules that were built. If the file is missing in your folder run the application once and it will create one with default value -- see later on how to run the application.

### Run SDR++ from the command line

From the top directory, you can simply run:

```bat
./build/Release/sdrpp.exe -r root_dev -c
```

Or, if you wish to run from the build directory e.g. `build/Release` and adapt the relative path to the `root_dev` folder:

```bat
./sdrpp.exe -r ../../root_dev -c
```

The optional `-c` argument is for keeping the console active in order to see the error messages.

Because all the paths are relative, for the rest of the command line instructions we are going to assume you are running from the top directory using the former command.
As mentioned previously you need to edit `root_dev/config.json` to add the modules that were built. From the default configuration file you need to add the paths in the `modules` section. Add to this list all the modules you wish to use.

```json
...
"modules": [
    "./build/radio/Release/radio.dll",
    "./build/recorder/Release/recorder.dll",
    "./build/rtl_tcp_source/Release/rtl_tcp_source.dll",
    "./build/audio_sink/Release/audio_sink.dll"
]
...
```

You also need to change the location of the resource and module directories, for development, I recommend:

```json
...
"modulesDirectory": "root_dev/modules",
...
"resourcesDirectory": "root_dev/res",
...
```

Remember that these paths will be relative to the run directory.

## Installing SDR++

If you choose to run SDR++ for development, you do not need this step.
First, copy over the exe and DLLs from `build/Release/` to `root_dev`.

Next you need to copy over all the modules that were compiled. To do so, copy the DLL file of the module (located in its build folder given below) to the `root_dev/modules` directory and other DLLs (that do not have the exact name of the module) to the `root_dev` directory.

The modules built will be some of the following (Repeat the instructions above for all you wish to use):

* `build/radio/Release/`
* `build/recorder/Release/`
* `build/rtl_tcp_source/Release/`
* `build/spyserver_source/Release/`
* `build/airspyhf_source/Release/`
* `build/plutosdr_source/Release/`
* `build/audio_sink/Release/`

# Building on Linux / BSD

## Select which modules you wish to build

Depending on which module you want to build, you will need to install some additional dependencies.
Please refer to the module list table further down in this readme for the names, dependencies and build options of each module.

The build options are then passed to the cmake command as such `cmake .. -DOPTION_NAME_HERE=ON -DANOTHER_OPTION_HERE=OFF` etc...

## Install dependencies

### Debian / Ubuntu (Ubuntu 22.04 / 24.04 LTS, Debian 12 Bookworm, Debian 13 Trixie)

```sh
sudo apt update
sudo apt install build-essential cmake git libfftw3-dev libglfw3-dev libvolk2-dev libzstd-dev librtaudio-dev \
    libairspy-dev libairspyhf-dev libhackrf-dev librtlsdr-dev libiio-dev libad9361-dev
```
*(Note: on newer distributions, `libvolk2-dev` is provided as `libvolk-dev` or `libvolk3-dev`)*

### Arch Linux / Manjaro

```sh
sudo pacman -S base-devel cmake git fftw glfw-x11 volk zstd rtaudio
```

### Fedora

```sh
sudo dnf install @development-tools cmake git fftw-devel glfw-devel volk-devel libzstd-devel rtaudio-devel
```

## Building

```sh
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Create a new root directory

```sh
sh ./create_root.sh
```

## Running for development

If you wish to install SDR++, skip to the next step.

First run SDR++ from the build directory to generate a default config file:

```sh
./build/sdrpp -r root_dev/
```

Then, you will need to edit the `root_dev/config.json` file to point to the modules that were built. Here is an example of what it should look like:

```json
...
"modules": [
    "./build/decoder_modules/radio/radio.so",
    "./build/misc_modules/recorder/recorder.so",
    "./build/source_modules/rtl_tcp_source/rtl_tcp_source.so",
    "./build/sink_modules/audio_sink/audio_sink.so"
]
...
```

You also need to change the location of the resource and module directories for development:

```json
...
"modulesDirectory": "./root_dev/modules",
...
"resourcesDirectory": "./root_dev/res",
...
```

Remember that these paths will be relative to the run directory.

Next, from the top directory, you can simply run:

```sh
./build/sdrpp -r root_dev
```

## Installing SDR++

To install SDR++ system-wide, run:

```sh
sudo cmake --install build
```

# Building on MacOS

SDR++ supports modern macOS versions on both Apple Silicon (ARM64) and Intel (x86_64).

## Install dependencies

Install dependencies using [Homebrew](https://brew.sh/):

```sh
brew install cmake fftw glfw volk zstd portaudio libusb airspy airspyhf hackrf rtl-sdr libbladerf codec2
```

## Build

Enable PortAudio sinks, disable RtAudio (which has issues on macOS), and enable macOS bundle defaults:

```sh
cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DOPT_BUILD_AUDIO_SINK=OFF \
    -DOPT_BUILD_PORTAUDIO_SINK=ON \
    -DOPT_BUILD_NEW_PORTAUDIO_SINK=ON \
    -DOPT_BUILD_BLADERF_SOURCE=ON \
    -DOPT_BUILD_M17_DECODER=ON \
    -DUSE_BUNDLE_DEFAULTS=ON

cmake --build build -j$(sysctl -n hw.ncpu)
```

## Create bundle and install

From the repository root:

```sh
sh make_macos_bundle.sh ./build ./SDR++.app
```

This will produce the `SDR++.app` bundle, which you can drag into `/Applications`.

# Module List

Not all modules are built by default. I decided to disable the build of those with large libraries, libraries that can't be installed through the package manager (or pothos) and those that are still in beta.
Modules in beta are still included in releases for the most part but not enabled in SDR++ (need to be instantiated).

## Sources

| Name                 | Stage      | Dependencies      | Option                         | Built by default| Built in Release        | Enabled in SDR++ by default |
|----------------------|------------|-------------------|--------------------------------|:---------------:|:-----------------------:|:---------------------------:|
| airspy_source        | Working    | libairspy         | OPT_BUILD_AIRSPY_SOURCE        | ✅              | ✅                     | ✅                         |
| airspyhf_source      | Working    | libairspyhf       | OPT_BUILD_AIRSPYHF_SOURCE      | ✅              | ✅                     | ✅                         |
| audio_source         | Working    | rtaudio           | OPT_BUILD_AUDIO_SOURCE         | ✅              | ✅                     | ✅                         |
| bladerf_source       | Working    | libbladeRF        | OPT_BUILD_BLADERF_SOURCE       | ⛔              | ✅                     | ✅                         |
| dragonlabs_source    | Working    | libdlcr           | OPT_BUILD_DRAGONLABS_SOURCE    | ⛔              | ⛔                     | ✅                         |
| file_source          | Working    | -                 | OPT_BUILD_FILE_SOURCE          | ✅              | ✅                     | ✅                         |
| fobossdr_source      | Working    | libfobos          | OPT_BUILD_FOBOSSDR_SOURCE      | ⛔              | ✅                     | ✅                         |
| hackrf_source        | Working    | libhackrf         | OPT_BUILD_HACKRF_SOURCE        | ✅              | ✅                     | ✅                         |
| harogic_source       | Beta       | htra_api          | OPT_BUILD_HAROGIC_SOURCE       | ⛔              | ⛔                     | ✅                         |
| hermes_source        | Beta       | -                 | OPT_BUILD_HERMES_SOURCE        | ✅              | ✅                     | ✅                         |
| hydrasdr_source      | Working    | libhydrasdr       | OPT_BUILD_HYDRASDR_SOURCE      | ⛔              | ✅                     | ✅                         |
| kcsdr_source         | Unfinished | libkcsdr          | OPT_BUILD_KCSDR_SOURCE         | ⛔              | ⛔                     | ⛔                         |
| limesdr_source       | Working    | liblimesuite      | OPT_BUILD_LIMESDR_SOURCE       | ⛔              | ✅                     | ✅                         |
| network_source       | Beta       | -                 | OPT_BUILD_NETWORK_SOURCE       | ✅              | ✅                     | ✅                         |
| perseus_source       | Beta       | libperseus-sdr    | OPT_BUILD_PERSEUS_SOURCE       | ⛔              | ✅                     | ✅                         |
| plutosdr_source      | Working    | libiio, libad9361 | OPT_BUILD_PLUTOSDR_SOURCE      | ✅              | ✅                     | ✅                         |
| rfnm_source          | Beta       | librfnm           | OPT_BUILD_RFNM_SOURCE          | ⛔              | ✅                     | ✅                         |
| rfspace_source       | Working    | -                 | OPT_BUILD_RFSPACE_SOURCE       | ✅              | ✅                     | ✅                         |
| rtl_sdr_source       | Working    | librtlsdr         | OPT_BUILD_RTL_SDR_SOURCE       | ✅              | ✅                     | ✅                         |
| rtl_tcp_source       | Working    | -                 | OPT_BUILD_RTL_TCP_SOURCE       | ✅              | ✅                     | ✅                         |
| sddc_source          | Beta       | libsddc           | OPT_BUILD_SDDC_SOURCE          | ⛔              | ⛔                     | ⛔                         |
| sdrplay_source       | Working    | SDRplay API       | OPT_BUILD_SDRPLAY_SOURCE       | ⛔              | ✅                     | ✅                         |
| sdrpp_server_source  | Working    | -                 | OPT_BUILD_SDRPP_SERVER_SOURCE  | ✅              | ✅                     | ✅                         |
| soapy_source         | Deprecated | soapysdr          | OPT_BUILD_SOAPY_SOURCE         | ⛔              | ⛔                     | ⛔                         |
| spectran_source      | Unfinished | RTSA Suite        | OPT_BUILD_SPECTRAN_SOURCE      | ⛔              | ⛔                     | ⛔                         |
| spectran_http_source | Beta       | -                 | OPT_BUILD_SPECTRAN_HTTP_SOURCE | ✅              | ✅                     | ✅                         |
| spyserver_source     | Working    | -                 | OPT_BUILD_SPYSERVER_SOURCE     | ✅              | ✅                     | ✅                         |
| usrp_source          | Beta       | libuhd            | OPT_BUILD_USRP_SOURCE          | ⛔              | ⛔                     | ✅                         |

## Sinks

| Name               | Stage      | Dependencies | Option                       | Built by default| Built in Release | Enabled in SDR++ by default |
|--------------------|------------|--------------|------------------------------|:---------------:|:----------------:|:---------------------------:|
| android_audio_sink | Working    | -            | OPT_BUILD_ANDROID_AUDIO_SINK | ⛔              | ✅              | ✅ (Android only)          |
| audio_sink         | Working    | rtaudio      | OPT_BUILD_AUDIO_SINK         | ✅              | ✅              | ✅                         |
| network_sink       | Working    | -            | OPT_BUILD_NETWORK_SINK       | ✅              | ✅              | ✅                         |
| new_portaudio_sink | Working    | portaudio    | OPT_BUILD_NEW_PORTAUDIO_SINK | ⛔              | ✅              | ⛔                         |
| portaudio_sink     | Working    | portaudio    | OPT_BUILD_PORTAUDIO_SINK     | ⛔              | ✅              | ⛔                         |

## Decoders

| Name                | Stage      | Dependencies | Option                        | Built by default| Built in Release | Enabled in SDR++ by default |
|---------------------|------------|--------------|-------------------------------|:---------------:|:----------------:|:---------------------------:|
| atv_decoder         | Unfinished | -            | OPT_BUILD_ATV_DECODER         | ⛔              | ⛔              | ⛔                         |
| dab_decoder         | Unfinished | -            | OPT_BUILD_DAB_DECODER         | ⛔              | ⛔              | ⛔                         |
| falcon9_decoder     | Unfinished | ffplay       | OPT_BUILD_FALCON9_DECODER     | ⛔              | ⛔              | ⛔                         |
| kg_sstv_decoder     | Unfinished | -            | OPT_BUILD_KG_SSTV_DECODER     | ⛔              | ⛔              | ⛔                         |
| m17_decoder         | Working    | -            | OPT_BUILD_M17_DECODER         | ⛔              | ✅              | ⛔                         |
| meteor_demodulator  | Working    | -            | OPT_BUILD_METEOR_DEMODULATOR  | ✅              | ✅              | ⛔                         |
| pager_decoder       | Unfinished | -            | OPT_BUILD_PAGER_DECODER       | ⛔              | ⛔              | ⛔                         |
| radio               | Working    | -            | OPT_BUILD_RADIO               | ✅              | ✅              | ✅                         |
| ryfi_decoder        | Working    | -            | OPT_BUILD_RYFI_DECODER        | ⛔              | ⛔              | ⛔                         |
| vor_receiver        | Unfinished | -            | OPT_BUILD_VOR_RECEIVER        | ⛔              | ⛔              | ⛔                         |
| weather_sat_decoder | Unfinished | -            | OPT_BUILD_WEATHER_SAT_DECODER | ⛔              | ⛔              | ⛔                         |

## Misc

| Name                | Stage      | Dependencies | Option                      | Built by default | Built in Release | Enabled in SDR++ by default |
|---------------------|------------|--------------|-----------------------------|:----------------:|:----------------:|:---------------------------:|
| discord_integration | Working    | -            | OPT_BUILD_DISCORD_PRESENCE  | ✅              | ✅               | ⛔                         |
| frequency_manager   | Working    | -            | OPT_BUILD_FREQUENCY_MANAGER | ✅              | ✅               | ✅                         |
| iq_exporter         | Working    | -            | OPT_BUILD_IQ_EXPORTER       | ✅              | ✅               | ⛔                         |
| recorder            | Working    | -            | OPT_BUILD_RECORDER          | ✅              | ✅               | ✅                         |
| rigctl_client       | Unfinished | -            | OPT_BUILD_RIGCTL_CLIENT     | ✅              | ✅               | ⛔                         |
| rigctl_server       | Working    | -            | OPT_BUILD_RIGCTL_SERVER     | ✅              | ✅               | ✅                         |
| scanner             | Beta       | -            | OPT_BUILD_SCANNER           | ✅              | ✅               | ⛔                         |
| scheduler           | Unfinished | -            | OPT_BUILD_SCHEDULER         | ⛔              | ⛔               | ⛔                         |

# Troubleshooting

First, please make sure you're running the latest automated build. If your issue is linked to a bug it is likely that is has already been fixed in later releases

## SDR++ crashes then it won't start again no matter what

This is a bug in 1.0.0 that was fixed in 1.0.1

In some cases, if a crash happened while the config was being saved, the config file would be corrupted and SDR++ would refuse to start because of it.

This has now been fixed. If a config file is corrupted it'll just reset it to its default state.

## "hash collision" error when starting

You likely installed the `soapysdr-module-all` package on Ubuntu/Debian. If not it's still a SoapySDR bug caused by multiple soapy modules coming in conflict. Uninstall anything related to SoapySDR then install soapysdr itself and only the soapy modules you actually need.

## "I don't see -insert module name here-, what's going on?"

If the module was included in a later update, it's not enabled in the config. The easiest way to fix this is just to delete the `config.json` file and let SDR++ recreate it (you will lose your setting relating to the main UI like VFO colors, zoom level and theme).
The best option however is to edit the config file to add an instance of the module you wish to have enabled (see the Module List).

## SDR++ crashes when stopping a RTL-SDR

This is a bug recently introduced by libusb1.4
To solve, this, simply downgrade to libusb1.3

## SDR++ crashes when starting a HackRF

If you also have the SoapySDR module enabled, this is a bug in libhackrf. It's caused by libhackrf not checking if it's already initialized.
The solution until a fixed libhackrf version is released is to disable the soapy_source module from SDR++. For this, go into the "Module Manager" menu and click the `-` button next to the row with "soapy_source". After that, restart SDR++.

## Issue not listed here?

If you still have an issue, please open an issue about it or ask on the discord. I'll try to respond as quickly as I can. Please avoid trying to contact me on every platform imaginable thinking I'll respond faster though...

# Contributing

Feel free to submit band plans via the GitHub issue tracker.
For code changes, please create a feature request instead.

# Credits

## Patrons

* Bob Logan
* [Christian Häusler](https://github.com/corvus-ch)
* Croccydile
* Dale L Puckett (K0HYD)
* [Daniele D'Agnelli](https://linkedin.com/in/dagnelli)
* [David Taylor (GM8ARV)](https://twitter.com/gm8arv)
* D. Jones
* Dexruus
* [EB3FRN](https://www.eb3frn.net/)
* Eric Johnson
* Ernest Murphy (NH7L)
* Flinger Films
* [Frank Werner (HB9FXQ)](https://twitter.com/HB9FXQ)
* gringogrigio
* Jandro
* Jeff Moe
* Joe Cupano
* KD1SQ
* Kezza
* Krys Kamieniecki
* Lee Donaghy
* Lee (KD1SQ)
* .lozenge. (Hank Hill)
* Martin Herren (HB9FXX)
* NeoVilsonWong
* Nitin (VU2JEK)
* ON4MU
* [Passion-Radio.com](https://passion-radio.com/)
* Paul Maine
* Peter Betz
* [Scanner School](https://scannerschool.com/)
* Scott Palmer
* [SignalsEverywhere](https://signalseverywhere.com/)
* Syne Ardwin (WI9SYN)
* [W4IPA](https://twitter.com/W4IPAstroke5)
* William Arcand (W1WRA)
* William Pitchford
* [Yves Rougy](https://www.twitch.tv/yorzian)
* [Zipper](https://github.com/reppiZ)

## Contributors

* [Aang23](https://github.com/Aang23)
* [Alexsey Shestacov](https://github.com/wingrime)
* [Aosync](https://github.com/aosync)
* [Benjamin Kyd](https://github.com/benkyd)
* [Benjamin Vernoux](https://github.com/bvernoux)
* [Cropinghigh](https://github.com/cropinghigh)
* [Fred F4EED](http://f4eed.wordpress.com/)
* [Howard0su](https://github.com/howard0su)
* John Donkersley
* [Joshua Kimsey](https://github.com/JoshuaKimsey)
* [Manawyrm](https://github.com/Manawyrm)
* [Martin Hauke](https://github.com/mnhauke)
* [Marvin Sinister](https://github.com/marvin-sinister)
* [Maxime Biette](https://github.com/mbiette)
* [Paulo Matias](https://github.com/thotypous)
* [Raov](https://twitter.com/raov_birbtog)
* [Cam K.](https://github.com/Starman0620)
* [Shuyuan Liu](https://github.com/shuyuan-liu)
* [Syne Ardwin (WI9SYN)](https://esaille.me/)
* [Szymon Zakrent](https://github.com/zakrent)
* Youssef Touil
* [Zimm](https://github.com/invader-zimm)


## Libraries used

* [SoapySDR (PothosWare)](https://github.com/pothosware/SoapySDR)
* [Dear ImGui (ocornut)](https://github.com/ocornut/imgui)
* [json (nlohmann)](https://github.com/nlohmann/json)
* [rtaudio](http://www.portaudio.com/)
* [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs)
