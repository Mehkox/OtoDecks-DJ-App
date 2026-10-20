# OtoDecks

**A two-deck DJ mixing application built in C++ with the JUCE framework.**

<img width="1593" height="681" alt="Otodecks DJ UI" src="https://github.com/user-attachments/assets/57c4bce0-2a0a-479b-a3bb-1350a155ee39" />

Built on a course starter project (Object Oriented Programming module). The starter provided the basic player, deck and waveform classes. My contributions are listed below.

## Features

- Two independent decks with play, pause, volume, speed, and position controls
- Waveform display with a moving playhead
- Elapsed and total time display with a progress bar
- Drag and drop audio files onto a deck or into the playlist
- Playlist library with track buttons to load a song onto either deck
- Live search to filter the playlist
- Playlist saved between sessions
- Track durations read from the audio file
- Custom themed interface

## My contributions

- **Playlist library:** `PlaylistComponent` with drag and drop import, track durations, and load to deck buttons
- **Live search:** filters the playlist as you type
- **Persistence:** playlist is saved to the user's app data folder and reloaded on launch
- **Deck interface:** themed layout, elapsed/total time, progress bar, track title
- **Deck integration:** `loadFile()` and title/duration setters so the playlist can control the decks

## OOP concepts used

| Concept | Where |
| --- | --- |
| Inheritance and polymorphism | `DJAudioPlayer` extends `AudioSource`. Components override `paint()` and `resized()` |
| Multiple interfaces | `PlaylistComponent` implements `TableListBoxModel`, `Button::Listener`, `FileDragAndDropTarget`, and `Timer` |
| Composition | `MainComponent` owns two players, two decks, and the playlist |
| Dependency injection | Both decks are passed into `PlaylistComponent` by reference through its constructor |
| Encapsulation | Private members, with public methods like `setGain()` that validate their input |
| Observer pattern | Listeners for buttons, sliders, and waveform change events |
| Resource management | `std::unique_ptr` for audio readers and sources |

## Class overview

| Class | Responsibility |
| --- | --- |
| `MainComponent` | Owns the players, decks, and playlist. Mixes the two decks into one audio output |
| `DJAudioPlayer` | Loads audio, plays it, and controls gain, speed, and position |
| `DeckGUI` | Controls and display for one deck |
| `WaveformDisplay` | Draws the waveform and playhead |
| `PlaylistComponent` | Track library with search, save/load, and load to deck buttons |

## Building

**Requirements**
- [JUCE](https://juce.com/)
- Visual Studio

**Steps**
1. Open `OtoDecks.jucer` in Projucer.
2. In the module settings, set the module paths to where JUCE is installed on your computer.
3. Click "Save and Open in IDE" for Visual Studio.
4. Build and run.

## Author

**Marcus Seet**
BSc (Hons) Computer Science, University of London (via SIM)

[LinkedIn](https://www.linkedin.com/in/marcus-seet-b8946b241) | [GitHub](https://github.com/Mehkox)
