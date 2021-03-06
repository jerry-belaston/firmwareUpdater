# Changelog
All notable changes to Firmware Updater will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/).

## [Unreleased]

## [2.0.0] - 2022-04-30
### Added
- Refactor the whole application to manage multiple parallel devices update
- Update minimum requirements (Qt 6.2.4 and Mojave 10.14)

## [1.0.2] - 2022-04-22
### Added
- Manage application version from both CMake and QMake (instead of hard coding in source file)
- Application now working under Qt versions 5 and 6
- Image cache in the Template Info view for boosting image url download and rendering

### Fixed
- Compilation warnings/errors, now set to maximum level (including warning as error)

## [1.0.1] - 2022-04-13
### Added
- First MacOS release version

### Fixed
- Search templates file closed to application executable (and no more
  into working directory)

## [1.0.0] - 2022-04-11
### Added
- First public Windows version
