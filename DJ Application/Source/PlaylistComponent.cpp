#include <JuceHeader.h>
#include "PlaylistComponent.h"

// Constructor for PlaylistComponent, initialises the table and UI elements
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2)
: player1(_player1), player2(_player2)
{
    // Add the table component and set it as the model
    addAndMakeVisible(tableComponent);
    tableComponent.setModel(this);
    
    // Define table columns for track title and queue management
    tableComponent.getHeader().addColumn("Track title", 1, 600);
    tableComponent.getHeader().addColumn("Left Queue", 2, 100);
    tableComponent.getHeader().addColumn("Right Queue", 3, 100);
    
    // Initialise deck buttons for adding tracks to left and right decks
    addAndMakeVisible(addToLeftDeckButton);
    addToLeftDeckButton.addListener(this);
    addAndMakeVisible(addToRightDeckButton);
    addToRightDeckButton.addListener(this);
    
    // Initialise queue management buttons
    addAndMakeVisible(playNextLeftButton);
    playNextLeftButton.addListener(this);
    playNextLeftButton.setEnabled(false);
    
    addAndMakeVisible(playNextRightButton);
    playNextRightButton.addListener(this);
    playNextRightButton.setEnabled(false);
    
    // Initialise delete button for removing tracks
    addAndMakeVisible(deleteButton);
    deleteButton.addListener(this);
    
    // Initialise search input and clear search button
    addAndMakeVisible(searchInput);
    searchInput.setTextToShowWhenEmpty("Search tracks...", Colours::grey);
    searchInput.onTextChange = [this] { filterPlaylist(); };
        
    addAndMakeVisible(clearSearchButton);
    clearSearchButton.addListener(this);
        
    // Initialise search text
    currentSearchText = "";


}

// Destructor
PlaylistComponent::~PlaylistComponent()
{
}

// Paint background with default colour
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

}

// Layout of GUI for playlist
void PlaylistComponent::resized()
{
    // Position the queue management controls at the top
    auto area = getLocalBounds();
    auto topSection = area.removeFromTop(40);
    
    // Position the search components
    auto searchArea = area.removeFromTop(40);
    searchInput.setBounds(searchArea.removeFromLeft(getWidth() - 100).reduced(5, 5));
    clearSearchButton.setBounds(searchArea.reduced(5, 5));
    
    // Position the deck buttons
    int buttonWidth = topSection.getWidth() / 4;
    
    addToLeftDeckButton.setBounds(topSection.removeFromLeft(buttonWidth));
    addToRightDeckButton.setBounds(topSection.removeFromLeft(buttonWidth));
    playNextLeftButton.setBounds(topSection.removeFromLeft(buttonWidth));
    playNextRightButton.setBounds(topSection.removeFromLeft(buttonWidth));
    
    // Position the delete button
    deleteButton.setBounds(area.removeFromTop(30).reduced(getWidth() / 4, 0));
    
    // Position the table
    tableComponent.setBounds(area.reduced(0, 10));
}

// Returns the number of rows in the table, accounting for search filtering
int PlaylistComponent::getNumRows()
{
    if (currentSearchText.isEmpty())
    {
        return trackTitles.size();
    }
    else
    {
        return filteredIndices.size();
    }
}

// Filters the playlist based on user input in the search bar
void PlaylistComponent::filterPlaylist()
{
    currentSearchText = searchInput.getText().toLowerCase();
    filteredIndices.clear();
    
    if (currentSearchText.isEmpty())
    {
        // Show all
        tableComponent.updateContent();
        return;
    }
    
    // Search and collect matching indices
    for (int i = 0; i < trackTitles.size(); ++i)
    {
        String title = String(trackTitles[i]).toLowerCase();
        if (title.contains(currentSearchText))
        {
            filteredIndices.push_back(i);
        }
    }
    
    tableComponent.updateContent();
}

// Paints row background
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
   
    if (rowIsSelected)
    {
        g.fillAll(Colours::lightblue);
    }
    else
    {
        g.fillAll(Colours::darkgrey);
    }
}

