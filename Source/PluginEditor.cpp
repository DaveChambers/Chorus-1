/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusAudioProcessorEditor::ChorusAudioProcessorEditor (ChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mState(p.getState())
{
	int windowWidth  = 2 * mWindowMarginWidth  + 3 * mSliderWidth + 2 * mLabelWidht + 10;
	int windowHeight = 2 * mWindowMarginHeight + 3 * mSliderHeight + mTitleHeight;
    setSize (windowWidth, windowHeight);
	initialiseGUI();
	mFlexArea = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
}

ChorusAudioProcessorEditor::~ChorusAudioProcessorEditor()
{
	// Empty destructor
}

//==============================================================================
void ChorusAudioProcessorEditor::paint (Graphics& g)
{
	// Fill background
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll (bgColour);
	// Draw lines to separate sections
	g.setColour(bgColour.darker(.5));
	auto area = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
	auto mixVolumeArea = area.removeFromLeft(mLabelWidht + 5);

	// Draw line between mix/volume and other controls
	g.drawLine(Line<float>(mixVolumeArea.getTopRight().toFloat(), mixVolumeArea.getBottomRight().toFloat()), 3.f);
	area.removeFromLeft(5);

	// Draw lines between labels (feedback, frequency, depth)
	auto labelArea = area.removeFromLeft(mLabelWidht);
	g.setColour(bgColour.darker(.5));
	labelArea.removeFromLeft(4);
	labelArea.removeFromRight(8);
	labelArea.removeFromTop(mTitleHeight + 3);
	g.drawLine(Line<float>(labelArea.getTopLeft().toFloat(), labelArea.getTopRight().toFloat()), 1.f);
	labelArea.removeFromTop(mSliderHeight);
	g.drawLine(Line<float>(labelArea.getTopLeft().toFloat(), labelArea.getTopRight().toFloat()), 1.f);
	labelArea.removeFromTop(mSliderHeight);
	g.drawLine(Line<float>(labelArea.getTopLeft().toFloat(), labelArea.getTopRight().toFloat()), 1.f);

	// Divide LCR controls
	g.setColour(bgColour.darker(.5));
	auto topLeft = area.getTopLeft();
	auto topRight = area.getTopRight();
	auto bottomLeft = area.getBottomLeft();
	area.removeFromTop(mTitleHeight + 3);
	g.drawLine(topLeft.getX(), topLeft.getY(), bottomLeft.getX(), bottomLeft.getY(), 3.f);
	g.drawLine(area.getTopLeft().getX(), area.getTopLeft().getY(), area.getTopRight().getX(), area.getTopRight().getY(), 3.f);
	for (auto i = 0; i < 2; ++i)
	{
		area.removeFromTop(mSliderHeight);
		g.drawLine(area.getTopLeft().getX(), area.getTopLeft().getY(), area.getTopRight().getX(), area.getTopRight().getY(), 3.f);
	}
}

//==============================================================================
void ChorusAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
	auto mixArea = area.removeFromLeft(jmax(mLabelWidht, mSliderWidth));
	area.removeFromLeft(10);

	// Mix ===============================
	FlexBox mixBox;
	mixBox.alignContent = FlexBox::AlignContent::center;
	mixBox.justifyContent = FlexBox::JustifyContent::center;
	mixBox.flexDirection = FlexBox::Direction::column;
	mixBox.items.addArray(
		{
			createItem(mMixLabel, mLabelWidht, mLabelHeight),
			createItem(mMixSlider, mSliderWidth, mSliderHeight)
		});

	auto gainArea = mixArea.removeFromBottom(static_cast<int>(mixArea.getHeight() / 2.f));
	mixBox.performLayout(mixArea.toFloat());


	FlexBox gainBox;
	gainBox.alignContent = FlexBox::AlignContent::center;
	gainBox.justifyContent = FlexBox::JustifyContent::center;
	gainBox.flexDirection = FlexBox::Direction::column;
	gainBox.items.addArray(
		{
			createItem(mVolumeLabel, mLabelWidht, mLabelHeight),
			createItem(mVolumeSlider, mSliderWidth, mSliderHeight)
		});

	gainBox.performLayout(gainArea.toFloat());

	//LCR box
	FlexBox LCRBox;
	LCRBox.alignContent = FlexBox::AlignContent::center;
	LCRBox.justifyContent = FlexBox::JustifyContent::center;
	LCRBox.flexDirection = FlexBox::Direction::row;
	LCRBox.items.addArray(
		{
			createItem(mLeftTitleLabel, mSliderWidth, mTitleHeight),
			createItem(mCenterTitleLabel, mSliderWidth, mTitleHeight),
			createItem(mRightTitleLabel, mSliderWidth, mTitleHeight),
		});

	auto LCRArea = area.removeFromTop(mTitleHeight);
	LCRArea.removeFromLeft(mLabelWidht);
	LCRBox.performLayout(LCRArea);

	// Title box
	FlexBox titleBox;
	titleBox.alignContent = FlexBox::AlignContent::center;
	titleBox.justifyContent = FlexBox::JustifyContent::center;
	titleBox.flexDirection = FlexBox::Direction::column;
	titleBox.items.addArray(
		{
			createItem(mFreqTitleLabel, mLabelWidht, mSliderHeight),
			createItem(mFeedbackTitleLabel, mLabelWidht, mSliderHeight),
			createItem(mDepthTitleLabel, mLabelWidht, mSliderHeight)
		});
	auto titleArea = area.removeFromLeft(mLabelWidht);
	titleBox.performLayout(titleArea.toFloat());
	
	// Left box
	FlexBox leftBox;
	leftBox.alignContent = FlexBox::AlignContent::center;
	leftBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	leftBox.flexDirection = FlexBox::Direction::column;
	leftBox.items.addArray(
		{
			createItem(mLfoFreqLSlider, mSliderWidth, mSliderHeight),
			createItem(mFeedbackLSlider, mSliderWidth, mSliderHeight),
			createItem(mLfoDepthLSlider, mSliderWidth, mSliderHeight)
		});

	auto leftArea = area.removeFromLeft(mSliderWidth);
	leftBox.performLayout(leftArea.toFloat());

	// Center box
	FlexBox centerBox;
	centerBox.alignContent = FlexBox::AlignContent::center;
	centerBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	centerBox.flexDirection = FlexBox::Direction::column;
	centerBox.items.addArray(
		{
			createItem(mLfoFreqCSlider, mSliderWidth, mSliderHeight),
			createItem(mFeedbackCSlider, mSliderWidth, mSliderHeight),
			createItem(mLfoDepthCSlider, mSliderWidth, mSliderHeight)
		});
	auto centerArea = area.removeFromLeft(mSliderWidth);
	centerBox.performLayout(centerArea.toFloat());

	// Right box
	FlexBox rightBox;
	rightBox.alignContent = FlexBox::AlignContent::center;
	rightBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	rightBox.flexDirection = FlexBox::Direction::column;
	rightBox.items.addArray(
		{
			createItem(mLfoFreqRSlider, mSliderWidth, mSliderHeight),
			createItem(mFeedbackRSlider, mSliderWidth, mSliderHeight),
			createItem(mLfoDepthRSlider, mSliderWidth, mSliderHeight)
		});
	auto rightArea = area.removeFromLeft(mSliderWidth);
	rightBox.performLayout(rightArea.toFloat());
}

