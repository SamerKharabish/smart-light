# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [UNRELEASED] - YYYY-MM-DD

### Added

### Changed

### Removed

### Fixed

### Known Issues

## [0.2.1] - 2025-10-10

### Added

### Changed

### Removed

### Fixed

- Updated Readme and fixed wrong sequence bug

### Known Issues

## [0.2.0] - 2025-09-25

### Added

- A led_builtin component to indicate statuses with the built-in LED.

### Changed

### Removed

### Fixed

### Known Issues

- Changing or adding a new environment for an example in the platformio.ini file may break
  PlatformIOs UI. The buttons in the bottom status bar trigger the notification *Cannot read
  properties of undefined (reading ‘id’)* when clicked. The firmware of the selected environment
  can neither be built, uploaded and "cleaning" the environment does also not work. Known
  solutions: either using directly *pio* commands or reloading the Window with ```Shift``` +
  ```Strg``` + ```P``` and selecting *>Developer: Reload Window*.

## [0.1.0] - 2025-09-23

### Added

### Changed

### Removed

### Fixed

### Known Issues
