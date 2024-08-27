#include "vcaxis.h"
#include "vcaxisgrid.h"
#include "vcaxisscale.h"
#include "vcaxistitle.h"
#include "vcbackdrop.h"
#include "vcbrush.h"
#include "vccategoryscale.h"
#include "vccolor.h"
#include "vccoor.h"
#include "vcdatagrid.h"
#include "vcdatapoint.h"
#include "vcdatapoints.h"
#include "vcdatapointlabel.h"
#include "vcfill.h"
#include "vcfont.h"
#include "vcfootnote.h"
#include "vcframe.h"
#include "vcintersection.h"
#include "vclabel.h"
#include "vclabels.h"
#include "vclcoor.h"
#include "vclegend.h"
#include "vclight.h"
#include "vclightsource.h"
#include "vclightsources.h"
#include "vclocation.h"
#include "vcmarker.h"
#include "vcpen.h"
#include "vcplot.h"
#include "vcplotbase.h"
#include "vcrect.h"
#include "vcseries.h"
#include "vcseriescollection.h"
#include "vcseriesmarker.h"
#include "vcseriesposition.h"
#include "vcshadow.h"
#include "vcstatline.h"
#include "vctextlayout.h"
#include "vctick.h"
#include "vctitle.h"
#include "vcvaluescale.h"
#include "vcview3d.h"
#include "vcwall.h"
#include "vcweighting.h"

//ChartType Constants
#define VtChChartType3dBar				0
#define VtChChartType2dBar				1
#define VtChChartType3dLine				2
#define VtChChartType2dLine				3
#define VtChChartType3dArea				4
#define VtChChartType2dArea				5
#define VtChChartType3dStep				6
#define VtChChartType2dStep				7
#define VtChChartType3dCombination		8
#define VtChChartType2dCombination		9
#define VtChChartType2dPie				14
#define VtChChartType2dXY				16

//LabelLocationType Constants
#define VtChLabelLocationTypeNoneNo		0//label displayed. 
#define VtChLabelLocationTypeAbovePoint	1//The label is displayed above the data point. 
#define VtChLabelLocationTypeBelowPoint	2//The label is displayed below the data point. 
#define VtChLabelLocationTypeCenter		3//The label is displayed centered on the data point. 
#define VtChLabelLocationTypeBase		4//The label is displayed at the base along the category axis, directly beneath the data point. 
#define VtChLabelLocationTypeInside		5//The label is displayed inside a pie slice. 
#define VtChLabelLocationTypeOutside	6//The label is displayed outside a pie slice. 
#define VtChLabelLocationTypeLeft		7//The label is displayed to the left of the data point. 
#define VtChLabelLocationTypeRight		8//The label is displayed to the right of the data point. 

//FillStyle Constants
#define VtFillStyleNull					0//No fill (background shows through) 
#define VtFillStyleBrush				1//A solid color or pattern fill 

//AxisId Constants
#define VtChAxisIdX						0//Identifies the x axis.
#define VtChAxisIdY						1//Identifies the y axis.
#define VtChAxisIdY2					2//Identifies the secondary y axis.
#define VtChAxisIdZ						3//Identifies the z axis.

//FontStyle Constants
#define VtFontStyleBold					1//Applies the bold attribute to the font. 
#define VtFontStyleItalic				2//Applies the italic attribute to the font. 
#define VtFontStyleOutline				3//Applies the outline attribute to the font. 

//Orientation Constants
#define VtOrientationHorizontal			0//The text is displayed horizontally. 
#define VtOrientationVertical			1//The letters of the text are drawn one on top of each other from the top down. 
#define VtOrientationUp					2//The text is rotated to read from bottom to top. 
#define VtOrientationDown				3//The text is rotated to read from top to bottom. 

//LabelComponent Constants
#define VtChLabelComponentValue			0//The value of the data point appears in the label. Data points in XY, Polar, and Bubble charts actually have two or three values. The default label for these chart types display all values in a standard format. You can customize this format to highlight an individual data value. 
#define VtChLabelComponentPercent		1//The value of the data point is displayed in the label as a percentage of the total value of the series. 
#define VtChLabelComponentSeriesName	2//The series name is used to label the data point. This name is taken from the label associated with the column in the data grid.  
#define VtChLabelComponentPointName		3//The data point name is used to label the data point. 

