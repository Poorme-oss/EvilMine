# evildragon

[![Github All Releases](https://img.shields.io/github/downloads/evildragon/evildragon/total.svg)](https://github.com/evildragon/evildragon/releases)
[![GitHub release](https://img.shields.io/github/release/evildragon/evildragon/all.svg)](https://github.com/evildragon/evildragon/releases)
[![GitHub Release Date](https://img.shields.io/github/release-date/evildragon/evildragon.svg)](https://github.com/evildragon/evildragon/releases)
[![GitHub license](https://img.shields.io/github/license/evildragon/evildragon.svg)](https://github.com/evildragon/evildragon/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/evildragon/evildragon.svg)](https://github.com/evildragon/evildragon/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/evildragon/evildragon.svg)](https://github.com/evildragon/evildragon/network)

evildragon is a high performance, open source, cross platform RandomX, KawPow, CryptoNight and AstroBWT unified CPU/GPU miner. Official binaries are available for Windows, Linux, macOS and FreeBSD.

## Mining backends
- **CPU** (x64/ARMv8)
- **OpenCL** for AMD GPUs.
- **CUDA** for NVIDIA GPUs via external [CUDA plugin](https://github.com/evildragon/evildragon-cuda).

## Download
* **[Binary releases](https://github.com/evildragon/evildragon/releases)**
* **[Build from source](https://evildragon.com/docs/miner/build)**

## Usage
The preferred way to configure the miner is the [JSON config file](src/config.json) as it is more flexible and human friendly. The [command line interface](https://evildragon.com/docs/miner/command-line-options) does not cover all features, such as mining profiles for different algorithms. Important options can be changed during runtime without miner restart by editing the config file or executing API calls.

* **[Wizard](https://evildragon.com/wizard)** helps you create initial configuration for the miner.
* **[Workers](http://workers.evildragon.info)** helps manage your miners via HTTP API.

## Donations
* Default donation 1% (1 minute in 100 minutes) can be increased via option `donate-level` or disabled in source code.
* XMR: `48edfHu7V9Z84YzzMa6fUueoELZ9ZRXq9VetWzYGzKt52XU5xvqgzYnDK9URnRoJMk1j8nLwEVsaSWJ4fhdUyZijBGUicoD`

## Developers
* **[evildragon](https://github.com/evildragon)**
* **[sech1](https://github.com/SChernykh)**

## Contacts
* support@evildragon.com
* [reddit](https://www.reddit.com/user/evildragon/)
* [twitter](https://twitter.com/evildragon_dev)
