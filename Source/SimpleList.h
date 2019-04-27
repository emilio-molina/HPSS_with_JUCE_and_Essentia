/*
  ==============================================================================

    SimpleList.h
    Created: 27 Apr 2019 11:25:33am
    Author:  Emilio Molina

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SimpleList : public Component, public ListBoxModel {
    std::function<void(int)> _selectedRowsChangedCallback;
public:
    SimpleList() {
        _selectedRowsChangedCallback = std::function<void(int)>();
        box.setModel(this);
        box.setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2f));
        box.setRowHeight(20);
        addAndMakeVisible(box);
    }
    
    void setSelectedRowsChangedCallback(std::function<void(int)> selectedRowsChangedCallback) {
        _selectedRowsChangedCallback = selectedRowsChangedCallback;
    }
    
    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height,
                          bool rowIsSelected) override {
        
        if(!isPositiveAndBelow(rowNumber, items.size())) return;
        g.fillAll(Colour::greyLevel(rowIsSelected ? 0.15f : 0.05f));
        g.setFont(14);
        g.setColour(Colours::whitesmoke);
        g.drawFittedText(items.getReference(rowNumber),{6,0,width - 12,height}, Justification::centredLeft, 1, 1.f);
    }
    
    int getNumRows() override {
        return items.size();
    }
    
    void addItem(const String &text) {
        items.add(text);
        box.updateContent();
    }
    
    void resized() override {
        box.setBounds(getLocalBounds());
    }
    
    void selectedRowsChanged(int rowId) override {
        if (_selectedRowsChangedCallback)
            _selectedRowsChangedCallback(rowId);
    }
    
private:
    ListBox box;
    StringArray items;
};