// Paints the track title and queue status inside the table cells
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(Colours::white);
    g.setFont(14.0f);
    
    // Get actual row index when filtering
    int actualRow = rowNumber;
    if (!currentSearchText.isEmpty() && rowNumber < filteredIndices.size())
    {
        actualRow = filteredIndices[rowNumber];
    }
    
    // Track Title
    if (columnId == 1)
    {
        g.drawText(trackTitles[actualRow], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    // Left Queue Status
    else if (columnId == 2)
    {
        // Check if track is in left queue
        for (const auto& track : leftDeckQueue)
        {
            if (track.rowIndex == actualRow)
            {
                g.setColour(Colours::orange);
                g.drawText("Queued", 2, 0, width - 4, height, Justification::centred, true);
                break;
            }
        }
    }
    // Right Queue Status
    else if (columnId == 3)
    {
        // Check if track is in right queue
        for (const auto& track : rightDeckQueue)
        {
            if (track.rowIndex == actualRow)
            {
                g.setColour(Colours::orange);
                g.drawText("Queued", 2, 0, width - 4, height, Justification::centred, true);
                break;
            }
        }
    }
}

// Create UI components for load, delete,
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    // Get the actual row index when filtering
    int actualRow = rowNumber;
    if (!currentSearchText.isEmpty() && rowNumber < filteredIndices.size())
    {
        actualRow = filteredIndices[rowNumber];
    }

    return existingComponentToUpdate;
}

