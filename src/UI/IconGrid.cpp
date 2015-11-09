#include "IconGrid.h"

#include "../Constants.h"


IconGrid::IconGrid():	mCurrentSelection(constants::NO_SELECTION),
						mIconSize(0),
						mIconMargin(0),
						mHighlightIndex(constants::NO_SELECTION)
{
	Utility<EventHandler>::get().mouseButtonDown().Connect(this, &IconGrid::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().Connect(this, &IconGrid::onMouseMotion);
}


IconGrid::~IconGrid()
{
	Utility<EventHandler>::get().mouseButtonDown().Disconnect(this, &IconGrid::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().Disconnect(this, &IconGrid::onMouseMotion);
}


void IconGrid::sheetPath(const std::string& _path)
{
	mIconSheet = Image(_path);
}


void  IconGrid::iconSize(int _size)
{
	mIconSize = _size;
	updateGrid();
}


void IconGrid::iconMargin(int _margin)
{
	mIconMargin = _margin;
	updateGrid();
}


/**
 * Updates the icon grid.
 */
void IconGrid::updateGrid()
{
	int cols = (rect().w() - (mIconMargin * 2)) / (mIconSize + mIconMargin);
	int rows = (rect().h() - (mIconMargin * 2)) / (mIconSize + mIconMargin);

	mGridSize(cols, rows);
}


void IconGrid::onMouseDown(MouseButton button, int x, int y)
{
	if (mIconItemList.empty())
		return;

	if (!isPointInRect(x, y, rect().x(), rect().y(), mGridSize.x() * (mIconSize + mIconMargin), mGridSize.y() * (mIconSize + mIconMargin)))
	{
		mCurrentSelection = -1;
		return;
	}

	mCurrentSelection = translateCoordsToIndex(x - rect().x(), y - rect().y());

	if (mCurrentSelection >= mIconItemList.size())
		mCurrentSelection = constants::NO_SELECTION;
}


void IconGrid::onMouseMotion(int x, int y, int dX, int dY)
{
	if (mIconItemList.empty())
		return;

	if (!isPointInRect(x, y, rect().x(), rect().y(), mGridSize.x() * (mIconSize + mIconMargin), mGridSize.y() * (mIconSize + mIconMargin)))
	{
		mHighlightIndex = constants::NO_SELECTION;
		return;
	}

	// Assumes all coordinates are not negative.
	mHighlightIndex = translateCoordsToIndex(x - rect().x(), y - rect().y());

	if (mHighlightIndex >= mIconItemList.size())
		mHighlightIndex = constants::NO_SELECTION;
}


int IconGrid::translateCoordsToIndex(int x, int y)
{
	return (x / (mIconSize + mIconMargin)) + (mGridSize.x() * (y / (mIconSize + mIconMargin)));
}


void IconGrid::positionChanged(float dX, float dY)
{

}


void IconGrid::sizeChanged()
{
	updateGrid();
}


/**
 * Adds an Icon Item to the IconGrid.
 * 
 * \note	This function does no sanity checking.
 */
void IconGrid::addItem(const std::string& name, int sheetIndex)
{
	int x_pos = (sheetIndex % (mIconSheet.width() / mIconSize)) * mIconSize;
	int y_pos = (sheetIndex / (mIconSheet.width() / mIconSize)) * mIconSize;

	mIconItemList.push_back(IconGridItem());
	mIconItemList.back()._imgSheetCoords((float)x_pos, (float)y_pos);
	mIconItemList.back()._name = name;
}


void IconGrid::update()
{
	if (!visible())
		return;

	Renderer& r = Utility<Renderer>::get();

	Rectangle_2df r2d = rect();

	r.drawBoxFilled(rect(), 0, 0, 0);

	if (mIconItemList.empty())
		return;


	for (size_t i = 0; i < mIconItemList.size(); ++i)
	{
		int x_pos = (i % (mIconSheet.width() / mIconSize));
		int y_pos = (i / (mIconSheet.width() / mIconSize));

		float x = (rect().x() + mIconMargin) + (x_pos * mIconSize) + (mIconMargin * x_pos);
		float y = (rect().y() + mIconMargin) + (y_pos * mIconSize) + (mIconMargin * y_pos);

		r.drawSubImage(mIconSheet, x, y, mIconItemList[i]._imgSheetCoords.x(), mIconItemList[i]._imgSheetCoords.y(), mIconSize, mIconSize);
	}

	if (mCurrentSelection != constants::NO_SELECTION)
	{
		int x_pos = (mCurrentSelection % mGridSize.x());
		int y_pos = (mCurrentSelection / mGridSize.x());
		r.drawBox((rect().x() + mIconMargin) + (x_pos * mIconSize) + (mIconMargin * x_pos), (rect().y() + mIconMargin) + (y_pos * mIconSize) + (mIconMargin * y_pos), mIconSize, mIconSize, 0, 100, 255);
	}

	if (mHighlightIndex != constants::NO_SELECTION)
	{
		int x_pos = (mHighlightIndex % mGridSize.x());
		int y_pos = (mHighlightIndex / mGridSize.x());
		r.drawBox((rect().x() + mIconMargin) + (x_pos * mIconSize) + (mIconMargin * x_pos), (rect().y() + mIconMargin) + (y_pos * mIconSize) + (mIconMargin * y_pos), mIconSize, mIconSize, 0, 180, 0);

		r.drawBoxFilled(rect().x() + x_pos, rect().y() + y_pos - 15, font().width(mIconItemList[mHighlightIndex]._name) + 6, 12, 245, 245, 245);
		r.drawBox(rect().x() + x_pos, rect().y() + y_pos - 15, font().width(mIconItemList[mHighlightIndex]._name) + 6, 12, 175, 175, 175);

		r.drawText(font(), mIconItemList[mHighlightIndex]._name, rect().x() + x_pos + 3, rect().y() + y_pos - 15, 0, 0, 0);
	}
}