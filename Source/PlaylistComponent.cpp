/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 4 Mar 2025 10:32:17pm
    Author:  Marcus Seet

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI& deck1, DeckGUI& deck2,AudioFormatManager& _formatManager) : formatManager(_formatManager), deckGUI1(deck1),deckGUI2(deck2)
{
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    searchBar.setReturnKeyStartsNewLine(false);
    searchBar.setTextToShowWhenEmpty("Search...", Colours::grey);
    searchBar.onTextChange = [this] {searchList(searchBar.getText());};
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);
    tableComponent.getHeader().addColumn("#", 1, 50);
    tableComponent.getHeader().addColumn("Track title", 2, 400);
    tableComponent.getHeader().addColumn("Duration", 3, 400);
    tableComponent.getHeader().addColumn("Load to Deck 1", 4, 200);tableComponent.getHeader().addColumn("Load to Deck 2", 5, 200);
    tableComponent.setModel(this);
    auto dataDir = File::getSpecialLocation(File::userApplicationDataDirectory)
        .getChildFile("OtoDecks");
    dataDir.createDirectory();
    playlistSave = dataDir.getChildFile("playlist.txt");
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(Colour::fromRGB(41, 41, 43));   // clear the background

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    tableComponent.setColour(TableListBox::backgroundColourId, Colour::fromRGB(41, 41, 43));
    searchBar.setColour(TextEditor::backgroundColourId, Colour::fromRGB(41, 41, 43));
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    searchBar.setBounds(0, 0, getWidth(), 30);
    tableComponent.setBounds(0, 30, getWidth(), getHeight()-30);
}

int PlaylistComponent::getNumRows()
{
    if (isSearching)
        return (int)filteredNames.size();
    return (int)trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if(rowIsSelected)
    {
        g.fillAll(Colour::fromRGB(157, 105, 163));
    }
    else{
        g.fillAll(Colour::fromRGB(201, 199, 186));
    }
}

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if(isSearching == false)
    {
        if(columnId == 1)
        {
            g.drawText(std::to_string(rowNumber + 1), 2, 0, width - 4, height, Justification::centredLeft,
                       true);
        }
        if(columnId == 2)
        {
            g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft,
                       true);
//            DBG("Drawing the name " << trackTitles[rowNumber]);
        }
        if(columnId == 3)
        {
            if(rowNumber < trackDurations.size())
            {
                g.drawText(trackDurations[rowNumber], 2, 0, width-4, height, Justification::centredLeft);
            }
        }
    }
    else
    {
        if(columnId == 1)
        {
            g.drawText(std::to_string(rowNumber + 1), 2, 0, width - 4, height, Justification::centredLeft,
                       true);
        }
        if(columnId == 2)
        {
            g.drawText(filteredNames[rowNumber], 2, 0, width - 4, height, Justification::centredLeft,
                       true);
        }
        if(columnId == 3)
        {
            if(rowNumber < filteredDurations.size())
            {
                g.drawText(filteredDurations[rowNumber], 2, 0, width-4, height, Justification::centredLeft);
            }
        }
    }
        
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent &)
{
    std::cout<<"Row number: " << rowNumber << " " << "Column: " <<columnId <<std::endl;
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    
    if(columnId == 4 || columnId == 5)
    {
        if(existingComponentToUpdate == nullptr)
        {
            
            TextButton* btn = new TextButton("Load");
            existingComponentToUpdate = btn;
            btn->addListener(this);
            btn->setColour(TextButton::buttonColourId, Colour::fromRGB(148, 104, 70));
            btn->setColour(TextButton::buttonOnColourId, Colour::fromRGB(201, 199, 186));
            
            int finalIndex = rowNumber;
            if(isSearching)
            {
                for (int i = 0; i < trackTitles.size(); ++i) {
                    if(trackTitles[i] == filteredNames[rowNumber])
                    {
                        finalIndex = i;
                        break;
                    }
                }
            }
            String rowId{std::to_string(rowNumber)};
            String colId{std::to_string(columnId)};
            btn->setComponentID(rowId + "_" + colId);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button)
{
    String buttonId = button->getComponentID();
    StringArray tokens;
    tokens.addTokens(buttonId, "_", "");
    
    int row = tokens[0].getIntValue();
    int columnId = tokens[1].getIntValue();
    
    if(isSearching)
    {
        for (int i = 0; i < trackURLs.size(); ++i) {
            if(trackURLs[i] == filteredURLs[row])
            {
                row = i;
                break;
            }
        }
    }
    
    
    if(row >= 0 && row < trackURLs.size())
    {
        DBG("PlaylistComponent::buttonClicked "<< trackTitles[row]);
        juce::File fileToPlay = trackURLs[row];
        if(columnId == 4)
        {
            deckGUI1.loadFile(fileToPlay);
            deckGUI1.setTrackTitle(fileToPlay.getFileNameWithoutExtension().toStdString());
        }
        if(columnId == 5)
        {
            deckGUI2.loadFile(fileToPlay);
            deckGUI2.setTrackTitle(fileToPlay.getFileNameWithoutExtension().toStdString());
        }
        
    }
        
    
}

bool PlaylistComponent::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "PlaylistComponent::isInterestedInFileDrag" << std::endl;
  return true;
}

void PlaylistComponent::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "PlaylistComponent::filesDropped" << std::endl;
    
    for (const auto& filePath : files)
        {
            File file(filePath);

            if (file.existsAsFile())
            {
                
                trackURLs.push_back(file);
                trackTitles.push_back(file.getFileNameWithoutExtension().toStdString());
                std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
                if(reader)
                {
                    double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
                    int minutes = static_cast<int>(durationInSeconds) / 60;
                    int seconds = static_cast<int>(durationInSeconds) % 60;
                    trackDurations.push_back(juce::String(minutes) + ":" + juce::String(seconds).paddedLeft('0', 2));
                }
                else
                {
                    trackDurations.push_back("N/A");
                }
            }
        }
    
    savePlaylist();
    tableComponent.updateContent();
    
}

