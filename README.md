# Firmware Updater

Copyright (C) 2022, Jerry Belaston

<p align="center">
  <img width="200" height="200" src="https://github.com/jerry-belaston/firmwareUpdater/blob/master/res/upload-hover.png" />
</p>

## 💡 Overview

Firmware Updater is a C++ Qt application simulating firmware update process of mock devices (xml files) based on JSON information coming from a mock server. 

This application has been implemented following [MVPMV architecture](https://docs.microsoft.com/en-us/archive/msdn-magazine/2011/december/mvpvm-design-pattern-the-model-view-presenter-viewmodel-design-pattern-for-wpf) approach. It demonstrates by the way the responsiveness of the Qt GUI regarding thread notifications.

This application offers two modes:
- Single: one device at a time can be updated.
- Multi: several devices can be simultaneously updated.

Note that default JSON templates and XML devices files are located close to the application executable (into into firmwareUpdater.app/Contents/MacOS on MacOS)

Let you discover! Have fun :wink:


## 🔑 License

Licensed under the MIT license. Read the [LICENSE](LICENSE) file for details.

## ▶️ Precompiled binaries

Precompiled binaries can be found [here](https://github.com/jerry-belaston/firmwareUpdater/releases).

## ⚙️ Build and Installation

Follow the ["Quick user build and installation"](doc/installation.md) guide to bootstrap the firmwareUpdater environment.

## 🔍 Root folders definition

- `doc` as documentation, contains build and technical implementation info
- `ext` as external, contains external libraries/dependencies
- `res` as resources, contains application resources (images, samples, etc.)
- `src` as source, contains source code
