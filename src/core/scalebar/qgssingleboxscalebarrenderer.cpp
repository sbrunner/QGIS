/***************************************************************************
                            qgssingleboxscalebarrenderer.cpp
                            --------------------------------
    begin                : June 2008
    copyright            : (C) 2008 by Marco Hugentobler
    email                : marco.hugentobler@karto.baug.ethz.ch
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgssingleboxscalebarrenderer.h"
#include "qgsscalebarsettings.h"
#include "qgslayoututils.h"
#include <QList>
#include <QPainter>

void QgsSingleBoxScaleBarRenderer::draw( QgsRenderContext &context, const QgsScaleBarSettings &settings, const ScaleBarContext &scaleContext ) const
{
  if ( !context.painter() )
  {
    return;
  }
  QPainter *painter = context.painter();

  double barTopPosition = context.convertToPainterUnits( QgsLayoutUtils::fontAscentMM( settings.font() ) + settings.labelBarSpace() + settings.boxContentSpace(), QgsUnitTypes::RenderMillimeters );

  painter->save();
  if ( context.flags() & QgsRenderContext::Antialiasing )
    painter->setRenderHint( QPainter::Antialiasing, true );

  QPen pen = settings.pen();
  pen.setWidthF( context.convertToPainterUnits( pen.widthF(), QgsUnitTypes::RenderMillimeters ) );
  painter->setPen( pen );

  bool useColor = true; //alternate brush color/white
  double xOffset = context.convertToPainterUnits( firstLabelXOffset( settings ), QgsUnitTypes::RenderMillimeters );

  QList<double> positions = segmentPositions( scaleContext, settings );
  QList<double> widths = segmentWidths( scaleContext, settings );

  for ( int i = 0; i < positions.size(); ++i )
  {
    if ( useColor ) //alternating colors
    {
      painter->setBrush( settings.brush() );
    }
    else //secondary color
    {
      painter->setBrush( settings.brush2() );
    }

    QRectF segmentRect( context.convertToPainterUnits( positions.at( i ), QgsUnitTypes::RenderMillimeters ) + xOffset,
                        barTopPosition, context.convertToPainterUnits( widths.at( i ), QgsUnitTypes::RenderMillimeters ),
                        context.convertToPainterUnits( settings.height(), QgsUnitTypes::RenderMillimeters ) );
    painter->drawRect( segmentRect );
    useColor = !useColor;
  }

  painter->restore();

  //draw labels using the default method
  drawDefaultLabels( context, settings, scaleContext );
}