void PlaylistComponent::savePlaylist()
{
    if(playlistSave.existsAsFile())
    {
        playlistSave.deleteFile();
    }
    FileOutputStream outputStream(playlistSave);
    
    if(outputStream.openedOk())
    {
        for(auto& file : trackURLs)
        {
            outputStream << file.getFullPathName() << "\n";
        }
    }
}

void PlaylistComponent::loadPlaylist()
{
    
    if(playlistSave.existsAsFile())
    {
        trackURLs.clear();
        trackTitles.clear();
        
        StringArray playlistURL;
        playlistSave.readLines(playlistURL);
        for(auto& line : playlistURL)
        {
            File file(line);
            if(file.existsAsFile())
            {
                trackURLs.push_back(file);
                trackTitles.push_back(file.getFileNameWithoutExtension().toStdString());
//                DBG("Pushed into thing" << trackTitles[0]);
                std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
                if(reader)
                {
                    double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
                    int minutes = static_cast<int>(durationInSeconds) / 60;
                    int seconds = static_cast<int>(durationInSeconds) % 60;
                    trackDurations.push_back(juce::String(minutes) + ":" + juce::String(seconds).paddedLeft('0', 2));
                }
                else
                {
                    trackDurations.push_back("N/A");
                }
            }
        }
    }
}

void PlaylistComponent::timerCallback()
{
    stopTimer();
    loadPlaylist();
    tableComponent.updateContent();
}

void PlaylistComponent::searchList(const String &query)
{
    filteredNames.clear();
    filteredURLs.clear();
    filteredDurations.clear();
    
    if(!query.isEmpty())
    {
        for (int i = 0; i < trackTitles.size(); ++i) {
            if(String(trackTitles[i]).containsIgnoreCase(query))
            {
                
                filteredNames.add(trackTitles[i]);
                filteredURLs.add(trackURLs[i]);
                filteredDurations.add(trackDurations[i]);
                DBG("Song name " << trackTitles[i] << " Contains " << query);
            }
        }
        isSearching = true;
//        tableComponent.updateContent();
        
    }
    else
    {
        for (int i = 0; i < trackTitles.size(); ++i)
        {
            filteredNames.add(trackTitles[i]);
            filteredURLs.add(trackURLs[i]);
            filteredDurations.add(trackDurations[i]);
            isSearching = false;
        }
    }
    tableComponent.updateContent();
}
