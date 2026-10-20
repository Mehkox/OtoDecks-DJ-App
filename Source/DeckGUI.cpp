/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
                 ) : waveformDisplay(formatManagerToUse, cacheToUse),player(_player),
formatManager(formatManagerToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volLabel);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);
    
    songTitle.setText("No Track Loaded", dontSendNotification);
    songTitle.setJustificationType(Justification::topLeft);
    addAndMakeVisible(songTitle);
    
    songDuration.setText("0:00", dontSendNotification);
    songDuration.setJustificationType(Justification::centredRight);
    addAndMakeVisible(songDuration);
    
    currentDuration.setText("0:00", dontSendNotification);
    currentDuration.setJustificationType(Justification::centredLeft);
    addAndMakeVisible(currentDuration);


    playButton.addListener(this);
    pauseButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    
    
    volSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    
    volLabel.setText("Volume", dontSendNotification);
    volLabel.setJustificationType(Justification::centred);

    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.setJustificationType(Justification::centred);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 100.0);
    posSlider.setRange(0.0, 1.0);

    addAndMakeVisible(progressBar);
    progressBar.setSliderStyle(Slider::LinearBar);
    progressBar.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    progressBar.setEnabled(false);
    
    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colour::fromRGB(201, 199, 186));
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (18.0f);
    g.drawText ("", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
    
    //playButton
    playButton.setColour(TextButton::buttonColourId, Colour::fromRGB(41, 41, 43));
    playButton.setColour(TextButton::textColourOffId, Colour::fromRGB(201, 199, 186));
    
    //pauseButton
    pauseButton.setColour(TextButton::buttonColourId, Colour::fromRGB(41, 41, 43));
    pauseButton.setColour(TextButton::textColourOffId, Colour::fromRGB(201, 199, 186));
    
    //volume Slider
    volSlider.setColour(Slider::thumbColourId, Colour::fromRGB(148, 104, 70));
    volSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(31, 122, 140));
    volSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGB(41, 41, 43));
    volLabel.setColour(Label::textColourId, Colour::fromRGB(41, 41, 43));
    volSlider.setColour(Slider::textBoxBackgroundColourId, Colour::fromRGB(41, 41, 43));
    volSlider.setColour(Slider::textBoxTextColourId, Colour::fromRGB(201, 199, 186));
    
    //speed Slider
    speedSlider.setColour(Slider::thumbColourId, Colour::fromRGB(148, 104, 70));
    speedSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGB(31, 122, 140));
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGB(41, 41, 43));
    speedLabel.setColour(Label::textColourId, Colour::fromRGB(41, 41, 43));
    speedSlider.setColour(Slider::textBoxBackgroundColourId, Colour::fromRGB(41, 41, 43));
    speedSlider.setColour(Slider::textBoxTextColourId, Colour::fromRGB(201, 199, 186));
    
    //Title
    songTitle.setColour(Label::textColourId, Colour::fromRGB(41, 41, 43));
    
    //Progress bar
    songDuration.setColour(Label::textColourId, Colour::fromRGB(41, 41, 43));
    currentDuration.setColour(Label::textColourId, Colour::fromRGB(41, 41, 43));
    posSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(41, 41, 43));
    posSlider.setColour(Slider::trackColourId, Colour::fromRGB(31, 122, 140));
    progressBar.setColour(Slider::backgroundColourId, Colour::fromRGB(41, 41, 43));
    progressBar.setColour(Slider::trackColourId, Colour::fromRGB(31, 122, 140));
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 4;
    double knobW = getWidth()/2;
    double progressBarH = 20;
    double waveformHeight = rowH * 1.5;
    double buttonsYcoords = waveformHeight + progressBarH + 30 + 20;
    double rotaryCoords = buttonsYcoords + 90;
    
    waveformDisplay.setBounds(0, 30, getWidth(), waveformHeight);
    currentDuration.setBounds(5, waveformHeight + 30, 50, progressBarH);
    progressBar.setBounds(55, waveformHeight + 30, getWidth()-110, progressBarH);
    songDuration.setBounds(getWidth() - 55,waveformHeight + 30,50, progressBarH);
    posSlider.setBounds(50, waveformHeight + progressBarH + 30, getWidth() - 100 , 20);
//
    playButton.setBounds(0, buttonsYcoords, getWidth()/2, 80);
    pauseButton.setBounds(getWidth()/2, buttonsYcoords, getWidth()/2, 80);
//
    volSlider.setBounds(0, rotaryCoords, knobW, rowH * 1.2);
    volLabel.setBounds(0, rotaryCoords, knobW, rowH);
    speedSlider.setBounds(knobW, rotaryCoords, knobW, rowH * 1.2);
    speedLabel.setBounds(knobW, rotaryCoords, knobW, rowH);
//
    songTitle.setBounds(10, 8, getWidth() - 20, 20);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &pauseButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                player->loadURL(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});
                
                
            }
        });
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
    waveformDisplay.loadURL(URL{File{files[0]}});
      std::cout << files[0] << std::endl;
  }
}

void DeckGUI::timerCallback()
{
//    std::cout << "DeckGUI::timerCallback" << std::endl;
    double relativePos = player->getPositionRelative();
    waveformDisplay.setPositionRelative(relativePos);
    
    double totalDuration = progressBar.getMaximum();
    
    double currentTime = totalDuration * relativePos;
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    String currentTimeStr = String(minutes) + ":" + String(seconds).paddedLeft('0', 2);
    
    currentDuration.setText(currentTimeStr, dontSendNotification);
    
    progressBar.setValue(currentTime,dontSendNotification);
}

void DeckGUI::loadFile(const juce::File &file)
{
    player->loadURL(juce::URL{file});
    waveformDisplay.loadURL(juce::URL{file});
    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader)
    {
        double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
        int minutes = static_cast<int>(durationInSeconds) / 60;
        int seconds = static_cast<int>(durationInSeconds) % 60;

        String durationStr = String(minutes) + ":" + String(seconds).paddedLeft('0', 2);
        songDuration.setText(durationStr, dontSendNotification);
        progressBar.setRange(0, durationInSeconds, dontSendNotification);
    }
}

void DeckGUI::setTrackTitle(const juce::String &title)
{
    songTitle.setText(title, dontSendNotification);
}

void DeckGUI::setTrackDuration(const juce::String &duration)
{
    songDuration.setText(duration, dontSendNotification);
}

