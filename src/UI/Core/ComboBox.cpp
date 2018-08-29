// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ComboBox.h"

/**
 * C'tor
 */
ComboBox::ComboBox()
{
	init();
}


/**
 * D'tor
 */
ComboBox::~ComboBox()
{
	resized().disconnect(this, &ComboBox::resizedHandler);
	moved().disconnect(this, &ComboBox::repositioned);
	lstItems.selectionChanged().disconnect(this, &ComboBox::selectionChanged);
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &ComboBox::onMouseDown);
	Utility<EventHandler>::get().mouseWheel().disconnect(this, &ComboBox::onMouseWheel);
}


/**
 * Internal initializer.
 * 
 * Performs basic init'ing of internal components.
 */
void ComboBox::init()
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &ComboBox::onMouseDown);
	Utility<EventHandler>::get().mouseWheel().connect(this, &ComboBox::onMouseWheel);

	btnDown.image("ui/icons/down.png");
	btnDown.size(20, 20);

	txtField.editable(false);
	lstItems.visible(false);
	lstItems.height(300);
	lstItems.sorted(false);

	resized().connect(this, &ComboBox::resizedHandler);
	moved().connect(this, &ComboBox::repositioned);
	lstItems.selectionChanged().connect(this, &ComboBox::selectionChanged);
}


/**
 * Font changed event handler.
 */
void ComboBox::onFontChanged()
{
	txtField.font(font());
	lstItems.font(font());
}


/**
 * Resized event handler.
 */
void ComboBox::resizedHandler(Control* c)
{
	if (height() < 20) { height(20); } // enforce minimum height;
	if (width() < 50) { width(50); } // enforce mininum width;

	btnDown.position(positionX() + width() - btnDown.width(), positionY());
	btnDown.height(height());
	txtField.width(width() - 20);
	txtField.height(height());
	lstItems.width(width());
	lstItems.position(positionX(), positionY() + height());

	mBaseArea(positionX(), positionY(), width(), btnDown.height());
}


/**
 * Position changed event handler.
 */
void ComboBox::repositioned(float, float)
{
	btnDown.position(positionX() + width() - btnDown.width(), positionY());
	txtField.position(positionX(), positionY());
	lstItems.position(positionX(), positionY() + height());

	mBaseArea(positionX(), positionY(), width(), btnDown.height());
}


/**
 * Mouse button down event handler.
 */
void ComboBox::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (button != EventHandler::BUTTON_LEFT) { return; }

	if (isPointInRect(Point_2d(x, y), mBaseArea))
	{
		lstItems.visible(!lstItems.visible());
		lstItems.visible() ? _rect().height(height() + lstItems.height()) : _rect()(mBaseArea.x(), mBaseArea.y(), mBaseArea.width(), mBaseArea.height());
	}
	else if (!isPointInRect(Point_2d(x, y), lstItems.rect()))
	{
		lstItems.visible(false);
		_rect()(mBaseArea.x(), mBaseArea.y(), mBaseArea.width(), mBaseArea.height());
	}
}


void ComboBox::onMouseWheel(int x, int y)
{

}


/**
 * ListBox selection changed event handler.
 */
void ComboBox::selectionChanged()
{
	txtField.text(lstItems.selectionText());
	lstItems.visible(false);
	_rect()(mBaseArea.x(), mBaseArea.y(), mBaseArea.width(), mBaseArea.height());
}


/**
 * Sets the maximum number of items to display before showing a scroll bar.
 */
void ComboBox::maxDisplayItems(int count)
{
	mMaxDisplayItems = count;
	
	if (count < constants::MINIMUM_DISPLAY_ITEMS)
	{
		mMaxDisplayItems = constants::MINIMUM_DISPLAY_ITEMS;
	}
}


/**
 * Adds an item to the list.
 */
void ComboBox::addItem(const std::string& item)
{
	lstItems.addItem(item);

	if (lstItems.count() > mMaxDisplayItems) { return; }
	lstItems.height(lstItems.count() * lstItems.lineHeight());
}


/**
 * 
 */
void ComboBox::update()
{
	txtField.update();
	btnDown.update();
	lstItems.update();
}