FlexItem ChorusAudioProcessorEditor::createItem(Component & c, const int & width, const int & height)
{
	return FlexItem(c).withWidth(static_cast<float>(width)).withHeight(static_cast<float>(height));
}

FlexItem ChorusAudioProcessorEditor::createItem(FlexBox & fb, const int & width, const int & height)
{
	return FlexItem(fb).withWidth(static_cast<float>(width)).withHeight(static_cast<float>(height));
}

//==============================================================================
void ChorusAudioProcessorEditor::initialiseGUI()
{
	Colour labelColour = Colours::white;

	// GAIN ====================================
	// Label
	mVolumeLabel.setText("Volume", dontSendNotification);
	mVolumeLabel.setColour(Label::textColourId, labelColour);
	mVolumeLabel.setSize(mLabelWidht, mLabelHeight);
	mVolumeLabel.setFont(mLabelFont);
	mVolumeLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mVolumeLabel);
	// Slider
	mVolumeSlider.setSliderStyle(mSliderStyle);
	mVolumeSlider.setSize(mSliderWidth, mSliderHeight);
	mVolumeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mVolumeSlider.setTextValueSuffix(" dB");
	addAndMakeVisible(mVolumeSlider);
	mVolumeSliderAttachment.reset(new SliderAttachment(mState, IDs::volume, mVolumeSlider));
	mVolumeSlider.setLookAndFeel(&mKnobLookAndFeel);

	// MIX =====================================
	// Label
	mMixLabel.setText("Mix", dontSendNotification);
	mMixLabel.setColour(Label::textColourId, labelColour);
	mMixLabel.setSize(mLabelWidht, mLabelHeight);
	mMixLabel.setFont(mLabelFont);
	mMixLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mMixLabel);
	// Slider
	mMixSlider.setSliderStyle(mSliderStyle);
	mMixSlider.setSize(mSliderWidth, mSliderHeight);
	mMixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mMixSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mMixSlider);
	mMixSliderAttachment.reset(new SliderAttachment(mState, IDs::wetness, mMixSlider));
	mMixSlider.setLookAndFeel(&mKnobLookAndFeel);

	// LFO FREQUENCY ===========================
	// Left ===================
	// Slider
	mLfoFreqLSlider.setSliderStyle(mSliderStyle);
	mLfoFreqLSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqLSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqLSlider);
	mLfoFreqLSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqL, mLfoFreqLSlider));
	mLfoFreqLSlider.setLookAndFeel(&mKnobLookAndFeel);

	// Center ==================
	// Slider
	mLfoFreqCSlider.setSliderStyle(mSliderStyle);
	mLfoFreqCSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqCSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqCSlider);
	mLfoFreqCSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqC, mLfoFreqCSlider));
	mLfoFreqCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mLfoFreqRSlider.setSliderStyle(mSliderStyle);
	mLfoFreqRSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqRSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqRSlider);
	mLfoFreqRSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqR, mLfoFreqRSlider));
	mLfoFreqRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// LFO DEPTH ===============================
	// Left ====================
	// Slider
	mLfoDepthLSlider.setSliderStyle(mSliderStyle);
	mLfoDepthLSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthLSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthLSlider);
	mLfoDepthLSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthL, mLfoDepthLSlider));
	mLfoDepthLSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Center =================
	// Slider
	mLfoDepthCSlider.setSliderStyle(mSliderStyle);
	mLfoDepthCSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthCSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthCSlider);
	mLfoDepthCSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthC, mLfoDepthCSlider));
	mLfoDepthCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mLfoDepthRSlider.setSliderStyle(mSliderStyle);
	mLfoDepthRSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthRSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthRSlider);
	mLfoDepthRSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthR, mLfoDepthRSlider));
	mLfoDepthRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// FEEDBACK ================================
	// Left ====================
	// Slider
	mFeedbackLSlider.setSliderStyle(mSliderStyle);
	mFeedbackLSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackLSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackLSlider);
	mFeedbackLSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackL, mFeedbackLSlider));
	mFeedbackLSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Center ==================
	// Slider
	mFeedbackCSlider.setSliderStyle(mSliderStyle);
	mFeedbackCSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackCSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackCSlider);
	mFeedbackCSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackC, mFeedbackCSlider));
	mFeedbackCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mFeedbackRSlider.setSliderStyle(mSliderStyle);
	mFeedbackRSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackRSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackRSlider);
	mFeedbackRSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackR, mFeedbackRSlider));
	mFeedbackRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// TITLE LABELS =====================================
	// FeedbackTitle
	mFeedbackTitleLabel.setText("Feedback", dontSendNotification);
	mFeedbackTitleLabel.setColour(Label::textColourId, labelColour);
	mFeedbackTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mFeedbackTitleLabel.setFont(mLabelFont);
	mFeedbackTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mFeedbackTitleLabel);
	// Depth
	mDepthTitleLabel.setText("Depth", dontSendNotification);
	mDepthTitleLabel.setColour(Label::textColourId, labelColour);
	mDepthTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mDepthTitleLabel.setFont(mLabelFont);
	mDepthTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mDepthTitleLabel);
	// Frequency
	mFreqTitleLabel.setText("Frequency", dontSendNotification);
	mFreqTitleLabel.setColour(Label::textColourId, labelColour);
	mFreqTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mFreqTitleLabel.setFont(mLabelFont);
	mFreqTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mFreqTitleLabel);
	// Left
	mLeftTitleLabel.setText("L", dontSendNotification);
	mLeftTitleLabel.setColour(Label::textColourId, labelColour);
	mLeftTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mLeftTitleLabel.setFont(mTitleFont);
	mLeftTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mLeftTitleLabel);
	// Center
	mCenterTitleLabel.setText("C", dontSendNotification);
	mCenterTitleLabel.setColour(Label::textColourId, labelColour);
	mCenterTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mCenterTitleLabel.setFont(mTitleFont);
	mCenterTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mCenterTitleLabel);
	// Right
	mRightTitleLabel.setText("R", dontSendNotification);
	mRightTitleLabel.setColour(Label::textColourId, labelColour);
	mRightTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mRightTitleLabel.setFont(mTitleFont);
	mRightTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mRightTitleLabel);
}
