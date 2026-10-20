/*
  ==============================================================================

    PlaylistComponent.h
    Created: 4 Mar 2025 10:32:17pm
    Author:  Marcus Seet

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
public juce::TableListBoxModel,
public juce::Button::Listener,
public FileDragAndDropTarget,
public Timer
{
public:
    PlaylistComponent(DeckGUI& deck1, DeckGUI& deck2, AudioFormatManager& formatManager);
    ~PlaylistComponent() override;
    int getNumRows () override;
    void paintRowBackground (Graphics &,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected) override;
    void paintCell (Graphics &,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected) override;
    void cellClicked (int rowNumber, int columnId, const MouseEvent &) override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    Component* refreshComponentForCell (int rowNumber,
    int columnId,
    bool isRowSelected,
    Component *existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    void timerCallback() override;
private:
    AudioFormatManager& formatManager;
    File playlistSave;
    void savePlaylist();
    void loadPlaylist();
    DeckGUI& deckGUI1;
    DeckGUI& deckGUI2;
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    std::vector<juce::File> trackURLs;
    std::vector<juce::String> trackDurations;
    
    TextEditor searchBar;
    Array<String> filteredNames;
    Array<juce::File> filteredURLs;
    Array<juce::String> filteredDurations;
    void searchList(const String& query);
    bool isSearching;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent);
};