// Update buttonClicked to handle clearSearchButton:
void PlaylistComponent::buttonClicked(Button* button)
{
    // Check which button was clicked
    if (button == &addToLeftDeckButton)
    {
        int selectedRow = tableComponent.getSelectedRow();
        if (selectedRow >= 0)
        {
            // Get actual row when filtering
            if (!currentSearchText.isEmpty() && selectedRow < filteredIndices.size())
            {
                selectedRow = filteredIndices[selectedRow];
            }
            addToQueue(selectedRow, true);
        }
    }
    else if (button == &addToRightDeckButton)
    {
        int selectedRow = tableComponent.getSelectedRow();
        if (selectedRow >= 0)
        {
            // Get actual row when filtering
            if (!currentSearchText.isEmpty() && selectedRow < filteredIndices.size())
            {
                selectedRow = filteredIndices[selectedRow];
            }
            addToQueue(selectedRow, false);
        }
    }
    else if (button == &playNextLeftButton)
    {
        playNextInQueue(true);
    }
    else if (button == &playNextRightButton)
    {
        playNextInQueue(false);
    }
    else if (button == &clearSearchButton)
    {
        searchInput.setText("", false);
        currentSearchText = "";
        filteredIndices.clear();
        tableComponent.updateContent();
    }
    else if (button == &deleteButton)
    {
        int selectedRow = tableComponent.getSelectedRow();
        if (selectedRow >= 0)
        {
            // Get actual row when filtering
            if (!currentSearchText.isEmpty() && selectedRow < filteredIndices.size())
            {
                selectedRow = filteredIndices[selectedRow];
            }
            
            if (selectedRow < trackTitles.size())
            {
                // Remove from queues if present
                for (auto it = leftDeckQueue.begin(); it != leftDeckQueue.end();)
                {
                    if (it->rowIndex == selectedRow)
                    {
                        it = leftDeckQueue.erase(it);
                    }
                    else if (it->rowIndex > selectedRow)
                    {
                        // Adjust indices for tracks after the deleted one
                        it->rowIndex--;
                        ++it;
                    }
                    else
                    {
                        ++it;
                    }
                }
                
                for (auto it = rightDeckQueue.begin(); it != rightDeckQueue.end();)
                {
                    if (it->rowIndex == selectedRow)
                    {
                        it = rightDeckQueue.erase(it);
                    }
                    else if (it->rowIndex > selectedRow)
                    {
                        // Adjust indices for tracks after the deleted one
                        it->rowIndex--;
                        ++it;
                    }
                    else
                    {
                        ++it;
                    }
                }
                
                // Remove from playlist
                trackTitles.erase(trackTitles.begin() + selectedRow);
                trackURLs.erase(trackURLs.begin() + selectedRow);
                
                // Re-filter if we're searching
                if (!currentSearchText.isEmpty())
                {
                    filterPlaylist();
                }
                else
                {
                    tableComponent.updateContent();
                }
                updateQueueButtons();
            }
        }
    }
    else
    {
        // Check if it's a load or delete button
        String componentID = button->getComponentID();
        
        if (componentID.isNotEmpty())
        {
            char action = componentID[0];
            int rowNumber = componentID.substring(1).getIntValue();
            
            if (rowNumber >= 0 && rowNumber < trackTitles.size())
            {
                if (action == 'L') // Load to Left Deck
                {
                    loadFileToPlayer(trackURLs[rowNumber], true);
                }
                else if (action == 'R') // Load to Right Deck
                {
                    loadFileToPlayer(trackURLs[rowNumber], false);
                }
                else if (action == 'D') // Delete from playlist
                {
                    // Remove from queues if present
                    for (auto it = leftDeckQueue.begin(); it != leftDeckQueue.end();)
                    {
                        if (it->rowIndex == rowNumber)
                        {
                            it = leftDeckQueue.erase(it);
                        }
                        else if (it->rowIndex > rowNumber)
                        {
                            // Adjust indices for tracks after the deleted one
                            it->rowIndex--;
                            ++it;
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    
                    for (auto it = rightDeckQueue.begin(); it != rightDeckQueue.end();)
                    {
                        if (it->rowIndex == rowNumber)
                        {
                            it = rightDeckQueue.erase(it);
                        }
                        else if (it->rowIndex > rowNumber)
                        {
                            // Adjust indices for tracks after the deleted one
                            it->rowIndex--;
                            ++it;
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    
                    // Remove from playlist
                    trackTitles.erase(trackTitles.begin() + rowNumber);
                    trackURLs.erase(trackURLs.begin() + rowNumber);
                    
                    // Re-filter if we're searching
                    if (!currentSearchText.isEmpty())
                    {
                        filterPlaylist();
                    }
                    else
                    {
                        tableComponent.updateContent();
                    }
                    updateQueueButtons();
                }
            }
        }
    }
}

// Drag & drop file handling
bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    // Accept audio file types
    for (const String& file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    return false;
}

// Handles file drop events and adds tracks to the playlist
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    for (const String& filename : files)
    {
        URL fileURL = URL(File(filename));
        File file(filename);
        addToPlaylist(fileURL, file.getFileNameWithoutExtension());
    }
}

// Adds a track to the playlist
void PlaylistComponent::addToPlaylist(URL trackURL, const String& trackTitle)
{
    // Push to vectors
    trackURLs.push_back(trackURL);
    trackTitles.push_back(trackTitle.toStdString());
    // Update table
    tableComponent.updateContent();
}

// Adds a track to the queue for playback
void PlaylistComponent::addToQueue(int rowNumber, bool leftDeck)
{
    if (rowNumber >= 0 && rowNumber < trackTitles.size())
    {
        QueuedTrack track;
        track.url = trackURLs[rowNumber];
        track.title = trackTitles[rowNumber];
        track.rowIndex = rowNumber;
        
        if (leftDeck)
        {
            leftDeckQueue.push_back(track);
        }
        else
        {
            rightDeckQueue.push_back(track);
        }
        
        updateQueueButtons();
        tableComponent.updateContent();
    }
}

// Play next track in queue
void PlaylistComponent::playNextInQueue(bool leftDeck)
{
    if (leftDeck && !leftDeckQueue.empty())
    {
        QueuedTrack track = leftDeckQueue.front();
        leftDeckQueue.erase(leftDeckQueue.begin());
        
        loadFileToPlayer(track.url, true);
    }
    else if (!leftDeck && !rightDeckQueue.empty())
    {
        QueuedTrack track = rightDeckQueue.front();
        rightDeckQueue.erase(rightDeckQueue.begin());
        
        loadFileToPlayer(track.url, false);
    }
    
    updateQueueButtons();
    tableComponent.updateContent();
}

// Loads file
void PlaylistComponent::loadFileToPlayer(URL fileURL, bool leftDeck)
{
    if (leftDeck)
    {
        player1->loadURL(fileURL);
    }
    else
    {
        player2->loadURL(fileURL);
    }
}

// Updates state of play next according to queue (empty or not)
void PlaylistComponent::updateQueueButtons()
{
    playNextLeftButton.setEnabled(!leftDeckQueue.empty());
    playNextRightButton.setEnabled(!rightDeckQueue.empty());
}


