#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

// Defines class that manages a playlist of audio tracks
class PlaylistComponent  : public juce::Component,
public TableListBoxModel, public Button::Listener,
public FileDragAndDropTarget
{
public:
    // Initialises PlaylistComponent with references to two DJ players.
    PlaylistComponent(DJAudioPlayer* player1, DJAudioPlayer* player2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // TableListBoxModel overrides for playlist table
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    // Handles button clicks
    void buttonClicked(Button * button) override;
    
    // File Drag & Drop Implementation
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    // Add track to playlist
    void addToPlaylist(URL trackURL, const String& trackTitle);
       
    // Methods for queue management
    void addToQueue(int rowNumber, bool leftDeck);
    void playNextInQueue(bool leftDeck);
    
    
private:
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    
    std::vector<URL> trackURLs;
    
    // Track queue structures
    struct QueuedTrack
    {
        URL url;
        String title;
        int rowIndex;
    };
        
    std::vector<QueuedTrack> leftDeckQueue;
    std::vector<QueuedTrack> rightDeckQueue;
        
    // Search functionality
    TextEditor searchInput;
    TextButton clearSearchButton{"Clear"};
    std::vector<int> filteredIndices;
    String currentSearchText;
    void filterPlaylist(); // Helper function to filter playlist
    
    // Reference to player for playback
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    
    // UI Components for deck selection and queue management
    TextButton addToLeftDeckButton{"Add to Left Deck"};
    TextButton addToRightDeckButton{"Add to Right Deck"};
    TextButton playNextLeftButton{"Play Next (Left)"};
    TextButton playNextRightButton{"Play Next (Right)"};
        
    // Delete track button
    TextButton deleteButton{"Delete"};
        
    // Helper methods
    void loadFileToPlayer(URL fileURL, bool leftDeck);
    void updateQueueButtons();
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
